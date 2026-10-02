cmake_minimum_required(VERSION 3.22.1)

project(LogicElement)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(terminal)

add_subdirectory(logic_element)

add_subdirectory(tests)

add_executable(main main.cpp dialog.cpp)

target_link_libraries(main PRIVATE terminal logic_element)