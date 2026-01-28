add_library(CommandServiceRealisationObjects STATIC
        command_service.cpp
        command_service.h
)

target_include_directories(CommandServiceRealisationObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Base
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Basic
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Composite
        ${CMAKE_SOURCE_DIR}/Command/CommandRep/interface
        ${CMAKE_SOURCE_DIR}/Command/CommandService/interface
)

set_target_properties(CommandServiceRealisationObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)