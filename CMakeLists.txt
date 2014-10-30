cmake_minimum_required(VERSION 2.8.4)
project(GTPcppconv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
    test.cpp
    Tools.cpp)

add_executable(GTPcppconv ${SOURCE_FILES})
