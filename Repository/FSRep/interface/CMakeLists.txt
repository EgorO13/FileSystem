add_library(FSRepInterface INTERFACE)
target_include_directories(FSRepInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}
)

target_sources(FSRepInterface INTERFACE i_fs_repository.h)