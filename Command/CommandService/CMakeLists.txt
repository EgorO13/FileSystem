add_subdirectory(interface)
add_subdirectory(realisation)

add_library(CommandServiceLib INTERFACE)

target_link_libraries(CommandServiceLib INTERFACE
        CommandServiceInterface
        CommandServiceRealisationObjects
)

target_include_directories(CommandServiceLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/CommandRep/interface
)