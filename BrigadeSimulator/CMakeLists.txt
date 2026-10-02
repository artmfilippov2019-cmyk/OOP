cmake_minimum_required(VERSION 3.22.1)
project(lab3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(dto)
add_subdirectory(HashTable)
add_subdirectory(mapper)
add_subdirectory(model)
add_subdirectory(presenter)
add_subdirectory(view)
add_subdirectory(loader)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    include_directories(tests)
    add_subdirectory(tests)
endif()

add_executable(main main.cpp
)

target_link_libraries(main PRIVATE
        dto
        HashTable
        mapper
        model
        presenter
        view
        loader
)