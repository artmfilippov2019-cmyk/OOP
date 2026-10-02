cmake_minimum_required(VERSION 3.16)
project(tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_executable(tests
        TestEntity.cpp
        TestHashTable.cpp
        TestRepository.cpp
        TestService.cpp
)

target_link_libraries(tests PRIVATE
        entity
        repository
        HashTable
        service
        mapper
        GTest::gtest
        GTest::gtest_main
)
