file(GLOB SOURCES *.cpp *.hpp)

add_library(presenter STATIC ${SOURCES})

target_link_libraries(presenter PUBLIC
        mapper
        service
        view
)