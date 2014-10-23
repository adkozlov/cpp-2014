cmake_minimum_required(VERSION 2.8.4)
project(homework_02)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp lint.cpp vector_t.cpp)
add_executable(homework_02 ${SOURCE_FILES})