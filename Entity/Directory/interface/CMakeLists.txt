add_library(DirectoryInterface INTERFACE)

target_include_directories(DirectoryInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/FSObject
        ${CMAKE_SOURCE_DIR}
)

target_sources(DirectoryInterface INTERFACE i_directory.h)