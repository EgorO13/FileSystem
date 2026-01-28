add_library(CommandRepInterface INTERFACE)

target_include_directories(CommandRepInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
)

target_sources(CommandRepInterface INTERFACE i_command_repository.h)