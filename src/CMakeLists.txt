# Add subdirectories
add_subdirectory(sub)

add_executable(cmd main.cpp)
target_link_libraries(cmd PRIVATE sub_lib)

# TOML loader example
add_executable(toml_loader toml_loader.cpp)
target_include_directories(toml_loader PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(toml_loader PRIVATE tomlplusplus::tomlplusplus)

# FMT example
add_executable(fmt_example fmt_example.cpp)
target_include_directories(fmt_example PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(fmt_example PRIVATE fmt::fmt)

# JSON loader example
add_executable(json_loader json_loader.cpp)
target_include_directories(json_loader PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(json_loader PRIVATE nlohmann_json::nlohmann_json)

# JSON writer example
add_executable(json_writer json_writer.cpp)
target_link_libraries(json_writer PRIVATE nlohmann_json::nlohmann_json)

# Quill logging example
add_executable(quill_example quill_example.cpp)
target_link_libraries(quill_example PRIVATE quill::quill)

# Indicators progress bar example
add_executable(indicator_example indicator_example.cpp)
target_link_libraries(indicator_example PRIVATE indicators::indicators)

# JSON benchmark with multiple libraries
add_executable(json_benchmark json_benchmark.cpp)
target_include_directories(json_benchmark PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(json_benchmark PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
    yyjson
    nanobench::nanobench
)

# JSON function test with multiple libraries
add_executable(json_function_test json_function_test.cpp)
target_include_directories(json_function_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(json_function_test PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
    yyjson
)

# CLI11 example with subcommands
add_executable(cli11_example cli11_example.cpp cli11_commands.cpp)
target_include_directories(cli11_example PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cli11_example PRIVATE CLI11::CLI11)

# ZeroMQ examples
add_executable(zmq_server zmq_server.cpp)
target_include_directories(zmq_server PRIVATE ${ZMQ_INCLUDE_DIRS})
target_link_libraries(zmq_server PRIVATE ${ZMQ_LIBRARIES})

add_executable(zmq_client zmq_client.cpp)
target_include_directories(zmq_client PRIVATE ${ZMQ_INCLUDE_DIRS})
target_link_libraries(zmq_client PRIVATE ${ZMQ_LIBRARIES})

# gRPC examples (pixiで導入したgRPCを使用)
# pkg-configを使ってgRPCとprotobufを見つける
find_package(PkgConfig REQUIRED)
pkg_check_modules(GRPCPP REQUIRED grpc++)
pkg_check_modules(PROTOBUF REQUIRED protobuf)

# protoファイルからC++ソースを生成
get_filename_component(greeter_proto "${CMAKE_SOURCE_DIR}/proto/greeter.proto" ABSOLUTE)
get_filename_component(greeter_proto_path "${greeter_proto}" PATH)

# 生成されるファイルのパス
set(greeter_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/greeter.pb.cc")
set(greeter_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/greeter.pb.h")
set(greeter_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/greeter.grpc.pb.cc")
set(greeter_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/greeter.grpc.pb.h")

# protocとgrpc_cpp_pluginの実行ファイルを探す
find_program(PROTOC_EXECUTABLE protoc REQUIRED)
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin REQUIRED)

# protobufとgRPCのコード生成コマンド
add_custom_command(
    OUTPUT "${greeter_proto_srcs}" "${greeter_proto_hdrs}" "${greeter_grpc_srcs}" "${greeter_grpc_hdrs}"
    COMMAND ${PROTOC_EXECUTABLE}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
         --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
         -I "${greeter_proto_path}"
         --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
         "${greeter_proto}"
    DEPENDS "${greeter_proto}"
    COMMENT "Generating gRPC code from greeter.proto"
)

# 生成されたprotoファイルをライブラリ化
add_library(greeter_grpc_proto
    ${greeter_proto_srcs}
    ${greeter_proto_hdrs}
    ${greeter_grpc_srcs}
    ${greeter_grpc_hdrs}
)
target_include_directories(greeter_grpc_proto PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GRPCPP_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
)
target_link_libraries(greeter_grpc_proto
    ${GRPCPP_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)

# gRPCサーバー
add_executable(grpc_server grpc_server.cpp)
target_include_directories(grpc_server PRIVATE
    ${GRPCPP_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
)
target_link_directories(grpc_server PRIVATE
    ${GRPCPP_LIBRARY_DIRS}
    ${PROTOBUF_LIBRARY_DIRS}
)
target_link_libraries(grpc_server PRIVATE
    greeter_grpc_proto
    ${GRPCPP_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)

# gRPCクライアント
add_executable(grpc_client grpc_client.cpp)
target_include_directories(grpc_client PRIVATE
    ${GRPCPP_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
)
target_link_directories(grpc_client PRIVATE
    ${GRPCPP_LIBRARY_DIRS}
    ${PROTOBUF_LIBRARY_DIRS}
)
target_link_libraries(grpc_client PRIVATE
    greeter_grpc_proto
    ${GRPCPP_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)

# プリコンパイル済みヘッダーの設定
if(USE_PCH)
    target_precompile_headers(cmd PRIVATE
        <memory>
        <string>
    )
    message(STATUS "Precompiled headers enabled for executables")
endif()
