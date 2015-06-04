cmake_minimum_required(VERSION 3.2)
project(sem_control_2_4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(sem_control_2_4 ${SOURCE_FILES} my.h)