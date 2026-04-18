# doctest
add_executable(test_sub test_sub.cpp)
target_include_directories(test_sub PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_sub PRIVATE sub_lib doctest::doctest)
set_target_properties(test_sub PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
add_test(NAME test_sub COMMAND $<TARGET_FILE:test_sub>)

# Catch2
add_executable(test_catch2 test_catch2.cpp)
target_include_directories(test_catch2 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_catch2 PRIVATE sub_lib Catch2::Catch2WithMain)
set_target_properties(test_catch2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
add_test(NAME test_catch2 COMMAND $<TARGET_FILE:test_catch2>)

# GoogleTest
add_executable(test_gtest test_gtest.cpp)
target_include_directories(test_gtest PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_gtest PRIVATE sub_lib GTest::gtest_main)
set_target_properties(test_gtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
add_test(NAME test_gtest COMMAND $<TARGET_FILE:test_gtest>)

# RapidCheck (GoogleTest integration)
add_executable(test_rapidcheck test_rapidcheck.cpp)
target_include_directories(test_rapidcheck PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_rapidcheck PRIVATE sub_lib GTest::gtest_main rapidcheck rapidcheck_gtest)
set_target_properties(test_rapidcheck PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
add_test(NAME test_rapidcheck COMMAND $<TARGET_FILE:test_rapidcheck>)

# ApprovalTests (GoogleTest integration)
add_executable(test_approvals test_approvals.cpp)
target_include_directories(test_approvals PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_approvals PRIVATE GTest::gtest_main ApprovalTests)
set_target_properties(test_approvals PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
