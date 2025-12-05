cmake_minimum_required(VERSION 3.14)
project(BitArrayProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

message(STATUS "Fetching GoogleTest...")
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_library(bitarray_lib
        src/BitArray.cpp
        src/BitArray.h
)

target_include_directories(bitarray_lib PUBLIC src)

add_executable(run_tests
        test/BitArrayTest.cpp
)

target_include_directories(run_tests PRIVATE src)

target_link_libraries(run_tests
        PRIVATE
        bitarray_lib
        GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(run_tests)

