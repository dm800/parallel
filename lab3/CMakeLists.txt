cmake_minimum_required(VERSION 3.20)

project(lab3 LANGUAGES CXX)

add_executable(lab3_bin
        main.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Matrix/Matrix.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Matrix/defaultmult.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Matrix/parallelmult.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Generate/randomgen.cpp
)

target_include_directories(lab3_bin PRIVATE
        ${CMAKE_SOURCE_DIR}/lab1/include
        ${CMAKE_SOURCE_DIR}/lab3/include
)

target_compile_features(lab3_bin PRIVATE cxx_std_23)

# <<< ВАЖНОЕ МЕСТО: включаем OpenMP для MinGW (g++)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(lab3_bin PRIVATE -fopenmp)
    target_link_options(lab3_bin PRIVATE -fopenmp)
endif()
# >>>

# Цель для запуска
set(APP_ARGS "" CACHE STRING "Arguments to pass to the app")

add_custom_target(lab3
        COMMAND $<TARGET_FILE:lab3_bin> ${APP_ARGS}
        DEPENDS lab3_bin
        USES_TERMINAL
        COMMENT "Running lab3 (OpenMP)..."
)
