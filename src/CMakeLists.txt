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
    yyjson::yyjson
    nanobench::nanobench
)

# JSON function test with multiple libraries
add_executable(json_function_test json_function_test.cpp)
target_include_directories(json_function_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(json_function_test PRIVATE
    fmt::fmt
    nlohmann_json::nlohmann_json
    yyjson::yyjson
)

# CLI11 example with subcommands
add_executable(cli11_example cli11_example.cpp cli11_commands.cpp)
target_include_directories(cli11_example PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cli11_example PRIVATE CLI11::CLI11)

# ZeroMQ examples
add_executable(zmq_server zmq_server.cpp)
target_include_directories(zmq_server PRIVATE ${ZMQ_INCLUDE_DIR})
target_link_libraries(zmq_server PRIVATE ${ZMQ_LIBRARY})

add_executable(zmq_client zmq_client.cpp)
target_include_directories(zmq_client PRIVATE ${ZMQ_INCLUDE_DIR})
target_link_libraries(zmq_client PRIVATE ${ZMQ_LIBRARY})

# プリコンパイル済みヘッダーの設定
if(USE_PCH)
    target_precompile_headers(cmd PRIVATE
        <memory>
        <string>
    )
    message(STATUS "Precompiled headers enabled for executables")
endif()
