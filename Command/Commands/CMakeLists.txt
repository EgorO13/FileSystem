add_subdirectory(interface)
add_subdirectory(realisation)

add_library(CommandsLib INTERFACE)

target_link_libraries(CommandsLib INTERFACE
        CommandsInterface
        CommandsRealisationObjects
)

target_include_directories(CommandsLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/FileSystem
)