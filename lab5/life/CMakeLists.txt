cmake_minimum_required(VERSION 3.20)
project(lab5life LANGUAGES CXX)

add_executable(lab5life
        main.cpp
)

target_compile_features(lab5life PRIVATE cxx_std_23)
