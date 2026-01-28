add_library(CommandServiceInterface INTERFACE)

target_include_directories(CommandServiceInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/CommandRep/interface
)

target_sources(CommandServiceInterface INTERFACE i_command_service.h)