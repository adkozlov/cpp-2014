cmake_minimum_required(VERSION 2.8.4)
project(sem_control_2_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(SOURCE_FILES src/main.cpp)
add_executable(sem_control_2_1 ${SOURCE_FILES})