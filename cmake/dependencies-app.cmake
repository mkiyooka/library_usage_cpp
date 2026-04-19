# メインアプリケーション用のサードパーティライブラリ定義
#
# 使用例:
#   add_external_package(fmt third_party/fmt-11.2.0
#       GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#       GIT_TAG 11.2.0
#   )
#   FetchContent_MakeAvailable(fmt)

# CLI11 - Command line parser
add_external_package(CLI11 third_party/CLI11-2.5.0
    URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v2.5.0.tar.gz
    URL_HASH SHA256=17e02b4cddc2fa348e5dbdbb582c59a3486fa2b2433e70a0c3bacb871334fd55
)
FetchContent_MakeAvailable(CLI11)

# fmt - Formatting library
add_external_package(fmt third_party/fmt-12.0.0
    URL https://github.com/fmtlib/fmt/archive/refs/tags/12.0.0.tar.gz
    URL_HASH SHA256=aa3e8fbb6a0066c03454434add1f1fc23299e85758ceec0d7d2d974431481e40
)
FetchContent_MakeAvailable(fmt)

# tomlplusplus - TOML configuration library
add_external_package(tomlplusplus third_party/tomlplusplus-3.4.0
    URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz
    URL_HASH SHA256=8517f65938a4faae9ccf8ebb36631a38c1cadfb5efa85d9a72e15b9e97d25155
)
FetchContent_MakeAvailable(tomlplusplus)

# nlohmann_json - JSON library
add_external_package(nlohmann_json third_party/nlohmann_json-3.12.0
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.tar.gz
    URL_HASH SHA256=4b92eb0c06d10683f7447ce9406cb97cd4b453be18d7279320f7b2f025c10187
)
FetchContent_MakeAvailable(nlohmann_json)

# yyjson - Fast JSON library
add_external_package(yyjson third_party/yyjson-0.12.0
    URL https://github.com/ibireme/yyjson/archive/refs/tags/0.12.0.tar.gz
    URL_HASH SHA256=b16246f617b2a136c78d73e5e2647c6f1de1313e46678062985bdcf1f40bb75d
)
FetchContent_MakeAvailable(yyjson)

# indicators - Progress bar library
add_external_package(indicators third_party/indicators-2.3
    URL https://github.com/p-ranav/indicators/archive/refs/tags/v2.3.tar.gz
    URL_HASH SHA256=70da7a693ff7a6a283850ab6d62acf628eea17d386488af8918576d0760aef7b
)
FetchContent_MakeAvailable(indicators)

# quill - Logging library
add_external_package(quill third_party/quill-11.0.2
    URL https://github.com/odygrd/quill/archive/refs/tags/v11.0.2.tar.gz
    URL_HASH SHA256=c4208f717e62fc4a7178917c9c39dbb90276d72c3cefd9077d0b973365d72667
)
FetchContent_MakeAvailable(quill)

# spdlog - Fast C++ logging library
# SPDLOG_FMT_EXTERNAL=ON: spdlog のバンドル fmt を使わず、
# 上記で導入済みの fmt::fmt を共有する（ODR違反・二重定義を防ぐ）
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
add_external_package(spdlog third_party/spdlog-1.17.0
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz
    URL_HASH SHA256=d8862955c6d74e5846b3f580b1605d2428b11d97a410d86e2fb13e857cd3a744
)
FetchContent_MakeAvailable(spdlog)

# yaml-cpp - YAML parser and emitter
add_external_package(yaml-cpp third_party/yaml-cpp-0.9.0
    URL https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.9.0.tar.gz
    URL_HASH SHA256=25cb043240f828a8c51beb830569634bc7ac603978e0f69d6b63558dadefd49a
)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(yaml-cpp)

# nanobench - Benchmarking library
add_external_package(nanobench third_party/nanobench-4.3.11
    URL https://github.com/martinus/nanobench/archive/refs/tags/v4.3.11.tar.gz
    URL_HASH SHA256=53a5a913fa695c23546661bf2cd22b299e10a3e994d9ed97daf89b5cada0da70
)
FetchContent_MakeAvailable(nanobench)

# libzmq (ZeroMQ core library)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)
add_external_package(libzmq third_party/libzmq-4.3.5
    URL https://github.com/zeromq/libzmq/archive/refs/tags/v4.3.5.tar.gz
    URL_HASH SHA256=6c972d1e6a91a0ecd79c3236f04cf0126f2f4dfbbad407d72b4606a7ba93f9c6
)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE) # ビルド時間短縮のためテスト無効化
set(BUILD_SHARED OFF CACHE BOOL "" FORCE) # 静的ライブラリとしてビルド
set(BUILD_STATIC ON CACHE BOOL "" FORCE) # 静的ライブラリとしてビルド
set(WITH_LIBSODIUM OFF CACHE BOOL "" FORCE) # libsodiumは使用しない
set(ENABLE_CPACK OFF CACHE BOOL "" FORCE) # CPackは使用しない
FetchContent_MakeAvailable(libzmq)

# cppzmq (ZeroMQ C++ binding)
add_external_package(cppzmq third_party/cppzmq-4.11.0
    URL https://github.com/zeromq/cppzmq/archive/refs/tags/v4.11.0.tar.gz
    URL_HASH SHA256=0fff4ff311a7c88fdb76fceefba0e180232d56984f577db371d505e4d4c91afd
)
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE) # ビルド時間短縮のためテスト無効化
FetchContent_MakeAvailable(cppzmq)

# ZeroMQ用の変数設定
set(ZMQ_INCLUDE_DIRS
    ${CMAKE_BINARY_DIR}/_deps/libzmq-src/include
    ${CMAKE_BINARY_DIR}/_deps/cppzmq-src
)
set(ZMQ_LIBRARIES libzmq-static)

# Eigen - Linear algebra library (header-only, MPL2)
FetchContent_Declare(Eigen3
    URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
    URL_HASH SHA256=8586084f71f9bde545ee7fa6d00288b264a2b7ac3607b974e54d13e7162c1c72
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(EIGEN_BUILD_PKGCONFIG OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Eigen3)

# EigenRand - Eigen-based random number generator (MIT, header-only)
# CMakeLists.txt がベンチマークを無条件ビルドするため EXCLUDE_FROM_ALL で回避しない
# → FetchContent_Populate のみ実行してヘッダーパスだけ取得する
FetchContent_Declare(EigenRand
    URL https://github.com/bab2min/EigenRand/archive/refs/tags/v0.5.0.tar.gz
    URL_HASH SHA256=159c13b7602884957f51d59d7484d62d9e82e7d7bc8bebbaf2880b72cf24e867
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_GetProperties(EigenRand)
if(NOT eigenrand_POPULATED)
    FetchContent_Populate(EigenRand)
endif()

# xtensor - N-dimensional arrays (header-only, BSD-3-Clause)
FetchContent_Declare(xtl
    URL https://github.com/xtensor-stack/xtl/archive/refs/tags/0.7.7.tar.gz
    URL_HASH SHA256=44fb99fbf5e56af5c43619fc8c29aa58e5fad18f3ba6e7d9c55c111b62df1fbb
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(xtl)

FetchContent_Declare(xtensor
    URL https://github.com/xtensor-stack/xtensor/archive/refs/tags/0.25.0.tar.gz
    URL_HASH SHA256=32d5d9fd23998c57e746c375a544edf544b74f0a18ad6bc3c38cbba968d5e6c7
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(xtensor)

# xsimd - SIMD intrinsics wrapper (header-only, BSD-3-Clause)
FetchContent_Declare(xsimd
    URL https://github.com/xtensor-stack/xsimd/archive/refs/tags/13.2.0.tar.gz
    URL_HASH SHA256=edd8cd3d548c185adc70321c53c36df41abe64c1fe2c67bc6d93c3ecda82447a
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(xsimd)

# Armadillo - Linear algebra (header-only mode, Apache-2.0)
FetchContent_Declare(armadillo
    URL https://sourceforge.net/projects/arma/files/armadillo-15.2.5.tar.xz/download
    URL_HASH SHA256=5b5f505acef9bfd8ca78b698378a206b12abe8981851b61ae8964fb92b100f36
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(ARMA_USE_WRAPPER OFF CACHE BOOL "" FORCE)
set(ARMA_DONT_USE_WRAPPER ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(armadillo)

# Highway - SIMD portability library (Apache-2.0)
FetchContent_Declare(highway
    URL https://github.com/google/highway/archive/refs/tags/1.2.0.tar.gz
    URL_HASH SHA256=7e0be78b8318e8bdbf6fa545d2ecb4c90f947df03f7aadc42c1967f019e63343
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(HWY_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(HWY_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(HWY_ENABLE_CONTRIB OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(highway)

# Kokkos - Performance portability (BSD-3-Clause)
FetchContent_Declare(Kokkos
    URL https://github.com/kokkos/kokkos/archive/refs/tags/4.5.01.tar.gz
    URL_HASH SHA256=e0008c89d7f03ebbe31eb3c89d40fe529b4e4072b40331ae6b9d6599f02dff44
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(Kokkos_ENABLE_SERIAL ON CACHE BOOL "" FORCE)
set(Kokkos_ENABLE_OPENMP OFF CACHE BOOL "" FORCE)
set(Kokkos_ENABLE_CUDA OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Kokkos)

# pocketfft - FFT library (BSD-3-Clause, header-only, no CMakeLists.txt)
FetchContent_Declare(pocketfft
    GIT_REPOSITORY https://github.com/mreineck/pocketfft.git
    GIT_TAG        cpp
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(pocketfft)

# NLopt - Nonlinear optimization (LGPL/MIT mix)
FetchContent_Declare(nlopt
    URL https://github.com/stevengj/nlopt/archive/refs/tags/v2.10.1.tar.gz
    URL_HASH SHA256=30d13ce16da119db3e987784f7864e35a562ec62c186352fae55cd003e6c58ff
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(NLOPT_CXX ON CACHE BOOL "" FORCE)
set(NLOPT_PYTHON OFF CACHE BOOL "" FORCE)
set(NLOPT_OCTAVE OFF CACHE BOOL "" FORCE)
set(NLOPT_MATLAB OFF CACHE BOOL "" FORCE)
set(NLOPT_GUILE OFF CACHE BOOL "" FORCE)
set(NLOPT_SWIG OFF CACHE BOOL "" FORCE)
set(NLOPT_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(nlopt)

# Ceres Solver - Nonlinear least squares (BSD-3-Clause)
# 依存: Eigen3 (上で導入済み), glog, gflags
find_package(glog QUIET)
if(NOT glog_FOUND)
    FetchContent_Declare(glog
        URL https://github.com/google/glog/archive/refs/tags/v0.7.1.tar.gz
        URL_HASH SHA256=00e4a87e87b7e7612f519a41e491f16623b12423620006f59f5688bfd8d13b08
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glog)
endif()
FetchContent_Declare(Ceres
    URL https://github.com/ceres-solver/ceres-solver/archive/refs/tags/2.2.0.tar.gz
    URL_HASH SHA256=12efacfadbfdc1bbfa203c236e96f4d3c210bed96994288b3ff0c8e7c6f350d4
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(MINIGLOG ON CACHE BOOL "" FORCE)
set(GFLAGS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(PROVIDE_UNINSTALL_TARGET OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Ceres)

# OpenXLSX - Excel XLSX read/write (BSD-3-Clause)
# masterブランチを使用: v0.3.2にはApple Clangのテンプレート解析エラーがある
FetchContent_Declare(OpenXLSX
    GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
set(OPENXLSX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OPENXLSX_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(OpenXLSX)

# cereal - Serialization library (BSD-3-Clause, header-only)
FetchContent_Declare(cereal
    URL https://github.com/USCiLab/cereal/archive/refs/tags/v1.3.2.tar.gz
    URL_HASH SHA256=16a7ad9b31ba5880dac55d62b5d6f243c3ebc8d46a3514149e56b5e7ea81f85f
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(JUST_INSTALL_CEREAL ON CACHE BOOL "" FORCE)
set(BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_SANDBOX OFF CACHE BOOL "" FORCE)
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cereal)

# HDF5 - Scientific data format (BSD-3-Clause)
set(HDF5_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(HDF5_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(HDF5_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(HDF5_ENABLE_Z_LIB_SUPPORT OFF CACHE BOOL "" FORCE)
set(HDF5_ENABLE_SZIP_SUPPORT OFF CACHE BOOL "" FORCE)
set(HDF5_BUILD_HL_LIB ON CACHE BOOL "" FORCE)
set(HDF5_BUILD_CPP_LIB ON CACHE BOOL "" FORCE)
FetchContent_Declare(HDF5
    URL https://github.com/HDFGroup/hdf5/archive/refs/tags/hdf5_1.14.6.tar.gz
    URL_HASH SHA256=09ee1c671a87401a5201c06106650f62badeea5a3b3941e9b1e2e1e08317357f
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(HDF5)

# Matplot++ - Plotting library (MIT)
FetchContent_Declare(matplotplusplus
    URL https://github.com/alandefreitas/matplotplusplus/archive/refs/tags/v1.2.1.tar.gz
    URL_HASH SHA256=9dd7cc92b2425148f50329f5a3bf95f9774ac807657838972d35334b5ff7cb87
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
set(MATPLOTPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MATPLOTPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(matplotplusplus)

# oneTBB - Threading Building Blocks (Apache-2.0)
set(TBB_TEST OFF CACHE BOOL "" FORCE)
set(TBB_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(TBB
    URL https://github.com/uxlfoundation/oneTBB/archive/refs/tags/v2022.3.0.tar.gz
    URL_HASH SHA256=01598a46c1162c27253a0de0236f520fd8ee8166e9ebb84a4243574f88e6e50a
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(TBB)

# Taskflow - Task parallelism (Apache-2.0 WITH LLVM-exception, header-only)
set(TF_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(TF_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(Taskflow
    URL https://github.com/taskflow/taskflow/archive/refs/tags/v3.9.0.tar.gz
    URL_HASH SHA256=d872a19843d12d437eba9b8664835b7537b92fe01fdb33ed92ca052d2483be2d
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(Taskflow)

# Zstandard - Fast lossless compression (BSD OR GPLv2 → BSD選択)
set(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(ZSTD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ZSTD_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ZSTD_BUILD_STATIC ON CACHE BOOL "" FORCE)
FetchContent_Declare(zstd
    URL https://github.com/facebook/zstd/archive/refs/tags/v1.5.7.tar.gz
    URL_HASH SHA256=37d7284556b20954e56e1ca85b80226768902e2edabd3b649e9e72c0c9012ee3
    SOURCE_SUBDIR build/cmake
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(zstd)

# CTRE - Compile-Time Regular Expressions (Apache-2.0 WITH LLVM-exception, header-only)
FetchContent_Declare(ctre
    URL https://github.com/hanickadot/compile-time-regular-expressions/archive/refs/tags/v3.10.0.tar.gz
    URL_HASH SHA256=b17e6c9a6cc0cea65132f62a6c699cefed952721063569d6339eb3ca471045e6
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(ctre)

# Abseil - Google C++ common libraries (Apache-2.0); required by RE2
set(ABSL_PROPAGATE_CXX_STD ON CACHE BOOL "" FORCE)
set(ABSL_BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(absl
    URL https://github.com/abseil/abseil-cpp/archive/refs/tags/20260107.1.tar.gz
    URL_HASH SHA256=4314e2a7cbac89cac25a2f2322870f343d81579756ceff7f431803c2c9090195
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(absl)

# RE2 - Fast regular expressions (BSD-3-Clause)
set(RE2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(RE2_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(re2
    URL https://github.com/google/re2/archive/refs/tags/2025-11-05.tar.gz
    URL_HASH SHA256=87f6029d2f6de8aa023654240a03ada90e876ce9a4676e258dd01ea4c26ffd67
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(re2)

# msgpack-c - MessagePack serialization (BSL-1.0, header-only C++ mode)
set(MSGPACK_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MSGPACK_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MSGPACK_USE_BOOST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(msgpack
    URL https://github.com/msgpack/msgpack-c/archive/refs/tags/cpp-7.0.0.tar.gz
    URL_HASH SHA256=070881ebea9208cf7e731fd5a46a11404025b2f260ab9527e32dfcb7c689fbfc
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(msgpack)

# Cap'n Proto - Serialization and RPC (MIT)
# GNU拡張が必要なため CMAKE_CXX_EXTENSIONS を一時的に ON にする
set(_saved_cxx_ext ${CMAKE_CXX_EXTENSIONS})
set(CMAKE_CXX_EXTENSIONS ON)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(CAPNP_LITE OFF CACHE BOOL "" FORCE)
FetchContent_Declare(capnproto
    URL https://github.com/capnproto/capnproto/archive/refs/tags/v1.1.0.tar.gz
    URL_HASH SHA256=c0a0d78a07e821f7bae26c7fcac20a9202eb3d639a673b2606b76092a1f35b6b
    SOURCE_SUBDIR c++
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(capnproto)
set(CMAKE_CXX_EXTENSIONS ${_saved_cxx_ext})

# Apache Arrow - Columnar memory format and analytics (Apache-2.0)
set(ARROW_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ARROW_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(ARROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ARROW_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(ARROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ARROW_BUILD_INTEGRATION OFF CACHE BOOL "" FORCE)
set(ARROW_USE_GLOG OFF CACHE BOOL "" FORCE)
set(ARROW_WITH_BROTLI OFF CACHE BOOL "" FORCE)
set(ARROW_WITH_BZ2 OFF CACHE BOOL "" FORCE)
set(ARROW_WITH_LZ4 OFF CACHE BOOL "" FORCE)
set(ARROW_WITH_SNAPPY OFF CACHE BOOL "" FORCE)
set(ARROW_WITH_ZLIB OFF CACHE BOOL "" FORCE)
set(ARROW_WITH_ZSTD OFF CACHE BOOL "" FORCE)
set(ARROW_FLIGHT OFF CACHE BOOL "" FORCE)
set(ARROW_PARQUET OFF CACHE BOOL "" FORCE)
set(ARROW_ORC OFF CACHE BOOL "" FORCE)
set(ARROW_DATASET OFF CACHE BOOL "" FORCE)
set(ARROW_JSON OFF CACHE BOOL "" FORCE)
set(ARROW_FILESYSTEM OFF CACHE BOOL "" FORCE)
set(ARROW_CSV OFF CACHE BOOL "" FORCE)
set(ARROW_HDFS OFF CACHE BOOL "" FORCE)
set(ARROW_COMPUTE OFF CACHE BOOL "" FORCE)
set(ARROW_IPC ON CACHE BOOL "" FORCE)
set(ARROW_SIMD_LEVEL "NONE" CACHE STRING "" FORCE)
set(ARROW_BUILD_UTILITIES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(Arrow
    URL https://github.com/apache/arrow/archive/refs/tags/apache-arrow-19.0.1.tar.gz
    URL_HASH SHA256=4c898504958841cc86b6f8710ecb2919f96b5e10fa8989ac10ac4fca8362d86a
    SOURCE_SUBDIR cpp
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(Arrow)
