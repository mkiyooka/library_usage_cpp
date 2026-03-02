# Logger benchmark: quill vs spdlog
add_executable(bench_logger bench_logger.cpp)
target_link_libraries(bench_logger PRIVATE
    quill::quill
    spdlog::spdlog
    nanobench::nanobench
)

set_target_properties(bench_logger PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benches
)
