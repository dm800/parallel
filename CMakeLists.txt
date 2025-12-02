cmake_minimum_required(VERSION 3.20)
project(Parallel LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

# Параметры запуска (можно менять через CMake Cache / CLion)
set(MPI_PROCS 1 CACHE STRING "How many MPI processes to run")
set(APP_ARGS "" CACHE STRING "Arguments to pass to lab2")

# Подкаталоги
add_subdirectory(lab1)  # ДОЛЖЕН собирать исполняемый таргет 'lab1'
add_subdirectory(lab2)  # ДОЛЖЕН собирать 'lab2_bin' и (опционально) custom target 'lab2'
add_subdirectory(lab4)

# Единая цель, которая ПОСЛЕДОВАТЕЛЬНО запускает lab1 и lab2_bin через mpiexec
add_custom_target(run_all
        DEPENDS lab1 lab2_bin                  # сначала собрать оба бинарника
        COMMAND "$<TARGET_FILE:lab1>"          # затем запустить lab1
        COMMAND ${MPIEXEC_EXECUTABLE}          # и запустить lab2 через MPI
        ${MPIEXEC_NUMPROC_FLAG} ${MPI_PROCS}
        ${MPIEXEC_PREFLAGS}
        "$<TARGET_FILE:lab2_bin>" ${APP_ARGS}
        ${MPIEXEC_POSTFLAGS}
        USES_TERMINAL
        COMMENT "Running lab1, then lab2 via MPI with ${MPI_PROCS} processes..."
)
