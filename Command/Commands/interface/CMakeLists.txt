add_library(CommandsInterface INTERFACE)

target_include_directories(CommandsInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/FileSystem
)

target_sources(CommandsInterface INTERFACE
        i_command.h
        i_compositable.h
)