cmake_minimum_required(VERSION 3.15)
project(graph_lib)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_FLAGS "-ldl ${CMAKE_CXX_FLAGS}")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(graph_lib main.cpp)

target_link_libraries(graph_lib sfml-graphics sfml-audio)