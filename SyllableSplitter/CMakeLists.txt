cmake_minimum_required(VERSION 3.22.1)

project(SepBySyllable)

set(CMAKE_CXX_STANDARD 20)

add_library(dialog dialog.hpp dialog.cpp)
add_library(prog prog.hpp prog.cpp)

target_link_libraries(dialog prog)
link_libraries(dialog prog)

add_executable(lab1 main.cpp)