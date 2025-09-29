#include <iostream>
#include <string>
#include <vector>

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <yyjson.h>
#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include "json_wrapper/yyjson_wrapper.hpp"

// ベンチマーク実行関数
void RunPerformanceBenchmark() {
    // 統一テストデータ
    const int int_field = 42;
    const double real_field = 3.14159;
    const std::string string_field = "test_string_日本語";
    const bool bool_field = true;
    const std::vector<int> array_field = {1, 2, 3, 4, 5};
    const std::vector<std::string> string_array = {"apple", "banana", "cherry"};
    const int nested_int = 10;
    const std::string nested_string = "nested_value";

    ankerl::nanobench::Bench bench;

    // 1. nlohmann/json ベンチマーク
    bench.run("nlohmann/json", [&] {
        nlohmann::json j;
        j["int_field"] = int_field;
        j["real_field"] = real_field;
        j["string_field"] = string_field;
        j["bool_field"] = bool_field;
        j["array_field"] = array_field;
        j["string_array"] = string_array;
        j["nested_field"]["nested_int"] = nested_int;
        j["nested_field"]["nested_string"] = nested_string;

        std::string result = j.dump();
        ankerl::nanobench::doNotOptimizeAway(result);
    });

    // 2. yyjson no-copy API ベンチマーク
    bench.run("yyjson no-copy API", [&] {
        auto *doc = yyjson_mut_doc_new(nullptr);
        auto *root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        yyjson_mut_obj_add_int(doc, root, "int_field", int_field);
        yyjson_mut_obj_add_real(doc, root, "real_field", real_field);
        yyjson_mut_obj_add_str(doc, root, "string_field", string_field.c_str());
        yyjson_mut_obj_add_bool(doc, root, "bool_field", bool_field);

        yyjson_mut_val *arr = yyjson_mut_arr(doc);
        for (int val : array_field) {
            yyjson_mut_arr_add_int(doc, arr, val);
        }
        yyjson_mut_obj_add_val(doc, root, "array_field", arr);

        yyjson_mut_val *str_arr = yyjson_mut_arr(doc);
        for (const auto &val : string_array) {
            yyjson_mut_arr_add_str(doc, str_arr, val.c_str());
        }
        yyjson_mut_obj_add_val(doc, root, "string_array", str_arr);

        yyjson_mut_val *nested = yyjson_mut_obj(doc);
        yyjson_mut_obj_add_int(doc, nested, "nested_int", nested_int);
        yyjson_mut_obj_add_str(doc, nested, "nested_string", nested_string.c_str());
        yyjson_mut_obj_add_val(doc, root, "nested_field", nested);

        char *json_str = yyjson_mut_write(doc, 0, nullptr);
        ankerl::nanobench::doNotOptimizeAway(json_str);

        std::free(static_cast<void *>(json_str)); // NOLINT(*-memory,*-malloc)
        yyjson_mut_doc_free(doc);
    });

    // 3. yyjson copy API ベンチマーク
    bench.run("yyjson copy API", [&] {
        auto *doc = yyjson_mut_doc_new(nullptr);
        auto *root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        yyjson_mut_obj_add_int(doc, root, "int_field", int_field);
        yyjson_mut_obj_add_real(doc, root, "real_field", real_field);
        yyjson_mut_obj_add_strcpy(doc, root, "string_field", string_field.c_str());
        yyjson_mut_obj_add_bool(doc, root, "bool_field", bool_field);

        yyjson_mut_val *arr = yyjson_mut_arr(doc);
        for (int val : array_field) {
            yyjson_mut_arr_add_int(doc, arr, val);
        }
        yyjson_mut_obj_add_val(doc, root, "array_field", arr);

        yyjson_mut_val *str_arr = yyjson_mut_arr(doc);
        for (const auto &val : string_array) {
            yyjson_mut_arr_add_strcpy(doc, str_arr, val.c_str());
        }
        yyjson_mut_obj_add_val(doc, root, "string_array", str_arr);

        yyjson_mut_val *nested = yyjson_mut_obj(doc);
        yyjson_mut_obj_add_int(doc, nested, "nested_int", nested_int);
        yyjson_mut_obj_add_strcpy(doc, nested, "nested_string", nested_string.c_str());
        yyjson_mut_obj_add_val(doc, root, "nested_field", nested);

        char *json_str = yyjson_mut_write(doc, 0, nullptr);
        ankerl::nanobench::doNotOptimizeAway(json_str);

        std::free(static_cast<void *>(json_str)); // NOLINT(*-memory,*-malloc)
        yyjson_mut_doc_free(doc);
    });

    // 4. yyjsonラッパー ベンチマーク
    bench.run("yyjsonラッパー", [&] {
        json::JsonBuilder builder;
        builder.Add("int_field", int_field);
        builder.Add("real_field", real_field);
        builder.Add("string_field", string_field);
        builder.Add("bool_field", bool_field);
        builder.Add("array_field", array_field);
        builder.Add("string_array", string_array);

        auto *nested = builder.AddNested("nested_field");
        builder.AddToNested(nested, "nested_int", nested_int);
        builder.AddToNested(nested, "nested_string", nested_string);

        std::string result = builder.Serialize();
        ankerl::nanobench::doNotOptimizeAway(result);
    });
}

int main() {
    std::cout << "=== nanobenchによるJSONライブラリ ベンチマーク ===\n";
    try {
        RunPerformanceBenchmark();
    } catch (const std::exception &e) {
        std::cerr << "エラー: " << e.what() << "\n" << std::flush;
        return 1;
    }

    return 0;
}
