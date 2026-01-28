add_library(FileInterface INTERFACE)

target_include_directories(FileInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/FSObject
        ${CMAKE_SOURCE_DIR}
)

target_sources(FileInterface INTERFACE i_file.h i_lockable.h)