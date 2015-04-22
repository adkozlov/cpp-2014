cmake_minimum_required(VERSION 3.1)
project(sem_control_2_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.cpp)
add_executable(sem_control_2_2 ${SOURCE_FILES} serializer.h)