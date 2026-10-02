file(GLOB SOURCES *.cpp *.hpp)

add_library(mapper STATIC ${SOURCES})

target_link_libraries(mapper PUBLIC dto entity)