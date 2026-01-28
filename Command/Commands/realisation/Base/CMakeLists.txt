add_library(BaseCommandObjects STATIC
        base_command.cpp
        base_command.h
)

target_include_directories(BaseCommandObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
)

set_target_properties(BaseCommandObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)