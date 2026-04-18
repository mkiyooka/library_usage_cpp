# テストフレームワーク用のサードパーティライブラリ定義

# doctest - Testing framework
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)
add_external_package(doctest ext/doctest-2.4.12
    URL https://github.com/doctest/doctest/archive/refs/tags/v2.4.12.tar.gz
    URL_HASH SHA256=73381c7aa4dee704bd935609668cf41880ea7f19fa0504a200e13b74999c2d70
)
FetchContent_MakeAvailable(doctest)

# Mark doctest as system library to exclude it from clang-tidy checks
if(TARGET doctest)
    get_target_property(doctest_include_dirs doctest INTERFACE_INCLUDE_DIRECTORIES)
    if(doctest_include_dirs)
        set_target_properties(doctest PROPERTIES
            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${doctest_include_dirs}"
        )
    endif()
endif()

# Catch2 - Testing framework
FetchContent_Declare(Catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.8.1.tar.gz
    URL_HASH SHA256=18b3f70ac80fccc340d8c6ff0f339b2ae64944782f8d2fca2bd705cf47cadb79
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(Catch2)

# GoogleTest - Testing framework
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.tar.gz
    URL_HASH SHA256=78c676fc63881529bf97bf9d45948d905a66833fbfa5318ea2cd7478cb98f399
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(googletest)

# RapidCheck - Property-based testing (Catch2/GTest integration included)
set(RC_ENABLE_CATCH2 ON CACHE BOOL "" FORCE)
set(RC_ENABLE_GTEST ON CACHE BOOL "" FORCE)
FetchContent_Declare(rapidcheck
    URL https://github.com/emil-e/rapidcheck/archive/refs/heads/master.tar.gz
    URL_HASH SHA256=5e2b2ed13c060e0bcd908e59dad751b75888a02646866b9be491e5a7b531fd63
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(rapidcheck)

# ApprovalTests.cpp - Approval testing (single-header)
FetchContent_Declare(ApprovalTests
    URL https://github.com/approvals/ApprovalTests.cpp/releases/download/v.10.13.0/ApprovalTests.v.10.13.0.hpp
    URL_HASH SHA256=c00f6390b81d9924dc646e9d32b61e1e09abda106c13704f714ac349241bb9ff
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_GetProperties(ApprovalTests)
if(NOT approvaltests_POPULATED)
    FetchContent_Populate(ApprovalTests)
endif()
# ダウンロードされたヘッダーのディレクトリを interface ライブラリとして公開
add_library(ApprovalTests INTERFACE)
target_include_directories(ApprovalTests INTERFACE ${approvaltests_SOURCE_DIR})
