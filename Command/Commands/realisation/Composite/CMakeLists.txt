add_library(CompositeCommandObjects STATIC
        composite_command.cpp
        composite_command.h
)

target_include_directories(CompositeCommandObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Base
)

target_link_libraries(CompositeCommandObjects PUBLIC
        BaseCommandObjects
)

set_target_properties(CompositeCommandObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)