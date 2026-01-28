add_subdirectory(CommandMapper)
add_subdirectory(StateService)

add_library(CommandStateServiceLib INTERFACE)

target_link_libraries(CommandStateServiceLib INTERFACE
        CommandMapperLib
        CommandStateServiceObjects
)

target_include_directories(CommandStateServiceLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Composite
)