#ifndef FIREWALL_MANAGER_H
#define FIREWALL_MANAGER_H

#include <vector>
#include <string>
#include "FirewallRule.h"

class FirewallManager {
public:
    // Thêm một quy tắc mới vào danh sách
    void addRule(const FirewallRule& rule);

    // Xóa tất cả các quy tắc đã áp dụng và làm sạch iptables
    void flushRules();

    // Áp dụng tất cả các quy tắc trong danh sách vào iptables
    void applyRules();

    // Hiển thị tất cả các quy tắc đang được quản lý
    void listRules() const;

    // Thiết lập quy tắc định hình lưu lượng (giới hạn băng thông)
    void setTrafficShaping(const std::string& interface, const std::string& rate);

private:
    std::vector<FirewallRule> rules;

    // Hàm tiện ích để thực thi một lệnh hệ thống
    void executeCommand(const std::string& command);
};

#endif // FIREWALL_MANAGER_H