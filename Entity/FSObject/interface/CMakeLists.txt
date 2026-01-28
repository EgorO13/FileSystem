add_library(FSObjectInterface INTERFACE)

target_include_directories(FSObjectInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/User
        ${CMAKE_SOURCE_DIR}/Entity/ACL
        ${CMAKE_SOURCE_DIR}
)

target_sources(FSObjectInterface INTERFACE i_fs_object.h)