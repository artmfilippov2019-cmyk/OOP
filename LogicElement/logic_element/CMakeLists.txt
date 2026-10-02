add_library(logic_element STATIC logic_element.hpp logic_element.cpp)

target_link_libraries(logic_element PUBLIC terminal)

target_include_directories(logic_element PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})