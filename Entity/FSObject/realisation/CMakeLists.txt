add_library(FSObjectLib STATIC
        fs_object.cpp
        fs_object.h
)

target_include_directories(FSObjectLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/User
        ${CMAKE_SOURCE_DIR}/Entity/ACL
        ${CMAKE_SOURCE_DIR}/Entity/FSObject/interface
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(FSObjectLib PUBLIC
        FSObjectInterface
        UserLib
        ACLLib
)