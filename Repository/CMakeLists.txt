add_subdirectory(FSRep)
add_subdirectory(GroupRep)
add_subdirectory(UserRep)

add_library(RepositoryLib STATIC)

target_link_libraries(RepositoryLib PUBLIC
        FSRepRealisationObjects
        GroupRepRealisationObjects
        UserRepRealisationObjects
)

target_include_directories(RepositoryLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Entity/ACL
        ${CMAKE_SOURCE_DIR}/Entity/User
        ${CMAKE_SOURCE_DIR}/Entity/Group
        ${CMAKE_SOURCE_DIR}/Entity/FSObject
        ${CMAKE_SOURCE_DIR}/Entity/Directory/interface
        ${CMAKE_SOURCE_DIR}/Entity/Directory/realisation
        ${CMAKE_SOURCE_DIR}/Entity/File/interface
        ${CMAKE_SOURCE_DIR}/Entity/File/realisation
)

target_include_directories(RepositoryLib PUBLIC
        ${CMAKE_SOURCE_DIR}/Repository/FSRep/interface
        ${CMAKE_SOURCE_DIR}/Repository/GroupRep/interface
        ${CMAKE_SOURCE_DIR}/Repository/UserRep/interface
        ${CMAKE_SOURCE_DIR}/Repository/FSRep/realisation/Path
)

set_target_properties(RepositoryLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

if(TARGET EntityLib)
    target_link_libraries(RepositoryLib PUBLIC EntityLib)
endif()