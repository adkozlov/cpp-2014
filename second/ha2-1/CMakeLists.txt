cmake_minimum_required(VERSION 2.8.4)
project(ha2_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(SOURCE_FILES test/main.cpp)
add_executable(ha2_1 ${SOURCE_FILES})