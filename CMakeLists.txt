cmake_minimum_required(VERSION 4.0)
project(24205_atayants_oop_cpp)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

set(GTEST_INCLUDE_DIR "C:/msys64/home/user/googletest-main/build/lib")
include_directories(${GTEST_INCLUDE_DIR})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_library(wordstats STATIC
        ${SRC_DIR}/Parser.cpp
        ${SRC_DIR}/CsvWriter.cpp
        ${SRC_DIR}/FileReader.cpp
        ${SRC_DIR}/StatisticsCollector.cpp
        ${SRC_DIR}/WordCounter.cpp
)
target_include_directories(wordstats PUBLIC ${SRC_DIR})

add_executable(24205_atayants_oop_cpp
        ${SRC_DIR}/main.cpp
)
target_link_libraries(24205_atayants_oop_cpp PRIVATE wordstats)

add_executable(ParserTest
        ${TEST_DIR}/ParserTest.cpp
)
target_link_libraries(ParserTest PRIVATE wordstats gtest gtest_main)

add_executable(StatisticsCollectorTest
        ${TEST_DIR}/StatisticsCollectorTest.cpp
)
target_link_libraries(StatisticsCollectorTest PRIVATE wordstats gtest gtest_main)

target_include_directories(ParserTest PRIVATE ${gtest_SOURCE_DIR}/include)
target_include_directories(StatisticsCollectorTest PRIVATE ${gtest_SOURCE_DIR}/include)
