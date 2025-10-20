cmake_minimum_required(VERSION 4.0)
project(24205_atayants_oop_cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(24205_atayants_oop_cpp
        main.cpp
        Parser.cpp
        Parser.h
        CsvWriter.cpp
        CsvWriter.h
        FileReader.cpp
        FileReader.h
        StatisticsCollector.cpp
        StatisticsCollector.h
        WordCounter.cpp
        WordCounter.h
)
