# Test executable
add_executable(test_sub
    test_sub.cpp
)

target_include_directories(test_sub PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_sub PRIVATE sub_lib doctest::doctest)

# Register the test with CTest
add_test(
    NAME test_sub
    COMMAND $<TARGET_FILE:test_sub>
)
