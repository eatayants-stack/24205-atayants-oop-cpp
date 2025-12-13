cmake_minimum_required(VERSION 3.16)
project(LifeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows/MSVC shared CRT issues.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB LIFE_CORE_SOURCES
        src/Rule.cpp
        src/Universe.cpp
        src/Parser.cpp
        src/Engine.cpp
        src/ConsoleUI.cpp
        src/AppSupport.cpp
        src/Command.cpp
        src/*.h)

add_library(life_core ${LIFE_CORE_SOURCES})
target_include_directories(life_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(life_cli src/main.cpp)
target_link_libraries(life_cli PRIVATE life_core)

add_executable(life_tests 
    test/ParserTest.cpp
    test/RuleTest.cpp
    test/EngineTest.cpp
    test/UniverseTest.cpp
    test/CommandTest.cpp
)
target_link_libraries(life_tests PRIVATE life_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(life_tests)

