cmake_minimum_required(VERSION 3.10)
project(24205_atayants_oop_cpp)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

# Configure googletest using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(wordstats STATIC
        ${SRC_DIR}/Parser.cpp
        ${SRC_DIR}/CsvWriter.cpp
        ${SRC_DIR}/FileReader.cpp
        ${SRC_DIR}/StatisticsCollector.cpp
        ${SRC_DIR}/WordStat.cpp
)
target_include_directories(wordstats PUBLIC ${SRC_DIR})

add_executable(24205_atayants_oop_cpp
        ${SRC_DIR}/main.cpp
)
target_link_libraries(24205_atayants_oop_cpp PRIVATE wordstats)

# Enable testing
enable_testing()

add_executable(ParserTest
        ${TEST_DIR}/ParserTest.cpp
)
target_link_libraries(ParserTest PRIVATE wordstats GTest::gtest GTest::gtest_main)
add_test(NAME ParserTest COMMAND ParserTest)

add_executable(StatisticsCollectorTest
        ${TEST_DIR}/StatisticsCollectorTest.cpp
)
target_link_libraries(StatisticsCollectorTest PRIVATE wordstats GTest::gtest GTest::gtest_main)
add_test(NAME StatisticsCollectorTest COMMAND StatisticsCollectorTest)
