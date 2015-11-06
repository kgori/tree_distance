cmake_minimum_required(VERSION 2.8.4)
project(cgtp)
set(MACOSX_RPATH ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

include_directories(src/include)
set(SOURCE_FILES
    src/BipartiteGraph.cpp
    src/Bipartition.cpp
    src/Geodesic.cpp
    src/PhyloTree.cpp
    src/PhyloTreeEdge.cpp
    src/Distance.cpp
    src/Ratio.cpp
    src/RatioSequence.cpp
    src/Tools.cpp)

add_executable(cgtp ${SOURCE_FILES} src/test.cpp)
add_executable(timer ${SOURCE_FILES} src/main.cpp)
