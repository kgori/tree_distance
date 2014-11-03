cmake_minimum_required(VERSION 2.8.4)
project(GTPcppconv)
set(MACOSX_RPATH ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(SOURCE_FILES
    BipartiteGraph.cpp
    Bipartition.cpp
    EdgeAttribute.cpp
    Geodesic.cpp
    PhyloTree.cpp
    PhyloTreeEdge.cpp
    Distance.cpp
    Ratio.cpp
    RatioSequence.cpp
    Tools.cpp)

add_executable(GTPcppconv ${SOURCE_FILES} test.cpp)
add_executable(timer ${SOURCE_FILES} main.cpp)
