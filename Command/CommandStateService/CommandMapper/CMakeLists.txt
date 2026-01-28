add_library(CommandMapperLib STATIC
        command_mapper.h
        composite_command_mapper.cpp
        composite_command_mapper.h
)

target_include_directories(CommandMapperLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Composite
)

set_target_properties(CommandMapperLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)