# C++ Library Usage Examples

C++ライブラリの使用例を集めたサンプルプロジェクトです。CMake FetchContentを使用してライブラリを管理しています。

## 導入ライブラリ

- **CLI11**: コマンドライン引数解析
- **fmt**: 文字列フォーマット
- **nlohmann_json**: JSON処理
- **yyjson**: 高速JSON処理
- **tomlplusplus**: TOML設定ファイル解析
- **indicators**: プログレスバー表示
- **quill**: 高性能ログ出力
- **nanobench**: ベンチマーク測定
- **doctest**: テストフレームワーク
- **cppzmq / libzmq**: ZeroMQメッセージング

## セットアップ

```bash
# Pixi環境のインストール
pixi install

# CMake設定とビルド
pixi run config
pixi run build

# テスト実行
pixi run test
```

## サンプル実行例

```bash
# CLI11 - コマンドライン解析
./build/src/cli11_example process -i input.txt -o output.txt

# JSON処理比較
./build/src/json_function_test

# JSONベンチマーク
./build/src/json_benchmark

# プログレスバー
./build/src/indicator_example

# ログ出力
./build/src/quill_example

# ZeroMQメッセージング（別々のターミナルで実行）
./build/src/zmq_server
./build/src/zmq_client
```

## 開発ツール

```bash
# コードフォーマット
pixi run format

# 静的解析
pixi run lint

# 全チェック実行
pixi run fullcheck
```

## ディレクトリ構造

- `src/`: 各ライブラリのサンプルコード
- `include/option_parsing/`: CLI11サブコマンド定義
- `include/json_wrapper/`: yyjsonラッパー
- `config_example/`: 設定ファイルサンプル（JSON/TOML）
- `tests/`: テストコード
