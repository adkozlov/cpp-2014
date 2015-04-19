cmake_minimum_required(VERSION 3.1)
project(ha2_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(SOURCE_FILES test/main.cpp)
add_executable(ha2_2 ${SOURCE_FILES})