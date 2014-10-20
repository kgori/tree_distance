cmake_minimum_required(VERSION 2.8.4)
project(GTP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Bipartition.cpp
    EdgeAttribute.cpp
    PhyloTreeEdge.cpp
    test.cpp)

add_executable(GTP ${SOURCE_FILES})