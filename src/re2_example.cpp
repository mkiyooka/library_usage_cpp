#include <iostream>
#include <string>
#include <vector>

#include <re2/re2.h>

int main() {
    std::cout << "=== RE2 Example ===\n\n";

    // --- FullMatch: 文字列全体にマッチ ---
    std::cout << "-- FullMatch --\n";
    RE2 date_re(R"((\d{4})-(\d{2})-(\d{2}))");
    std::string year, month, day;
    if (RE2::FullMatch("2026-04-19", date_re, &year, &month, &day)) {
        std::cout << "  date: year=" << year << " month=" << month << " day=" << day << "\n";
    }

    // --- PartialMatch: 部分マッチ ---
    std::cout << "\n-- PartialMatch --\n";
    RE2 num_re(R"(\d+)");
    std::string text = "foo 42 bar 100 baz";
    int num;
    if (RE2::PartialMatch(text, num_re, &num)) {
        std::cout << "  first number: " << num << "\n";
    }

    // --- FindAndConsume: 逐次検索 ---
    std::cout << "\n-- FindAndConsume (all numbers) --\n";
    re2::StringPiece sp(text);
    int n;
    while (RE2::FindAndConsume(&sp, num_re, &n)) {
        std::cout << "  found: " << n << "\n";
    }

    // --- Replace ---
    std::cout << "\n-- Replace --\n";
    std::string s = "Hello, World! Hello, RE2!";
    int count = RE2::GlobalReplace(&s, "Hello", "Greetings");
    std::cout << "  " << s << " (" << count << " replacements)\n";

    // --- 名前付きキャプチャグループ ---
    std::cout << "\n-- Named capture groups --\n";
    RE2 log_re(R"((?P<level>\w+)\s+(?P<msg>.+))");
    std::vector<std::string> lines = {"ERROR connection refused", "INFO server started",
                                      "WARN low memory"};
    for (const auto& line : lines) {
        std::string level, msg;
        if (RE2::FullMatch(line, log_re, &level, &msg)) {
            std::cout << "  [" << level << "] " << msg << "\n";
        }
    }

    // --- パターンの有効性チェック ---
    std::cout << "\n-- Pattern validation --\n";
    RE2 bad_re("(unclosed");
    if (!bad_re.ok()) {
        std::cout << "  invalid pattern: " << bad_re.error() << "\n";
    }

    return 0;
}
