add_library(CommandRepRealisationObjects STATIC
        command_repository.cpp
        command_repository.h
)

target_include_directories(CommandRepRealisationObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Base
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Composite
        ${CMAKE_SOURCE_DIR}/Command/CommandRep/interface
)

set_target_properties(CommandRepRealisationObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)