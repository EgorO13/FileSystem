add_library(FileSystemInterface INTERFACE)
target_include_directories(FileSystemInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(FileSystemInterface INTERFACE i_file_system.h)