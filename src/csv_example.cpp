#include <iostream>
#include <sstream>
#include <vector>

#include <csv.hpp>

int main() {
    std::cout << "=== csv-parser Example ===\n\n";

    // インメモリCSVをパース
    std::string data = "name,age,score\n"
                       "Alice,30,95.5\n"
                       "Bob,25,87.0\n"
                       "Carol,28,92.3\n"
                       "Dave,35,78.9\n";

    std::cout << "-- Parsing CSV from string --\n";
    csv::CSVReader reader(csv::parse(data));
    for (auto &row : reader) {
        std::string name = row["name"].get<std::string>();
        int age = row["age"].get<int>();
        double score = row["score"].get<double>();
        std::cout << "  " << name << "  age=" << age << "  score=" << score << "\n";
    }

    // 統計情報
    std::cout << "\n-- Column stats --\n";
    csv::CSVReader reader2(csv::parse(data));
    std::vector<double> scores;
    for (auto &row : reader2) {
        scores.push_back(row["score"].get<double>());
    }
    double sum = 0;
    for (double s : scores)
        sum += s;
    double avg = sum / static_cast<double>(scores.size());
    std::cout << "  score avg: " << avg << "\n";
    std::cout << "  rows: " << scores.size() << "\n";

    // CSVを書き出す
    std::cout << "\n-- Writing CSV --\n";
    auto writer = csv::make_csv_writer(std::cout);
    writer << std::vector<std::string>{"id", "value"};
    writer << std::make_tuple(1, "hello");
    writer << std::make_tuple(2, "world");

    return 0;
}
