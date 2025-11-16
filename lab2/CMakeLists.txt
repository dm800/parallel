cmake_minimum_required(VERSION 3.20)

project(lab2 LANGUAGES CXX)

# 1) Находим MPI (даёт импортируемую цель MPI::MPI_CXX и переменные MPIEXEC_*)
find_package(MPI REQUIRED)

# 2) Собираем *бинарник* с кодом
add_executable(lab2_bin
        main.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Matrix/Matrix.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Matrix/defaultmult.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Matrix/parallelmult.cpp
        ${CMAKE_SOURCE_DIR}/lab1/include/Generate/randomgen.cpp
)

target_include_directories(lab2_bin PRIVATE
        ${CMAKE_SOURCE_DIR}/lab1/include
        ${CMAKE_SOURCE_DIR}/lab2/include
)

target_link_libraries(lab2_bin PRIVATE MPI::MPI_CXX)
target_compile_features(lab2_bin PRIVATE cxx_std_23)


set(MPI_PROCS 4 CACHE STRING "How many MPI processes to run")
set(APP_ARGS "" CACHE STRING "Arguments to pass to the app")


add_custom_target(lab2
        COMMAND ${MPIEXEC_EXECUTABLE}
        ${MPIEXEC_NUMPROC_FLAG} ${MPI_PROCS}
        ${MPIEXEC_PREFLAGS}
        $<TARGET_FILE:lab2_bin> ${APP_ARGS}
        ${MPIEXEC_POSTFLAGS}
        DEPENDS lab2_bin
        USES_TERMINAL
        COMMENT "Running lab2 with ${MPI_PROCS} MPI processes..."
)
