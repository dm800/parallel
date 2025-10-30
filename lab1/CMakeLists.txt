cmake_minimum_required(VERSION 3.20)

project(lab1 LANGUAGES CXX)

add_executable(lab1)
target_sources(lab1 PRIVATE
        main.cpp
        include/Matrix/matrix.h
        include/Matrix/defaultmult.h
        include/Matrix/parallelmult.h
        include/Generate/randomgen.h
)

target_include_directories(lab1 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(lab1 PRIVATE cxx_std_23)
