add_subdirectory(interface)
add_subdirectory(realisation)

add_library(CommandRepLib INTERFACE)

target_link_libraries(CommandRepLib INTERFACE
        CommandRepInterface
        CommandRepRealisationObjects
)

target_include_directories(CommandRepLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
)