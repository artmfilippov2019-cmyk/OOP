file(GLOB SOURCES *.cpp *.hpp)

add_library(loader STATIC ${SOURCES})

target_link_libraries(loader PUBLIC
        mapper
        repository
        service
        presenter
        view
)