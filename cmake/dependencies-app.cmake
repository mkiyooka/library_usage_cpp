# メインアプリケーション用のサードパーティライブラリ定義
#
# 使用例:
#   declare_fetchcontent_with_local(fmt third_party/fmt-11.2.0
#       GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#       GIT_TAG 11.2.0
#   )
#   FetchContent_MakeAvailable(fmt)

# CLI11 - Command line parser
declare_fetchcontent_with_local(CLI11 third_party/CLI11-2.5.0
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(CLI11)

# fmt - Formatting library
declare_fetchcontent_with_local(fmt third_party/fmt-11.2.0
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.2.0
)
FetchContent_MakeAvailable(fmt)

# tomlplusplus - TOML configuration library
declare_fetchcontent_with_local(tomlplusplus third_party/tomlplusplus-3.4.0
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

# nlohmann_json - JSON library
declare_fetchcontent_with_local(nlohmann_json third_party/nlohmann_json-3.12.0
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# yyjson - Fast JSON library
declare_fetchcontent_with_local(yyjson third_party/yyjson-0.10.0
    GIT_REPOSITORY https://github.com/ibireme/yyjson.git
    GIT_TAG 0.10.0
)
FetchContent_MakeAvailable(yyjson)

# indicators - Progress bar library
declare_fetchcontent_with_local(indicators third_party/indicators-2.3
    GIT_REPOSITORY https://github.com/p-ranav/indicators.git
    GIT_TAG v2.3
)
FetchContent_MakeAvailable(indicators)

# quill - Logging library
declare_fetchcontent_with_local(quill third_party/quill-10.0.1
    GIT_REPOSITORY https://github.com/odygrd/quill.git
    GIT_TAG v10.0.1
)
FetchContent_MakeAvailable(quill)

# nanobench - Benchmarking library
declare_fetchcontent_with_local(nanobench third_party/nanobench-4.3.11
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.3.11
)
FetchContent_MakeAvailable(nanobench)

# libzmq (ZeroMQ core library)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)
declare_fetchcontent_with_local(libzmq third_party/libzmq-4.3.5
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    GIT_TAG v4.3.5
)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE) # ビルド時間短縮のためテスト無効化
set(BUILD_SHARED OFF CACHE BOOL "" FORCE) # 静的ライブラリとしてビルド
set(BUILD_STATIC ON CACHE BOOL "" FORCE) # 静的ライブラリとしてビルド
set(WITH_LIBSODIUM OFF CACHE BOOL "" FORCE) # libsodiumは使用しない
set(ENABLE_CPACK OFF CACHE BOOL "" FORCE) # CPackは使用しない
FetchContent_MakeAvailable(libzmq)

# cppzmq (ZeroMQ C++ binding)
declare_fetchcontent_with_local(cppzmq third_party/cppzmq-4.11.0
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    GIT_TAG v4.11.0
)
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE) # ビルド時間短縮のためテスト無効化
FetchContent_MakeAvailable(cppzmq)

# ZeroMQ用の変数設定
set(ZMQ_INCLUDE_DIRS
    ${CMAKE_BINARY_DIR}/_deps/libzmq-src/include
    ${CMAKE_BINARY_DIR}/_deps/cppzmq-src
)
set(ZMQ_LIBRARIES libzmq-static)
