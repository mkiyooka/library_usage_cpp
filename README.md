# C++ Library Usage Examples

C++ライブラリの使用例を集めたサンプルプロジェクトです。CMake FetchContentを使用してライブラリを管理しています。

## プロジェクト構成

- **ビルドシステム**: CMake 3.19+ + Ninja
- **環境管理**: Pixi (クロスプラットフォーム対応)
- **テストフレームワーク**: doctest
- **C++標準**: C++17

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
./build/cli11_example process -i input.txt -o output.txt

# JSON処理比較
./build/json_function_test

# JSONベンチマーク
./build/json_benchmark

# プログレスバー
./build/indicator_example

# ログ出力
./build/quill_example

# ZeroMQメッセージング（別々のターミナルで実行）
./build/zmq_server
./build/zmq_client

# テスト実行
./build/tests/test_sub
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

## ディレクトリ構成

```markdown
- `src/`: 各ライブラリのサンプルコード
    - `sub/`: サブモジュールサンプル
- `include/`: ヘッダーファイル
    - `option_parsing/`: CLI11サブコマンド定義
    - `json_wrapper/`: yyjsonラッパー
- `tests/`: テストコード
- `cmake/`: CMake設定ファイル
    - `local-or-fetch.cmake`: FetchContentヘルパー
    - `dependencies-app.cmake`: アプリ用ライブラリ
    - `dependencies-test.cmake`: テスト用ライブラリ
    - `custom-targets.cmake`: カスタムターゲット
    - `quality-setup.cmake`: コード品質設定
    - `quality-tools.cmake`: コード品質ツール
    - `collect-fetchcontent-licenses.cmake`: ライセンス収集
- `config_example/`: 設定ファイルサンプル（JSON/TOML）
- `third_party/`: サードパーティライブラリ（ローカルキャッシュ）
```
