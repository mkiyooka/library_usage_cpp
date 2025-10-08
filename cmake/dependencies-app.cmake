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
add_external_package(fmt third_party/fmt-11.2.0
    URL https://github.com/fmtlib/fmt/archive/refs/tags/11.2.0.tar.gz
    URL_HASH SHA256=bc23066d87ab3168f27cef3e97d545fa63314f5c79df5ea444d41d56f962c6af
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
add_external_package(yyjson third_party/yyjson-0.10.0
    URL https://github.com/ibireme/yyjson/archive/refs/tags/0.10.0.tar.gz
    URL_HASH SHA256=0d901cb2c45c5586e3f3a4245e58c2252d6b24bf4b402723f6179523d389b165
)
FetchContent_MakeAvailable(yyjson)

# indicators - Progress bar library
add_external_package(indicators third_party/indicators-2.3
    URL https://github.com/p-ranav/indicators/archive/refs/tags/v2.3.tar.gz
    URL_HASH SHA256=70da7a693ff7a6a283850ab6d62acf628eea17d386488af8918576d0760aef7b
)
FetchContent_MakeAvailable(indicators)

# quill - Logging library
add_external_package(quill third_party/quill-10.0.1
    URL https://github.com/odygrd/quill/archive/refs/tags/v10.0.1.tar.gz
    URL_HASH SHA256=ec7c0291d8b6533fc6e01d8e6694c5fcc3c803109b7397197c2c7ebd0107129f
)
FetchContent_MakeAvailable(quill)

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
