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

# Spdlog logging example
add_executable(spdlog_example spdlog_example.cpp)
target_link_libraries(spdlog_example PRIVATE spdlog::spdlog)

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

# YAML example
add_executable(yaml_example yaml_example.cpp)
target_link_libraries(yaml_example PRIVATE yaml-cpp::yaml-cpp)

# ZeroMQ examples
add_executable(zmq_server zmq_server.cpp)
target_include_directories(zmq_server PRIVATE ${ZMQ_INCLUDE_DIRS})
target_link_libraries(zmq_server PRIVATE ${ZMQ_LIBRARIES})

add_executable(zmq_client zmq_client.cpp)
target_include_directories(zmq_client PRIVATE ${ZMQ_INCLUDE_DIRS})
target_link_libraries(zmq_client PRIVATE ${ZMQ_LIBRARIES})

# gRPC examples (ENABLE_GRPC=ONのときのみビルド)
if(ENABLE_GRPC)
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
endif() # ENABLE_GRPC

# Eigen example
add_executable(eigen_example eigen_example.cpp)
target_link_libraries(eigen_example PRIVATE Eigen3::Eigen)

# EigenRand example (header-only, depends on Eigen)
add_executable(eigenrand_example eigenrand_example.cpp)
target_include_directories(eigenrand_example PRIVATE ${EigenRand_SOURCE_DIR})
target_link_libraries(eigenrand_example PRIVATE Eigen3::Eigen)

# xtensor example
add_executable(xtensor_example xtensor_example.cpp)
target_link_libraries(xtensor_example PRIVATE xtensor)

# Armadillo example (header-only mode with Accelerate on macOS)
add_executable(armadillo_example armadillo_example.cpp)
target_include_directories(armadillo_example PRIVATE
    ${armadillo_SOURCE_DIR}/include
)
if(APPLE)
    target_link_libraries(armadillo_example PRIVATE "-framework Accelerate")
else()
    find_package(BLAS QUIET)
    find_package(LAPACK QUIET)
    if(BLAS_FOUND AND LAPACK_FOUND)
        target_link_libraries(armadillo_example PRIVATE ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
    endif()
endif()

# xsimd example
add_executable(xsimd_example xsimd_example.cpp)
target_link_libraries(xsimd_example PRIVATE xsimd)

# Highway example
add_executable(highway_example highway_example.cpp)
target_link_libraries(highway_example PRIVATE hwy)

# Kokkos example
add_executable(kokkos_example kokkos_example.cpp)
target_link_libraries(kokkos_example PRIVATE Kokkos::kokkos)

# pocketfft example (header-only)
add_executable(pocketfft_example pocketfft_example.cpp)
target_include_directories(pocketfft_example PRIVATE ${pocketfft_SOURCE_DIR})

# NLopt example
add_executable(nlopt_example nlopt_example.cpp)
target_link_libraries(nlopt_example PRIVATE nlopt)

# Ceres example
add_executable(ceres_example ceres_example.cpp)
target_link_libraries(ceres_example PRIVATE Ceres::ceres)

# OpenXLSX example
add_executable(openxlsx_example openxlsx_example.cpp)
target_link_libraries(openxlsx_example PRIVATE OpenXLSX::OpenXLSX)

# cereal example (header-only)
add_executable(cereal_example cereal_example.cpp)
target_link_libraries(cereal_example PRIVATE cereal::cereal)

# HDF5 C++ example
add_executable(hdf5_example hdf5_example.cpp)
target_link_libraries(hdf5_example PRIVATE hdf5_cpp-static hdf5-static)
target_include_directories(hdf5_example PRIVATE
    ${hdf5_BINARY_DIR}
    ${hdf5_SOURCE_DIR}/src
    ${hdf5_SOURCE_DIR}/c++/src
)

# Matplot++ example
add_executable(matplot_example matplot_example.cpp)
target_link_libraries(matplot_example PRIVATE matplot)

# プリコンパイル済みヘッダーの設定
if(USE_PCH)
    target_precompile_headers(cmd PRIVATE
        <memory>
        <string>
    )
    message(STATUS "Precompiled headers enabled for executables")
endif()
