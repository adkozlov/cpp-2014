cmake_minimum_required(VERSION 3.1)
project(ha2_3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11")

set(SOURCE_FILES test/test.cpp)
add_executable(ha2_3 ${SOURCE_FILES})