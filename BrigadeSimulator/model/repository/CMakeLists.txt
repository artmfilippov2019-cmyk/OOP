file(GLOB SOURCES *.cpp *.hpp)

add_library(repository STATIC ${SOURCES})

target_link_libraries(repository PUBLIC entity HashTable)
