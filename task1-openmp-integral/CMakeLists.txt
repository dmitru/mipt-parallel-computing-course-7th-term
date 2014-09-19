cmake_minimum_required(VERSION 2.8.4)
project(Task1)

set(OpenMP_C_FLAGS -fopenmp)
set(OpenMP_CXX_FLAGS -fopenmp)

include_directories(.)

include_directories(/usr/local/include/c++/4.9.0/)
include_directories(/usr/local/include/c++/4.9.0/x86_64-apple-darwin13.1.0/)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

set(SOURCE_FILES omp-integral.cpp)
add_executable(omp-integral ${SOURCE_FILES})
