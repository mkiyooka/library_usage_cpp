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

# Serialization benchmark: cereal vs msgpack vs Cap'n Proto vs FlatBuffers
set(CAPNP_OUT_DIR ${CMAKE_BINARY_DIR}/src/capnp_gen)
set(FBS_OUT_DIR ${CMAKE_BINARY_DIR}/src/fbs_gen)

add_executable(bench_serialization bench_serialization.cpp
    ${CAPNP_OUT_DIR}/person.capnp.c++
)
target_include_directories(bench_serialization PRIVATE
    ${CAPNP_OUT_DIR}
    ${FBS_OUT_DIR}
)
target_link_libraries(bench_serialization PRIVATE
    celero
    cereal::cereal
    msgpack-cxx
    CapnProto::capnp
    flatbuffers
)
set_target_properties(bench_serialization PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benches
)
