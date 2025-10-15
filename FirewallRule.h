#ifndef FIREWALL_RULE_H
#define FIREWALL_RULE_H

#include <string>

// Enum để xác định hành động của quy tắc (Cho phép hoặc Chặn)
enum class Action {
    ALLOW,
    BLOCK
};

// Enum để xác định hướng của lưu lượng
enum class Direction {
    IN,
    OUT
};

// Cấu trúc đại diện cho một quy tắc tường lửa
struct FirewallRule {
    std::string name;         // Tên quy tắc để dễ nhận diện
    std::string source_ip;    // Địa chỉ IP nguồn ("any" nếu không xác định)
    std::string dest_ip;      // Địa chỉ IP đích ("any" nếu không xác định)
    std::string protocol;     // Giao thức (tcp, udp, icmp, hoặc "any")
    int port = -1;            // Cổng (-1 nếu áp dụng cho mọi cổng)
    Action action = Action::ALLOW; // Hành động mặc định
    Direction direction = Direction::IN; // Hướng mặc định
};

#endif // FIREWALL_RULE_H