cmake_minimum_required(VERSION 2.8.4)
project(sem_control_4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/maybe_tests.cpp)

add_executable(sem_control_4 ${SOURCE_FILES})