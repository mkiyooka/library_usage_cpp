add_library(sub_lib
    STATIC
    sub.cpp
)
target_include_directories(sub_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
