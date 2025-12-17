cmake_minimum_required(VERSION 3.20)
project(lab5sync LANGUAGES CXX)

add_executable(lab5sync
        main.cpp
)

target_compile_features(lab5sync PRIVATE cxx_std_23)
