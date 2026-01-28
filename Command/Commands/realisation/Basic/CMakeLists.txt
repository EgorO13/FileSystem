add_library(BasicCommandsObjects STATIC
        basic_commands.cpp
        basic_commands.h
)

target_include_directories(BasicCommandsObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Base
)

target_link_libraries(BasicCommandsObjects PUBLIC
        BaseCommandObjects
)

set_target_properties(BasicCommandsObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)