#include <iostream>
#include <string_view>
#include <vector>

#include <ctre.hpp>

// 文字クラス内の - はエスケープするか末尾に置く
inline constexpr auto EMAIL_RE = ctll::fixed_string{R"(([\w.+\-]+)@([\w\-]+\.[\w.]+))"};
inline constexpr auto IPV4_RE = ctll::fixed_string{R"((\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3}))"};
inline constexpr auto LOG_RE = ctll::fixed_string{R"(\[(\w+)\] (.+))"};

int main() {
    std::cout << "=== CTRE (Compile-Time Regular Expressions) Example ===\n\n";

    // メールアドレスマッチ
    std::cout << "-- Email matching --\n";
    std::vector<std::string_view> candidates = {"user@example.com", "bad-address", "alice.b@corp.co.jp", "no_at_sign"};
    for (auto s : candidates) {
        if (auto m = ctre::match<EMAIL_RE>(s)) {
            std::cout << "  MATCH: " << s << "  user=" << m.get<1>() << "  domain=" << m.get<2>() << "\n";
        } else {
            std::cout << "  no match: " << s << "\n";
        }
    }

    // IPv4 検索 (search_all)
    std::cout << "\n-- IPv4 search in text --\n";
    std::string_view text = "Connect to 192.168.1.100 or fallback 10.0.0.1 done.";
    for (auto m : ctre::search_all<IPV4_RE>(text)) {
        std::cout << "  Found: " << m.get<0>() << "\n";
    }

    // ログ行パース
    std::cout << "\n-- Log line parsing --\n";
    std::vector<std::string_view> logs = {
        "[INFO] Server started", "[ERROR] Connection refused", "[WARN] Low memory", "invalid line"
    };
    for (auto line : logs) {
        if (auto m = ctre::match<LOG_RE>(line)) {
            std::cout << "  level=" << m.get<1>() << "  msg=" << m.get<2>() << "\n";
        }
    }

    return 0;
}
