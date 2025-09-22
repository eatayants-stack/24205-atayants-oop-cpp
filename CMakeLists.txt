cmake_minimum_required(VERSION 4.0)
project(24205_atayants_oop_cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(24205_atayants_oop_cpp
        main.cpp
        Normalizer.cpp
        Normalizer.h
        Parser.cpp
        Parser.h
        CSV-Loader.cpp
        CSV-Loader.h
        TotalWords.cpp
        TotalWords.h
        MergeWordCount.cpp
        MergeWordCount.h
)
