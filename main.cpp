#include <iostream>
#include "FirewallManager.h"

void printMenu() {
    std::cout << "\n--- Custom Firewall Menu ---\n";
    std::cout << "1. Add a new rule\n";
    std::cout << "2. List all rules\n";
    std::cout << "3. Apply all rules\n";
    std::cout << "4. Flush all rules (clear iptables)\n";
    std::cout << "5. Set traffic shaping\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    FirewallManager firewall;
    int choice;

    do {
        printMenu();
        std::cin >> choice;

        if (choice == 1) {
            FirewallRule newRule;
            int actionChoice, dirChoice;
            
            std::cout << "Enter rule name: ";
            std::cin >> newRule.name;
            std::cout << "Enter source IP ('any' for all): ";
            std::cin >> newRule.source_ip;
            std::cout << "Enter destination IP ('any' for all): ";
            std::cin >> newRule.dest_ip;
            std::cout << "Enter protocol (tcp, udp, icmp, or 'any'): ";
            std::cin >> newRule.protocol;
            std::cout << "Enter port (-1 for all): ";
            std::cin >> newRule.port;
            
            std::cout << "Enter action (1 for ALLOW, 2 for BLOCK): ";
            std::cin >> actionChoice;
            newRule.action = (actionChoice == 1) ? Action::ALLOW : Action::BLOCK;

            std::cout << "Enter direction (1 for IN, 2 for OUT): ";
            std::cin >> dirChoice;
            newRule.direction = (dirChoice == 1) ? Direction::IN : Direction::OUT;

            firewall.addRule(newRule);
            std::cout << "Rule added successfully.\n";
        } else if (choice == 2) {
            firewall.listRules();
        } else if (choice == 3) {
            firewall.applyRules();
        } else if (choice == 4) {
            firewall.flushRules();
        } else if (choice == 5) {
            std::string interface, rate;
            std::cout << "Enter network interface (e.g., eth0): ";
            std::cin >> interface;
            std::cout << "Enter rate limit (e.g., 1mbit, 512kbit): ";
            std::cin >> rate;
            firewall.setTrafficShaping(interface, rate);
        }

    } while (choice != 6);

    std::cout << "Exiting firewall program.\n";
    return 0;
}