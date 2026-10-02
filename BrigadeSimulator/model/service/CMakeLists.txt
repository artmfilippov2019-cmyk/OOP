file(GLOB SOURCES *.cpp *.hpp)

find_package(yaml-cpp REQUIRED)

add_library(service STATIC ${SOURCES})

target_link_libraries(service PUBLIC repository entity yaml-cpp)