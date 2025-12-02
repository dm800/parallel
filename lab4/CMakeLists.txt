cmake_minimum_required(VERSION 3.20)
project(lab4 LANGUAGES CXX)

add_executable(lab4
        main.cpp
        include/Fork.cpp
        include/Wiseman.cpp
        include/Manager.cpp
        include/Manager.h
        include/Fork.h
        include/Wiseman.h
)

target_include_directories(lab4 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(lab4 PRIVATE cxx_std_23)
