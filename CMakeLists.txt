cmake_minimum_required(VERSION 3.20)
project(Parallel LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lab1)
add_subdirectory(lab2)

add_custom_target(run_all
        DEPENDS lab1 lab2
        COMMAND "$<TARGET_FILE:lab1>"
        COMMAND "$<TARGET_FILE:lab2>"
        COMMAND "${CMAKE_COMMAND}" -E chdir "$<TARGET_FILE_DIR:lab1>" "$<TARGET_FILE:lab1>"
        COMMAND "${CMAKE_COMMAND}" -E chdir "$<TARGET_FILE_DIR:lab2>" "$<TARGET_FILE:lab2>"
        USES_TERMINAL
        COMMENT "Running lab1 then lab2..."
)