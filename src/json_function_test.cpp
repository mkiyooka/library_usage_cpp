#include <chrono>
#include <fmt/format.h>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <yyjson.h>

#include "json_wrapper/yyjson_wrapper.hpp"

// 統一テストデータ
struct TestData {
    int basic_int = 42;
    double basic_double = 3.14159;
    std::string basic_string = "basic_value_日本語";
    bool basic_bool = true;
    std::vector<int> basic_array = {1, 2, 3, 4, 5};
    std::vector<std::string> string_array = {"item1", "item2", "アイテム3"};

    // ネスト構造用
    int nested_num1 = 10, nested_num2 = 20;
    std::string nested_str1 = "first_string";
    std::string nested_str2 = "second_string";
    std::string timestamp = "2025-09-17T15:19:52.854500UTC";
};

// 基準JSON (nlohmann/json) を生成
std::string generateBaselineJson(const TestData &data) {
    nlohmann::json j;
    j["basic_int"] = data.basic_int;
    j["basic_double"] = data.basic_double;
    j["basic_string"] = data.basic_string;
    j["basic_bool"] = data.basic_bool;
    j["basic_array"] = data.basic_array;
    j["string_array"] = data.string_array;
    j["timestamp"] = data.timestamp;

    // ネスト構造
    j["nested_numbers"]["num1"] = data.nested_num1;
    j["nested_numbers"]["num2"] = data.nested_num2;
    j["nested_strings"]["str1"] = data.nested_str1;
    j["nested_strings"]["str2"] = data.nested_str2;

    return j.dump();
}

// JSON比較（構造的比較）
bool compareJsonStructure(const std::string &json1, const std::string &json2) {
    try {
        auto j1 = nlohmann::json::parse(json1);
        auto j2 = nlohmann::json::parse(json2);
        return j1 == j2;
    } catch (...) {
        return false;
    }
}

// yyjson copy API テスト
std::string testYyjsonCopy(const TestData &data) {
    auto *doc = yyjson_mut_doc_new(nullptr);
    auto *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    yyjson_mut_obj_add_int(doc, root, "basic_int", data.basic_int);
    yyjson_mut_obj_add_real(doc, root, "basic_double", data.basic_double);
    yyjson_mut_obj_add_strcpy(doc, root, "basic_string", data.basic_string.c_str());
    yyjson_mut_obj_add_bool(doc, root, "basic_bool", data.basic_bool);
    yyjson_mut_obj_add_strcpy(doc, root, "timestamp", data.timestamp.c_str());

    // 配列
    yyjson_mut_val *arr = yyjson_mut_arr(doc);
    for (int val : data.basic_array) {
        yyjson_mut_arr_add_int(doc, arr, val);
    }
    yyjson_mut_obj_add_val(doc, root, "basic_array", arr);

    yyjson_mut_val *str_arr = yyjson_mut_arr(doc);
    for (const auto &val : data.string_array) {
        yyjson_mut_arr_add_strcpy(doc, str_arr, val.c_str());
    }
    yyjson_mut_obj_add_val(doc, root, "string_array", str_arr);

    // ネスト構造
    yyjson_mut_val *nested_numbers = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_int(doc, nested_numbers, "num1", data.nested_num1);
    yyjson_mut_obj_add_int(doc, nested_numbers, "num2", data.nested_num2);
    yyjson_mut_obj_add_val(doc, root, "nested_numbers", nested_numbers);

    yyjson_mut_val *nested_strings = yyjson_mut_obj(doc);
    yyjson_mut_obj_add_strcpy(doc, nested_strings, "str1", data.nested_str1.c_str());
    yyjson_mut_obj_add_strcpy(doc, nested_strings, "str2", data.nested_str2.c_str());
    yyjson_mut_obj_add_val(doc, root, "nested_strings", nested_strings);

    char *json_str = yyjson_mut_write(doc, 0, nullptr);
    std::string result(json_str);
    free(json_str);
    yyjson_mut_doc_free(doc);

    return result;
}

// yyjsonラッパー テスト
std::string testYyjsonWrapper(const TestData &data) {
    json::JsonBuilder builder;
    builder.Add("basic_int", data.basic_int);
    builder.Add("basic_double", data.basic_double);
    builder.Add("basic_string", data.basic_string);
    builder.Add("basic_bool", data.basic_bool);
    builder.Add("basic_array", data.basic_array);
    builder.Add("string_array", data.string_array);
    builder.Add("timestamp", data.timestamp);

    // ネスト構造
    auto *nested_numbers = builder.AddNested("nested_numbers");
    builder.AddToNested(nested_numbers, "num1", data.nested_num1);
    builder.AddToNested(nested_numbers, "num2", data.nested_num2);

    auto *nested_strings = builder.AddNested("nested_strings");
    builder.AddToNested(nested_strings, "str1", data.nested_str1);
    builder.AddToNested(nested_strings, "str2", data.nested_str2);

    return builder.Serialize();
}

// ライブラリ情報
struct LibraryInfo {
    std::string name;
    std::string description;
    bool copy_safe;
    std::function<std::string(const TestData &)> test_func;
};

int main() {
    std::cout << "=== JSON機能テスト・比較ツール ===\n";
    std::cout << "各ライブラリで統一データを生成し、出力比較を実施します。\n\n";

    TestData data;
    std::string baseline = generateBaselineJson(data);

    // テスト対象ライブラリ
    std::vector<LibraryInfo> libraries = {
        { "yyjson (copy)",     "高速かつ安全（copy API使用）", true,                             testYyjsonCopy},
        {"yyjsonラッパー",  "統一Add関数による高性能ラッパー", true,                          testYyjsonWrapper},
        { "nlohmann/json", "モダンC++ JSON（使いやすさ重視）", true, [&](const TestData &) { return baseline; }}
    };

    std::cout << "=== JSON出力比較 ===\n\n";

    bool all_match = true;
    for (const auto &lib : libraries) {
        std::cout << "### " << lib.name << "\n";
        std::cout << "Copy Safe: " << (lib.copy_safe ? "YES" : "WARNING") << "\n";
        std::cout << "Description: " << lib.description << "\n";

        try {
            std::string result = lib.test_func(data);
            bool matches = compareJsonStructure(baseline, result);
            std::cout << "Content matches baseline: " << (matches ? "PASS" : "FAIL") << "\n";
            if (!matches) {
                all_match = false;
            }
        } catch (const std::exception &e) {
            std::cout << "ERROR: " << e.what() << "\n";
            all_match = false;
        }
        std::cout << "\n";
    }
    std::cout << "=== 総合結果 ===\n";
    std::cout << "出力一致性: " << (all_match ? "PASS" : "FAIL") << "\n\n";

    return all_match ? 0 : 1;
}
