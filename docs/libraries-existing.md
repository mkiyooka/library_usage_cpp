# 既存導入ライブラリ

このプロジェクトに既に組み込まれているライブラリの機能まとめです。

---

## CLI11 — コマンドライン引数解析

- **バージョン**: 2.5.0
- **ライセンス**: BSD-3-Clause
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/cli11_example.cpp`, `src/cli11_commands.cpp`

### 主な機能

- サブコマンド（`add`, `process` など）の定義
- 必須・オプション引数、フラグの宣言
- 型自動変換（`int`, `double`, `string`, `vector` など）
- ヘルプメッセージの自動生成
- バリデーター（ファイル存在チェックなど）

### 使い方の例

```cpp
CLI::App app{"My App"};
auto* sub = app.add_subcommand("process", "Process a file");
std::string input;
sub->add_option("-i,--input", input, "Input file")->required();
CLI11_PARSE(app, argc, argv);
```

---

## fmt — 文字列フォーマット

- **バージョン**: 12.0.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅（ヘッダーオンリーモード選択可）
- **サンプル**: `src/fmt_example.cpp`

### 主な機能

- Python 風の `{}` プレースホルダーによるフォーマット
- コンパイル時フォーマット文字列チェック
- カスタム型のフォーマット対応
- `fmt::print` による直接出力（`printf` 相当）
- C++20 `std::format` の先駆け（ほぼ互換）

### 使い方の例

```cpp
fmt::print("Hello, {}!\n", name);
std::string s = fmt::format("{:.2f}", 3.14159);
```

---

## nlohmann_json — JSON 処理

- **バージョン**: 3.12.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/json_loader.cpp`, `src/json_writer.cpp`

### 主な機能

- JSON の読み書き（文字列・ファイル）
- C++ オブジェクトとの変換（`from_json` / `to_json` マクロ）
- ポインタ（JSON Pointer）・パッチ（JSON Patch）対応
- `std::map`, `std::vector` などの標準コンテナと自動変換

### 使い方の例

```cpp
nlohmann::json j = {{"name", "Alice"}, {"score", 95}};
std::string name = j["name"];
j["items"].push_back(42);
```

---

## yyjson — 高速 JSON 処理

- **バージョン**: 0.12.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ❌（C ライブラリ）
- **サンプル**: `src/json_benchmark.cpp`, `src/json_function_test.cpp`（`include/json_wrapper/` 経由）

### 主な機能

- C 言語実装の超高速 JSON パーサー
- 読み取り専用・変更可能の2モード
- SIMD 最適化済み
- nlohmann_json と比較して 5〜10 倍の速度

---

## tomlplusplus — TOML 設定ファイル解析

- **バージョン**: 3.4.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/toml_loader.cpp`

### 主な機能

- TOML v1.0 完全準拠
- 型安全なアクセス（`value_or`, `as_array`, `as_table`）
- 配列・テーブル・インラインテーブルのネスト対応
- ファイル・文字列からのパース

---

## yaml-cpp — YAML パーサー・エミッター

- **バージョン**: 0.9.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ❌（コンパイルが必要）
- **サンプル**: `src/yaml_example.cpp`

### 主な機能

- YAML ファイルの読み込み（`YAML::LoadFile`）
- ノードアクセス（`node["key"].as<T>()`）
- `YAML::Emitter` による YAML 文字列の生成
- シーケンス・マップのイテレーション

### 使い方の例

```cpp
YAML::Node root = YAML::LoadFile("config.yaml");
std::string title = root["title"].as<std::string>();

YAML::Emitter out;
out << YAML::BeginMap << YAML::Key << "name" << YAML::Value << "test" << YAML::EndMap;
```

---

## indicators — プログレスバー表示

- **バージョン**: 2.3
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/indicator_example.cpp`

### 主な機能

- 進捗バー・スピナー・マルチバーの表示
- カスタムスタイル（色、文字、幅）
- 非同期スレッドセーフ更新

---

## quill — 高性能ログ出力

- **バージョン**: 11.0.2
- **ライセンス**: MIT
- **ヘッダーオンリー**: ❌
- **サンプル**: `src/quill_example.cpp`

### 主な機能

- 低レイテンシ非同期ログ（バックグラウンドスレッド）
- フロントエンドのログ書き込みが数 ns 以下
- ファイル・コンソール・ローテーションハンドラ
- Python logging 風のログレベル（DEBUG/INFO/WARN/ERROR）

---

## spdlog — 高速 C++ ログ出力

- **バージョン**: 1.17.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅（または事前コンパイル可）
- **サンプル**: `src/spdlog_example.cpp`

### 主な機能

- fmt ベースのフォーマット
- 複数シンク（コンソール・ファイル・ローテーション）への同時出力
- スレッドセーフ同期・非同期モード
- ログレベルのランタイム変更

---

## nanobench — ベンチマーク測定

- **バージョン**: 4.3.11
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅
- **サンプル**: `benches/bench_logger.cpp`, `src/json_benchmark.cpp`

### 主な機能

- ウォームアップ付きの精度の高いマイクロベンチマーク
- 統計情報（median, MAD など）の出力
- JSON/HTML 出力対応
- 比較結果の表示

---

## cppzmq / libzmq — ZeroMQ メッセージング

- **バージョン**: cppzmq 4.11.0 / libzmq 4.3.5
- **ライセンス**: MIT / MPL-2.0
- **サンプル**: `src/zmq_server.cpp`, `src/zmq_client.cpp`

### 主な機能

- Pub/Sub, Push/Pull, Req/Rep, Dealer/Router パターン
- TCP・IPC・inproc トランスポート
- 非同期メッセージ送受信
- ゼロコピーメッセージ

---

## doctest — テストフレームワーク

- **バージョン**: 2.4.12
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅
- **サンプル**: `tests/`

### 主な機能

- `TEST_CASE`, `SUBCASE` によるテスト定義
- `CHECK`, `REQUIRE` などのアサーション
- コンパイル時間が短く既存コードへの埋め込みが容易
