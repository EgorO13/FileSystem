add_library(FSServiceInterface INTERFACE)
target_include_directories(FSServiceInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(FSServiceInterface INTERFACE i_fs_service.h)