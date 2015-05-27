cmake_minimum_required(VERSION 3.1)
project(sem_control_2_3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(sem_control_2_3 ${SOURCE_FILES} src/au_allocator.h)