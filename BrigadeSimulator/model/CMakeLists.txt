add_subdirectory(entity)
add_subdirectory(repository)
add_subdirectory(service)

add_library(model INTERFACE)

target_link_libraries(model INTERFACE
        entity
        repository
        service
)