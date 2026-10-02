file(GLOB SOURCES *.cpp *.hpp)

add_library(view STATIC ${SOURCES}
        BrigadeView.cpp
        BrigadeView.hpp
)

target_link_libraries(view PUBLIC
        entity
        repository
)