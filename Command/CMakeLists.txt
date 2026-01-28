add_subdirectory(CommandRep)
add_subdirectory(Commands)
add_subdirectory(CommandService)
add_subdirectory(CommandStateService)

add_library(CommandLib INTERFACE)

target_link_libraries(CommandLib INTERFACE
        CommandRepLib
        CommandsLib
        CommandServiceLib
        CommandStateServiceLib
)

target_include_directories(CommandLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/FileSystem
)