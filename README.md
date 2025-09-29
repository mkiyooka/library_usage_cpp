# C++ Library Usage Examples

C++ライブラリの使用例を集めたサンプルプロジェクトです。

## ライブラリサンプル

- **CLI11**: コマンドライン引数解析
- **fmt**: 文字列フォーマット
- **nlohmann_json**: JSON処理
- **yyjson**: 高速JSON処理 + カスタムラッパー
- **tomlplusplus**: TOML設定ファイル
- **indicators**: プログレスバー
- **quill**: 高性能ログ出力

## セットアップ

```bash
# 環境構築
pixi install

# ビルド
pixi run setup
pixi run build
```

## サンプル実行例

```bash
# CLI11 - コマンドライン解析
./build/src/cli11_example process -i input.txt -o output.txt

# JSON処理比較
./build/src/json_function_test

# プログレスバー
./build/src/indicator_example

# ログ出力
./build/src/quill_example
```

## ディレクトリ構造

```text
src/                        # 各ライブラリのサンプルコード
include/option_parsing/     # オプション解析関連
include/json_wrapper/       # JSONラッパー
config_example/             # 設定ファイルサンプル
```
