#include "FirewallManager.h"
#include <iostream>
#include <cstdlib> // For system()
#include <sstream>

// Hàm thực thi lệnh và in ra màn hình
void FirewallManager::executeCommand(const std::string& command) {
#ifdef _WIN32
    // On Windows we cannot run iptables/tc. Print the command instead (dry-run).
    std::cout << "[DRY-RUN on Windows] Would execute: " << command << std::endl;
#else
    std::cout << "Executing: " << command << std::endl;
    int ret = system(command.c_str());
    if (ret != 0) {
        std::cerr << "Command exited with code " << ret << std::endl;
    }
#endif
}

void FirewallManager::addRule(const FirewallRule& rule) {
    rules.push_back(rule);
}

void FirewallManager::flushRules() {
    std::cout << "Flushing all existing firewall rules..." << std::endl;
    executeCommand("sudo iptables -F INPUT");  // Xóa các quy tắc trong chuỗi INPUT
    executeCommand("sudo iptables -F OUTPUT"); // Xóa các quy tắc trong chuỗi OUTPUT
    executeCommand("sudo iptables -F FORWARD");
    executeCommand("sudo iptables -P INPUT ACCEPT");   // Đặt chính sách mặc định là ACCEPT
    executeCommand("sudo iptables -P OUTPUT ACCEPT");
    executeCommand("sudo iptables -P FORWARD ACCEPT");
    rules.clear();
    std::cout << "All rules have been flushed." << std::endl;
}

void FirewallManager::applyRules() {
    if (rules.empty()) {
        std::cout << "No rules to apply." << std::endl;
        return;
    }

    std::cout << "Applying firewall rules..." << std::endl;
    // Đặt chính sách mặc định là DROP nếu muốn bảo mật cao hơn (chỉ cho phép những gì đã định nghĩa)
    // executeCommand("sudo iptables -P INPUT DROP");
    // executeCommand("sudo iptables -P OUTPUT DROP");


    for (const auto& rule : rules) {
        std::stringstream cmd;
        cmd << "sudo iptables -A ";

        // Xác định chuỗi (chain) dựa trên hướng
        cmd << (rule.direction == Direction::IN ? "INPUT" : "OUTPUT");

        // Giao thức
        if (rule.protocol != "any") {
            cmd << " -p " << rule.protocol;
        }

        // IP nguồn
        if (rule.source_ip != "any") {
            cmd << " -s " << rule.source_ip;
        }

        // IP đích
        if (rule.dest_ip != "any") {
            cmd << " -d " << rule.dest_ip;
        }

        // Cổng (chỉ áp dụng cho TCP/UDP)
        if (rule.port != -1 && (rule.protocol == "tcp" || rule.protocol == "udp")) {
            cmd << " --dport " << rule.port;
        }

        // Hành động
        cmd << " -j " << (rule.action == Action::ALLOW ? "ACCEPT" : "DROP");

        executeCommand(cmd.str());
    }
    std::cout << "Firewall rules applied successfully." << std::endl;
}

void FirewallManager::listRules() const {
    if (rules.empty()) {
        std::cout << "No rules are currently defined." << std::endl;
        return;
    }

    std::cout << "------ Defined Firewall Rules ------" << std::endl;
    for (const auto& rule : rules) {
        std::cout << "Name: " << rule.name
                  << ", Dir: " << (rule.direction == Direction::IN ? "IN" : "OUT")
                  << ", Action: " << (rule.action == Action::ALLOW ? "ALLOW" : "BLOCK")
                  << ", Proto: " << rule.protocol
                  << ", Port: " << (rule.port == -1 ? "any" : std::to_string(rule.port))
                  << ", Source: " << rule.source_ip
                  << ", Dest: " << rule.dest_ip << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;
}

void FirewallManager::setTrafficShaping(const std::string& interface, const std::string& rate) {
    std::cout << "Applying traffic shaping on interface " << interface << "..." << std::endl;
    // Xóa các quy tắc tc cũ trên giao diện
    std::string command_flush = "sudo tc qdisc del dev " + interface + " root";
    executeCommand(command_flush);

    // Thêm quy tắc mới để giới hạn băng thông
    std::string command_add = "sudo tc qdisc add dev " + interface + " root tbf rate " + rate + " burst 32kbit latency 400ms";
    executeCommand(command_add);
    std::cout << "Traffic shaping rule applied." << std::endl;
}