find_package(Catch2 2 REQUIRED)

add_executable(logic_element_testing testing.cpp)

target_link_libraries(logic_element_testing PRIVATE terminal logic_element Catch2::Catch2)

target_include_directories(logic_element_testing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})