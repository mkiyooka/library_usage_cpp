# 科学技術計算ライブラリ

新規に導入した科学技術計算向けライブラリの機能まとめです。

---

## Eigen — 線形代数

- **バージョン**: 3.4.0
- **ライセンス**: MPL-2.0（修正ファイルの開示義務あり）
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/eigen_example.cpp`

### 主な機能

| カテゴリ       | 内容                                                |
| -------------- | --------------------------------------------------- |
| 行列・ベクトル | `Matrix2d`, `Vector3d`, `MatrixXd`（動的サイズ）    |
| 基本演算       | 加算・乗算・転置・行列式・トレース                  |
| 連立方程式     | `.lu().solve(b)`, `.colPivHouseholderQr().solve(b)` |
| 固有値分解     | `EigenSolver`, `SelfAdjointEigenSolver`             |
| SVD            | `JacobiSVD`, `BDCSVD`                               |
| Cholesky       | `LLT`, `LDLT`                                       |
| スパース行列   | `SparseMatrix<double>` + 各種ソルバー               |
| ジオメトリ     | `AngleAxisd`, `Quaterniond`, `Transform`            |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE Eigen3::Eigen)
```

### 使い方の例

```cpp
#include <Eigen/Dense>
Eigen::Matrix3d A;
A << 1, 2, 3, 4, 5, 6, 7, 8, 10;
Eigen::Vector3d x = A.lu().solve(b);
```

---

## EigenRand — Eigen 行列用乱数生成

- **バージョン**: 0.5.0
- **ライセンス**: MIT
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/eigenrand_example.cpp`

### 主な機能

| 分布           | 関数                                                          |
| -------------- | ------------------------------------------------------------- |
| 正規分布       | `normalLike(o, urng, mean, std)`                              |
| 一様分布       | `uniformRealLike(o, urng)`, `uniformIntLike(o, urng, lo, hi)` |
| ベルヌーイ分布 | `bernoulli(o, urng, p)`                                       |
| 指数分布       | `exponentialLike(o, urng, lambda)`                            |
| 多変量正規分布 | `makeMvNormalGen(mean, cov).generate(rows, urng)`             |

Eigen の行列・ベクトル・配列と同じシェイプで直接乱数を生成できる。`std::mt19937` 互換に加えて `Vmt19937_64`（SIMD 最適化版）が利用可能。

### CMake リンク

```cmake
# FetchContent_Populate のみ（CMakeLists.txt がベンチマークを無条件追加するため）
target_include_directories(target PRIVATE ${EigenRand_SOURCE_DIR})
target_link_libraries(target PRIVATE Eigen3::Eigen)
```

### 使い方の例

```cpp
#include <Eigen/Dense>
#include <EigenRand/EigenRand>

Eigen::Rand::Vmt19937_64 urng{42};
Eigen::MatrixXf m(3, 3);
m = Eigen::Rand::normalLike(m, urng, 0.0f, 1.0f);
```

---

## xtensor — N次元配列（NumPy 互換）

- **バージョン**: 0.25.0
- **ライセンス**: BSD-3-Clause
- **ヘッダーオンリー**: ✅（C++17 以上）
- **サンプル**: `src/xtensor_example.cpp`

### 主な機能

| カテゴリ         | 内容                                                            |
| ---------------- | --------------------------------------------------------------- |
| 配列             | `xarray`（動的）, `xtensor`（固定ランク）, `xfixed`（固定形状） |
| 遅延評価         | 式テンプレートによりコピーなしで計算                            |
| ブロードキャスト | NumPy と同じセマンティクス                                      |
| スライシング     | `xt::view(a, 0, xt::all())` など                                |
| 数学関数         | `xt::sin`, `xt::exp`, `xt::sqrt`, `xt::sum`, `xt::mean`         |
| 生成             | `xt::arange`, `xt::linspace`, `xt::zeros`, `xt::ones`           |
| I/O              | `xt::load_npy`, `xt::dump_npy`（NumPy 形式）                    |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE xtensor)
```

### 使い方の例

```cpp
#include <xtensor/xarray.hpp>
#include <xtensor/xmath.hpp>
xt::xarray<double> a = {{1., 2.}, {3., 4.}};
auto result = xt::sum(a * 2.0);
```

---

## Armadillo — MATLAB 風線形代数

- **バージョン**: 15.2.5
- **ライセンス**: Apache-2.0
- **ヘッダーオンリー**: ✅（`ARMA_DONT_USE_WRAPPER` 使用時は BLAS/LAPACK リンクが必要）
- **サンプル**: `src/armadillo_example.cpp`

### 主な機能

| カテゴリ    | 内容                                     |
| ----------- | ---------------------------------------- |
| 行列        | `mat`, `vec`, `cx_mat`（複素数）         |
| 基本演算    | `det`, `trace`, `inv`, `pinv`, `norm`    |
| 連立方程式  | `arma::solve(A, b)`                      |
| 固有値      | `eig_gen`（一般）, `eig_sym`（対称）     |
| SVD         | `svd`, `svd_econ`                        |
| 統計        | `mean`, `stddev`, `var`, `median`, `cov` |
| ランダム    | `randu`, `randn`（乱数行列）             |
| LAPACK 連携 | OpenBLAS/MKL/Accelerate 自動検出         |

### macOS での利用（Accelerate フレームワーク）

```cmake
target_link_libraries(target PRIVATE "-framework Accelerate")
```

### 使い方の例

```cpp
#define ARMA_DONT_USE_WRAPPER
#include <armadillo>
arma::mat A = {{2, 1}, {1, 3}};
arma::vec x = arma::solve(A, arma::vec{1, 2});
```

---

## Ceres Solver — 非線形最小二乗最適化

- **バージョン**: 2.2.0
- **ライセンス**: BSD-3-Clause
- **サンプル**: `src/ceres_example.cpp`

### 主な機能

| カテゴリ     | 内容                                                  |
| ------------ | ----------------------------------------------------- |
| 自動微分     | `AutoDiffCostFunction` — テンプレートで導関数自動計算 |
| 数値微分     | `NumericDiffCostFunction`                             |
| 解析微分     | `SizedCostFunction`                                   |
| 最適化手法   | Levenberg-Marquardt, Dogleg, BFGS, L-BFGS             |
| 線形ソルバー | DENSE_QR, SPARSE_SCHUR, ITERATIVE_SCHUR               |
| 損失関数     | `HuberLoss`, `CauchyLoss`（ロバスト推定）             |
| 用途         | SLAM, 3D 再構成, カーブフィッティング, バンドル調整   |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE Ceres::ceres)
```

### 使い方の例

```cpp
#include <ceres/ceres.h>
struct CostFunctor {
    template <typename T>
    bool operator()(const T* const x, T* residual) const {
        residual[0] = x[0] - T(3.0);
        return true;
    }
};
ceres::Problem problem;
problem.AddResidualBlock(
    new ceres::AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor),
    nullptr, &x);
ceres::Solve(options, &problem, &summary);
```

---

## NLopt — 非線形最適化

- **バージョン**: 2.10.1
- **ライセンス**: LGPL/MIT（アルゴリズム依存）
- **サンプル**: `src/nlopt_example.cpp`

### 主な機能

| カテゴリ     | 内容                                                      |
| ------------ | --------------------------------------------------------- |
| アルゴリズム | COBYLA, BOBYQA, NEWUOA, MMA, SLSQP, L-BFGS, CRS2 ほか 40+ |
| 制約         | 不等式・等式制約                                          |
| 勾配         | 勾配あり（LD\_\*）・なし（LN\_\*）の両対応                |
| 停止条件     | 絶対・相対許容誤差、最大評価回数、最大時間                |

### アルゴリズム命名規則

- `LD_` : 局所最適化・勾配あり
- `LN_` : 局所最適化・勾配なし
- `GD_` : 大域最適化・勾配あり
- `GN_` : 大域最適化・勾配なし

### CMake リンク

```cmake
target_link_libraries(target PRIVATE nlopt)
```

### 使い方の例

```cpp
#include <nlopt.hpp>
nlopt::opt opt(nlopt::LD_LBFGS, 2);
opt.set_min_objective(my_func, nullptr);
opt.set_xtol_rel(1e-8);
std::vector<double> x = {0.0, 0.0};
double fmin;
opt.optimize(x, fmin);
```

---

## xsimd — SIMD 抽象化ライブラリ

- **バージョン**: 13.2.0
- **ライセンス**: BSD-3-Clause
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/xsimd_example.cpp`

### 主な機能

| カテゴリ           | 内容                                         |
| ------------------ | -------------------------------------------- |
| 対応アーキテクチャ | SSE2〜SSE4.2, AVX, AVX2, AVX-512, NEON, SVE  |
| データ型           | `float`, `double`, `int32_t`, `int64_t` など |
| 演算               | 加減乗除・FMA・比較・min/max・ビット演算     |
| 数学関数           | `sin`, `cos`, `exp`, `log`, `sqrt` など      |
| ロード・ストア     | `load_aligned`, `load_unaligned`, `store_*`  |
| ブロードキャスト   | スカラーから SIMD レジスタへの展開           |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE xsimd)
```

### 使い方の例

```cpp
#include <xsimd/xsimd.hpp>
using batch_t = xsimd::batch<double>;
auto va = batch_t::load_aligned(a_ptr);
auto result = xsimd::sin(va) + va;
result.store_aligned(out_ptr);
```

---

## Highway — ポータブル SIMD

- **バージョン**: 1.2.0
- **ライセンス**: Apache-2.0
- **ヘッダーオンリー**: ✅（一部コンパイルあり）
- **サンプル**: `src/highway_example.cpp`

### 主な機能

| カテゴリ           | 内容                                                             |
| ------------------ | ---------------------------------------------------------------- |
| 対応アーキテクチャ | x86 (SSE4〜AVX-512), ARM (NEON〜SVE2), RISC-V (RVV)              |
| 動的ディスパッチ   | `HWY_EXPORT` / `HWY_DYNAMIC_DISPATCH` で実行時最適ターゲット選択 |
| 演算               | `LoadU`, `StoreU`, `Add`, `Mul`, `MulAdd`, `ReduceSum` など      |
| コード構造         | `HWY_BEFORE_NAMESPACE` / `HWY_AFTER_NAMESPACE` で分離            |
| 用途               | 画像処理、暗号、機械学習推論など                                 |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE hwy)
```

---

## Kokkos — 性能ポータビリティフレームワーク

- **バージョン**: 4.5.01
- **ライセンス**: BSD-3-Clause
- **サンプル**: `src/kokkos_example.cpp`

### 主な機能

| カテゴリ         | 内容                                               |
| ---------------- | -------------------------------------------------- |
| 実行空間         | Serial, OpenMP, CUDA, HIP, SYCL                    |
| 並列化           | `parallel_for`, `parallel_reduce`, `parallel_scan` |
| メモリ           | `View`（多次元配列）, `DualView`（CPU/GPU 共有）   |
| アトミック       | `atomic_add`, `atomic_compare_exchange`            |
| スケジューリング | `TeamPolicy`, `MDRangePolicy`                      |
| 用途             | HPC、数値シミュレーション、GPU 計算                |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE Kokkos::kokkos)
```

### 使い方の例

```cpp
#include <Kokkos_Core.hpp>
Kokkos::initialize(argc, argv);
{
    Kokkos::View<double*> a("a", N);
    Kokkos::parallel_for("init", N, KOKKOS_LAMBDA(int i) { a(i) = i; });
    double sum = 0;
    Kokkos::parallel_reduce("sum", N,
        KOKKOS_LAMBDA(int i, double& s) { s += a(i); }, sum);
}
Kokkos::finalize();
```

---

## pocketfft — 高速 FFT

- **バージョン**: master（NumPy/SciPy 同梱版）
- **ライセンス**: BSD-3-Clause
- **ヘッダーオンリー**: ✅（`pocketfft_hdronly.h` 1ファイル）
- **サンプル**: `src/pocketfft_example.cpp`

### 主な機能

| カテゴリ       | 内容                                                    |
| -------------- | ------------------------------------------------------- |
| 変換種類       | `r2c`（実数→複素）, `c2r`（逆変換）, `c2c`（複素→複素） |
| DCT/DST        | `dct`, `dst`（複数タイプ）                              |
| 次元           | 任意次元（1D, 2D, 3D, …）                               |
| 精度           | `float`, `double`, `long double`                        |
| マルチスレッド | スレッド数指定可能                                      |
| NumPy 互換     | NumPy の内部実装そのもの                                |

### 使い方の例

```cpp
#include <pocketfft_hdronly.h>
pocketfft::r2c({N}, {sizeof(double)}, {sizeof(std::complex<double>)},
               {0}, pocketfft::FORWARD, in.data(), out.data(), 1.0);
```

---

## Matplot++ — C++ プロットライブラリ

- **バージョン**: 1.2.1
- **ライセンス**: MIT
- **サンプル**: `src/matplot_example.cpp`

### 主な機能

| カテゴリ     | 内容                                          |
| ------------ | --------------------------------------------- |
| 基本プロット | `plot`, `scatter`, `bar`, `stem`              |
| 統計         | `hist`, `boxplot`, `violin`                   |
| 3D           | `surf`, `mesh`, `contour`, `scatter3`         |
| 装飾         | `xlabel`, `ylabel`, `title`, `legend`, `grid` |
| エクスポート | PNG, PDF, SVG, EPS (`save`)                   |
| サブプロット | `subplot`, `tiledlayout`                      |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE matplot)
```

### 使い方の例

```cpp
#include <matplot/matplot.h>
auto x = matplot::linspace(0, 2 * matplot::pi, 100);
matplot::plot(x, matplot::transform(x, [](double v){ return std::sin(v); }));
matplot::save("plot.png");
```

---

## OpenXLSX — Excel XLSX 読み書き

- **バージョン**: master (v0.3.2+)
- **ライセンス**: BSD-3-Clause
- **サンプル**: `src/openxlsx_example.cpp`

### 主な機能

| カテゴリ     | 内容                                                  |
| ------------ | ----------------------------------------------------- |
| 読み取り     | セル値（文字列・数値・bool）, 行・列イテレーション    |
| 書き込み     | セル値設定, フォントスタイル, 数式                    |
| シート操作   | シート追加・削除・名前変更・コピー                    |
| ワークブック | 新規作成 (`create`) および既存ファイルを開く (`open`) |
| セルアドレス | `"A1"` 形式と `(row, col)` 形式の両対応               |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE OpenXLSX::OpenXLSX)
```

### 使い方の例

```cpp
#include <OpenXLSX.hpp>
OpenXLSX::XLDocument doc;
doc.create("output.xlsx");
auto wks = doc.workbook().worksheet("Sheet1");
wks.cell("A1").value() = "Hello";
wks.cell(2, 1).value() = 42.0;
doc.save();
```

---

## cereal — シリアライゼーション

- **バージョン**: 1.3.2
- **ライセンス**: BSD-3-Clause
- **ヘッダーオンリー**: ✅
- **サンプル**: `src/cereal_example.cpp`

### 主な機能

| カテゴリ         | 内容                                                           |
| ---------------- | -------------------------------------------------------------- |
| アーカイブ形式   | `BinaryOutputArchive`, `JSONOutputArchive`, `XMLOutputArchive` |
| 対応型           | 標準コンテナ（`vector`, `map`, `string` など）                 |
| カスタム型       | `serialize` テンプレートメソッドで定義                         |
| NVP              | `CEREAL_NVP(x)` で名前付き値ペア                               |
| ポリモーフィズム | `CEREAL_REGISTER_TYPE` で基底クラス経由のシリアライズ          |

### 使い方の例

```cpp
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
struct Data {
    std::string name;
    std::vector<double> values;
    template<class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(name), CEREAL_NVP(values));
    }
};
// 書き込み
cereal::JSONOutputArchive ar(ofs);
ar(data);
```

---

## HDF5 C++ — 科学技術データフォーマット

- **バージョン**: 1.14.6
- **ライセンス**: BSD-3-Clause（HDF5 ライセンス）
- **サンプル**: `src/hdf5_example.cpp`

### 主な機能

| カテゴリ     | 内容                                                   |
| ------------ | ------------------------------------------------------ |
| データセット | 任意次元・型の配列の読み書き                           |
| グループ     | ファイルシステム風の階層構造                           |
| 属性         | データセット・グループへのメタデータ付与               |
| 圧縮         | gzip/szip/lz4 圧縮対応（要オプション有効化）           |
| データ型     | ネイティブ型・複合型・可変長文字列                     |
| 大規模対応   | ペタバイト級、並列 HDF5（MPI 対応）                    |
| 用途         | 数値シミュレーション、観測データ、機械学習データセット |

### CMake リンク

```cmake
target_link_libraries(target PRIVATE hdf5_cpp-static hdf5-static)
```

### 使い方の例

```cpp
#include <H5Cpp.h>
H5::H5File file("data.h5", H5F_ACC_TRUNC);
hsize_t dims[1] = {1000};
H5::DataSpace dspace(1, dims);
H5::DataSet dset = file.createDataSet("array", H5::PredType::NATIVE_DOUBLE, dspace);
dset.write(data.data(), H5::PredType::NATIVE_DOUBLE);
```
