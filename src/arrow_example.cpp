#include <iostream>
#include <memory>

#include <arrow/api.h>
#include <arrow/array.h>
#include <arrow/builder.h>
#include <arrow/table.h>
#include <arrow/type.h>

int main() {
    std::cout << "=== Apache Arrow Example ===\n\n";

    // Int32配列のビルド
    arrow::Int32Builder int_builder;
    (void)int_builder.AppendValues({1, 2, 3, 4, 5});
    std::shared_ptr<arrow::Array> int_array;
    (void)int_builder.Finish(&int_array);

    // Float64配列のビルド
    arrow::DoubleBuilder dbl_builder;
    (void)dbl_builder.AppendValues({1.1, 2.2, 3.3, 4.4, 5.5});
    std::shared_ptr<arrow::Array> dbl_array;
    (void)dbl_builder.Finish(&dbl_array);

    // String配列のビルド
    arrow::StringBuilder str_builder;
    (void)str_builder.AppendValues({"alice", "bob", "carol", "dave", "eve"});
    std::shared_ptr<arrow::Array> str_array;
    (void)str_builder.Finish(&str_array);

    // スキーマ定義
    auto schema = arrow::schema(
        {arrow::field("id", arrow::int32()), arrow::field("score", arrow::float64()),
         arrow::field("name", arrow::utf8())}
    );

    // テーブル作成
    auto table = arrow::Table::Make(schema, {int_array, dbl_array, str_array});
    std::cout << "Schema:\n" << schema->ToString() << "\n";
    std::cout << "Rows: " << table->num_rows() << ", Columns: " << table->num_columns() << "\n\n";

    // 列アクセス
    auto id_col = std::static_pointer_cast<arrow::Int32Array>(table->column(0)->chunk(0));
    auto score_col = std::static_pointer_cast<arrow::DoubleArray>(table->column(1)->chunk(0));
    auto name_col = std::static_pointer_cast<arrow::StringArray>(table->column(2)->chunk(0));

    std::cout << "id  score  name\n";
    std::cout << "--- ------ -----\n";
    for (int64_t i = 0; i < table->num_rows(); ++i) {
        std::cout << id_col->Value(i) << "   " << score_col->Value(i) << "  " << name_col->GetString(i) << "\n";
    }

    // ChunkedArray スライス
    std::cout << "\nSliced table (rows 1-3):\n";
    auto sliced = table->Slice(1, 3);
    auto s_id = std::static_pointer_cast<arrow::Int32Array>(sliced->column(0)->chunk(0));
    for (int64_t i = 0; i < sliced->num_rows(); ++i) {
        std::cout << "  id=" << s_id->Value(i) << "\n";
    }

    return 0;
}
