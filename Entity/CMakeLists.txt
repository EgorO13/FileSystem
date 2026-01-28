add_subdirectory(ACL)
add_subdirectory(FSObject)
add_subdirectory(File)
add_subdirectory(Directory)
add_subdirectory(Group)
add_subdirectory(User)
add_subdirectory(Mapper)

add_library(EntityLib STATIC)

target_link_libraries(EntityLib PUBLIC
        ACLLib
        UserLib
        GroupLib
        FSObjectLib
        DirectoryLib
        FileLib
        MapperLib
)

target_include_directories(EntityLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/FSObject/interface
        ${CMAKE_SOURCE_DIR}/Entity/ACL
        ${CMAKE_SOURCE_DIR}/Entity/User
        ${CMAKE_SOURCE_DIR}
)