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

---

## Catch2 — テストフレームワーク

- **バージョン**: 3.8.1
- **ライセンス**: BSL-1.0
- **ヘッダーオンリー**: ❌（v3 からコンパイルが必要）
- **サンプル**: `tests/test_catch2.cpp`

### 主な機能

- `TEST_CASE`, `SECTION` による BDD 風テスト構造
- `REQUIRE_THAT`, `CHECK_THAT` マッチャー
- テンプレートテスト (`TEMPLATE_TEST_CASE`)
- ベンチマーク機能内蔵 (`BENCHMARK`)
- **推奨**: GoogleTest と機能がほぼ同等だが、外部依存なし・ヘッダー構成がシンプル

---

## GoogleTest — テストフレームワーク

- **バージョン**: 1.16.0
- **ライセンス**: BSD-3-Clause
- **ヘッダーオンリー**: ❌
- **サンプル**: `tests/test_gtest.cpp`

### 主な機能

- `TEST`, `TEST_F` によるテスト・フィクスチャ定義
- `EXPECT_*`, `ASSERT_*` アサーション群
- `EXPECT_THAT` + Matcher の組み合わせ
- パラメータ化テスト (`TEST_P`, `INSTANTIATE_TEST_SUITE_P`)
- Google Mock 内蔵
- **推奨**: 業界標準。RapidCheck・ApprovalTests との統合が容易

---

## RapidCheck — プロパティベーステスト

- **バージョン**: HEAD (2024-09)
- **ライセンス**: BSD-2-Clause
- **ヘッダーオンリー**: ❌
- **サンプル**: `tests/test_rapidcheck.cpp`

### 主な機能

- Haskell QuickCheck の C++ 移植
- 入力値の自動生成・縮小 (shrinking)
- GoogleTest/Catch2 統合マクロ (`RC_GTEST_PROP`)
- カスタムジェネレータ (`rc::gen::*`)
- **推奨**: 境界値・回帰テストを網羅的に行う場合に有効

---

## ApprovalTests — スナップショットテスト

- **バージョン**: 10.13.0
- **ライセンス**: Apache-2.0
- **ヘッダーオンリー**: ✅（単一ヘッダー）
- **サンプル**: `tests/test_approvals.cpp`

### 主な機能

- 出力文字列をファイル（`.approved.txt`）と比較
- 差分が出たら承認ファイルを更新するだけでテスト更新
- GoogleTest / Catch2 / doctest 対応
- 複雑なオブジェクトや大量出力の「ゴールデンファイル」テストに最適
- **推奨**: 出力形式が安定しているが頻繁に変化するレポート・シリアライズ出力の検証に有効

---

## oneTBB — 並列アルゴリズム

- **バージョン**: 2022.1.0
- **ライセンス**: Apache-2.0
- **ヘッダーオンリー**: ❌
- **サンプル**: `src/onetbb_example.cpp`

### 主な機能

- `tbb::parallel_for`, `tbb::parallel_reduce`, `tbb::parallel_sort`
- `tbb::concurrent_vector`, `tbb::concurrent_hash_map`
- `tbb::task_arena` によるスレッド数・優先度制御
- フロー グラフ (Flow Graph) による DAG 並列化
- **推奨**: CPU 並列化の標準的な選択肢。Taskflow より低レベル API で細かい制御が可能

---

## Taskflow — タスクグラフ並列化

- **バージョン**: 3.9.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/taskflow_example.cpp`

### 主な機能

- DAG タスクグラフの宣言的な定義 (`taskflow.emplace`)
- `for_each_index` によるデータ並列ループ
- 条件付きタスク (condition task) によるループ・分岐
- 非同期・サブフローのサポート
- **推奨**: タスク依存関係を明示したい場合に oneTBB より直感的

---

## Zstandard — 高速圧縮

- **バージョン**: 1.5.7
- **ライセンス**: BSD-3-Clause / GPL-2.0
- **ヘッダーオンリー**: ❌
- **サンプル**: `src/zstd_example.cpp`

### 主な機能

- 高速圧縮・展開（LZ4 並みの速度と gzip 並みの圧縮率を両立）
- 圧縮レベル 1〜22（`ZSTD_maxCLevel()`）
- ストリーミング API (`ZSTD_CStream`, `ZSTD_DStream`)
- 辞書圧縮 (Dictionary) で小さいデータでも高圧縮率
- **推奨**: ファイル・ネットワークデータの圧縮で LZ4 の後継として広く使われる

---

## CTRE — コンパイル時正規表現

- **バージョン**: 3.10.1
- **ライセンス**: Apache-2.0
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/ctre_example.cpp`

### 主な機能

- 正規表現をコンパイル時に検証・最適化
- `ctre::match`, `ctre::search`, `ctre::search_all`
- キャプチャグループへの型安全アクセス (`m.get<N>()`)
- ゼロオーバーヘッドのパターンマッチ（実行時正規表現エンジン不要）
- **推奨**: パターンが固定の場合に RE2 より高速。`\-` のエスケープなど PCRE と細部が異なる

---

## RE2 — 高速正規表現

- **バージョン**: 2024-11-01
- **ライセンス**: BSD-3-Clause
- **ヘッダーオンリー**: ❌
- **サンプル**: `src/re2_example.cpp`

### 主な機能

- 線形時間保証（壊滅的バックトラックなし）
- `RE2::FullMatch`, `RE2::PartialMatch`, `RE2::FindAndConsume`
- 名前付きキャプチャグループ (`(?P<name>...)`)
- `RE2::GlobalReplace` による文字列置換
- **推奨**: 動的パターン・ユーザー入力のパターンには CTRE より安全

---

## msgpack-c — MessagePackシリアライゼーション

- **バージョン**: 7.0.0
- **ライセンス**: BSL-1.0
- **ヘッダーオンリー**: ✅（C++ モード）
- **サンプル**: `src/msgpack_example.cpp`

### 主な機能

- `MSGPACK_DEFINE` マクロでカスタム型をシリアライズ
- `msgpack::pack` / `msgpack::unpack` による pack/unpack
- バイナリフォーマットで cereal より軽量・高速
- 言語間互換性が高い（Python/Ruby/Rust などで相互運用可能）
- **用途**: 軽量な IPC・通信プロトコルとして有効。cereal より通信用途向き

---

## Cap'n Proto — 高速シリアライゼーション・RPC

- **バージョン**: 1.1.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ❌
- **サンプル**: `src/capnproto_example.cpp`, `proto/person.capnp`

### 主な機能

- IDL (`.capnp`) からの C++ コード自動生成
- ゼロコピーのメモリ表現（エンコード・デコードなし）
- Cap'n Proto RPC による非同期 RPC フレームワーク
- Protocol Buffers よりも高速（パース処理なし）
- **用途**: Protobuf の代替として。gRPC の代わりに Cap'n Proto RPC で軽量な通信システムを構築可能

---

## Apache Arrow — 列指向データフォーマット

- **バージョン**: 19.0.1
- **ライセンス**: Apache-2.0
- **ヘッダーオンリー**: ❌
- **サンプル**: `src/arrow_example.cpp`

### 主な機能

- 言語間共通の列指向メモリレイアウト (Arrow IPC)
- `Int32Builder`, `DoubleBuilder`, `StringBuilder` によるバッファ構築
- `arrow::Table` による複数カラム管理
- Parquet・Feather などのファイルフォーマット対応
- Python/R/Java など多言語との相互運用
- **用途**: DataFrameライクな列指向データ処理・大規模データのプロセス間共有
