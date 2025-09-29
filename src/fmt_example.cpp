#include <string>
#include <vector>

#include <fmt/format.h>

int main() {
    // 基本的な文字列フォーマット
    std::string name = "Alice";
    int age = 25;
    fmt::print("Hello, {}! You are {} years old.\n", name, age);

    // 数値フォーマット
    double pi = 3.14159265359;
    fmt::print("Pi with 2 decimals: {:.2f}\n", pi);
    fmt::print("Pi in scientific notation: {:.2e}\n", pi);

    // 位置指定フォーマット
    fmt::print("{1} comes after {0}\n", "first", "second");

    // パディングとアライメント
    fmt::print("Left aligned:   '{:<10}'\n", "text");
    fmt::print("Right aligned:  '{:>10}'\n", "text");
    fmt::print("Center aligned: '{:^10}'\n", "text");

    // 16進数とバイナリ
    int value = 255;
    fmt::print("Decimal: {}, Hex: {:x}, Binary: {:b}\n", value, value, value);

    // vectorの内容を表示
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    fmt::print("Numbers: [");
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (i > 0) {
            fmt::print(", ");
        }
        fmt::print("{}", numbers[i]);
    }
    fmt::print("]\n");

    return 0;
}
