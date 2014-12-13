cmake_minimum_required(VERSION 2.8.4)
project(ha3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11")

set(SOURCE_FILES src/test.cpp src/any.cpp)
add_executable(ha3 ${SOURCE_FILES})