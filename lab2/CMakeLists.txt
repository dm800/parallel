cmake_minimum_required(VERSION 3.20)

project(lab2 LANGUAGES CXX)

add_executable(lab2)
target_sources(lab2 PRIVATE
        main.cpp
)

target_include_directories(lab2 PRIVATE
        ${CMAKE_SOURCE_DIR}/lab1/include
)

target_compile_features(lab2 PRIVATE cxx_std_23)
