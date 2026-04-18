# C++ Library Usage Examples

C++ライブラリの使用例を集めたサンプルプロジェクトです。CMake FetchContentを使用してライブラリを管理しています。

## プロジェクト構成

- **ビルドシステム**: CMake 3.19+ + Ninja
- **環境管理**: Pixi (クロスプラットフォーム対応)
- **テストフレームワーク**: doctest
- **C++標準**: C++17

## 導入ライブラリ

### 汎用ユーティリティ

- **CLI11**: コマンドライン引数解析
- **fmt**: 文字列フォーマット
- **nlohmann_json**: JSON処理
- **yyjson**: 高速JSON処理
- **tomlplusplus**: TOML設定ファイル解析
- **yaml-cpp**: YAMLパーサー・エミッター
- **indicators**: プログレスバー表示
- **quill**: 高性能ログ出力
- **spdlog**: 高速C++ログ出力
- **nanobench**: ベンチマーク測定
- **doctest**: テストフレームワーク
- **cppzmq / libzmq**: ZeroMQメッセージング
- **gRPC / Protobuf**: RPCフレームワーク（オプション）

### 科学技術計算

- **Eigen**: 線形代数（行列演算・固有値・SVD等）
- **EigenRand**: Eigen行列に対する高速乱数生成（正規分布・一様分布・ベルヌーイ等）
- **xtensor**: N次元配列（NumPy互換）
- **Armadillo**: MATLAB風線形代数（BLAS/LAPACK連携）
- **Ceres Solver**: 非線形最小二乗最適化（Google製）
- **NLopt**: 非線形最適化（40+アルゴリズム）
- **xsimd**: SIMD抽象化（SSE/AVX/NEON/SVE）
- **Highway**: ポータブルSIMD（Google製）
- **Kokkos**: 性能ポータビリティ（CPU/GPU並列化）
- **pocketfft**: 高速FFT（NumPy/SciPy内部実装）
- **Matplot++**: C++プロットライブラリ（Matplotlib互換）
- **OpenXLSX**: Excel XLSX 読み書き
- **cereal**: C++シリアライゼーション（JSON/バイナリ/XML）
- **HDF5 C++**: 科学技術データフォーマット

詳細は [`docs/`](docs/) を参照してください。

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

## gRPCを有効化してビルドする場合

gRPCはダウンロード・ビルドに時間を要するため、デフォルトでは無効になっています。
有効化するには `ENABLE_GRPC=ON` オプションを指定してください。

```bash
# gRPCありのビルド（pixi環境が必要）
cmake --preset=release -DENABLE_GRPC=ON
cmake --build build -j 8

# gRPCサーバー・クライアントの実行（別々のターミナルで）
./build/grpc_server
./build/grpc_client
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

# ログ出力（quill）
./build/quill_example

# ログ出力（spdlog）
./build/spdlog_example

# YAML設定ファイル読み込み
./build/yaml_example

# ZeroMQメッセージング（別々のターミナルで実行）
./build/zmq_server
./build/zmq_client

# ロギングベンチマーク（quill vs spdlog）
./build/benches/bench_logger

# テスト実行
./build/tests/test_sub

# --- 科学技術計算 ---

# 線形代数（Eigen）
./build/eigen_example

# Eigen行列乱数生成（EigenRand）
./build/eigenrand_example

# N次元配列（xtensor）
./build/xtensor_example

# MATLAB風線形代数（Armadillo）
./build/armadillo_example

# 非線形最小二乗最適化（Ceres Solver）
./build/ceres_example

# 非線形最適化（NLopt）
./build/nlopt_example

# SIMD演算（xsimd）
./build/xsimd_example

# ポータブルSIMD（Highway）
./build/highway_example

# 性能ポータビリティ並列化（Kokkos）
./build/kokkos_example

# 高速FFT（pocketfft）
./build/pocketfft_example

# プロット・可視化（Matplot++ - PNG出力）
./build/matplot_example

# Excel XLSX 読み書き（OpenXLSX）
./build/openxlsx_example

# シリアライゼーション（cereal - JSON/バイナリ）
./build/cereal_example

# HDF5 科学データ読み書き
./build/hdf5_example
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

## ドキュメント

- [`docs/libraries-existing.md`](docs/libraries-existing.md): 汎用ユーティリティライブラリの機能まとめ
- [`docs/libraries-scientific.md`](docs/libraries-scientific.md): 科学技術計算ライブラリの機能まとめ

## ディレクトリ構成

```markdown
- `src/`: 各ライブラリのサンプルコード
    - `sub/`: サブモジュールサンプル
- `include/`: ヘッダーファイル
    - `option_parsing/`: CLI11サブコマンド定義
    - `json_wrapper/`: yyjsonラッパー
- `benches/`: ベンチマークコード
- `tests/`: テストコード
- `docs/`: ライブラリ機能ドキュメント
- `proto/`: gRPC Protobuf定義ファイル
- `cmake/`: CMake設定ファイル
    - `local-or-fetch.cmake`: FetchContentヘルパー
    - `dependencies-app.cmake`: アプリ用ライブラリ
    - `dependencies-test.cmake`: テスト用ライブラリ
    - `custom-targets.cmake`: カスタムターゲット
    - `quality-setup.cmake`: コード品質設定
    - `quality-tools.cmake`: コード品質ツール
    - `collect-fetchcontent-licenses.cmake`: ライセンス収集
- `config_example/`: 設定ファイルサンプル（JSON/TOML/YAML）
- `third_party/`: サードパーティライブラリ（ローカルキャッシュ）
```
