
cmake_minimum_required(VERSION 3.20)

project(GraphVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(graph_visualizer src/main.cpp src/Graph.cpp)

target_include_directories(
    graph_visualizer
    PRIVATE
    include
)