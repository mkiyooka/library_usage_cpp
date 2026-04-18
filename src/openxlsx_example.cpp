#include <iostream>
#include <string>
#include <vector>

#include <OpenXLSX.hpp>

static void WriteSpreadsheet() {
    std::cout << "=== Write Excel File ===\n";

    OpenXLSX::XLDocument doc;
    doc.create("output_example.xlsx");

    auto wks = doc.workbook().worksheet("Sheet1");

    // ヘッダー
    wks.cell("A1").value() = "Name";
    wks.cell("B1").value() = "Score";
    wks.cell("C1").value() = "Grade";

    // データ
    struct Record {
        std::string name;
        double score;
        std::string grade;
    };
    std::vector<Record> data = {
        {  "Alice", 92.5,  "A"},
        {    "Bob", 78.0,  "B"},
        {"Charlie", 85.5, "B+"},
        {  "Diana", 95.0, "A+"},
        {    "Eve", 60.0,  "C"},
    };

    for (size_t i = 0; i < data.size(); ++i) {
        int row = static_cast<int>(i) + 2;
        wks.cell(row, 1).value() = data[i].name;
        wks.cell(row, 2).value() = data[i].score;
        wks.cell(row, 3).value() = data[i].grade;
    }

    doc.save();
    doc.close();
    std::cout << "Written: output_example.xlsx\n";
}

static void ReadSpreadsheet() {
    std::cout << "\n=== Read Excel File ===\n";

    OpenXLSX::XLDocument doc;
    doc.open("output_example.xlsx");

    auto wks = doc.workbook().worksheet("Sheet1");

    std::cout << "Header: ";
    for (int col = 1; col <= 3; ++col) {
        std::cout << wks.cell(1, col).value().get<std::string>() << "  ";
    }
    std::cout << "\n";

    for (int row = 2; row <= 6; ++row) {
        std::cout << "  " << wks.cell(row, 1).value().get<std::string>() << "  "
                  << wks.cell(row, 2).value().get<double>() << "  " << wks.cell(row, 3).value().get<std::string>()
                  << "\n";
    }

    doc.close();
}

int main() {
    WriteSpreadsheet();
    ReadSpreadsheet();
    return 0;
}
