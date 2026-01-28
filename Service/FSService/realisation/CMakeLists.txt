add_library(FSServiceLib STATIC
        fs_service.cpp
        fs_service.h
)

target_include_directories(FSServiceLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Service/FSService/interface
)

target_link_libraries(FSServiceLib PUBLIC
        FSServiceInterface
        FileLib
        DirectoryLib
        FSObjectLib
        UserLib
        ACLLib
        TableLib
)