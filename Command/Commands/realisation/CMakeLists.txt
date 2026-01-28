add_subdirectory(Base)
add_subdirectory(Basic)
add_subdirectory(Composite)

add_library(CommandsRealisationObjects INTERFACE)

target_link_libraries(CommandsRealisationObjects INTERFACE
        BaseCommandObjects
        BasicCommandsObjects
        CompositeCommandObjects
)

target_include_directories(CommandsRealisationObjects INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Base
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Basic
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Composite
)
