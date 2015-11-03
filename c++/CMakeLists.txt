cmake_minimum_required(VERSION 3.3)
project(pagerank)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp PageRank.cpp Node.cpp)
add_executable(pagerank ${SOURCE_FILES})