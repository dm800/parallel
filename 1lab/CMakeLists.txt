cmake_minimum_required(VERSION 4.0)
project(Parallel)

set(CMAKE_CXX_STANDARD 23)

set(SOURCEFILES
        main.cpp
        include/Matrix/matrix.h
        include/TimeCheck/timer.h
        include/Matrix/defaultmult.h
        include/Matrix/parallelmult.h
        include/Generate/randomgen.h
)


include_directories(include)


file(GLOB SOURCES ${SOURCEFILES})
add_executable(Parallel ${SOURCES})
