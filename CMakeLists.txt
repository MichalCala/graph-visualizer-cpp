
cmake_minimum_required(VERSION 3.20)

project(GraphVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Biblioteka SFML
find_package(SFML 3 REQUIRED COMPONENTS Graphics)

#Program konsolowy
add_executable(graph_visualizer src/main.cpp src/Graph.cpp)

target_include_directories(
    graph_visualizer
    PRIVATE
    include
)

add_executable(
    graph_visualizer_gui
    src/gui_main.cpp
    src/Graph.cpp
)

target_include_directories(
    graph_visualizer_gui
    PRIVATE
    include
)

target_link_libraries(
    graph_visualizer_gui
    PRIVATE
    SFML::Graphics
)