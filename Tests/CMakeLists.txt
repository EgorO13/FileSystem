cmake_minimum_required(VERSION 3.15)

add_library(TestObjects STATIC
        EntityTest/test_file.cpp
        EntityTest/test_dir.cpp
        RepoTest/test_fs_repo.cpp
        RepoTest/test_user_repo.cpp
        RepoTest/test_group_repo.cpp
        RepoTest/test_command_repo.cpp
        ServiceTest/test_sec_service.cpp
        ServiceTest/test_session_service.cpp
        ServiceTest/test_user_management_service.cpp
        ServiceTest/test_fs_service.cpp
        TableTest/test_table.cpp
)

set_target_properties(TestObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(TestObjects PUBLIC
        ..
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Repository
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Command
        ${CMAKE_SOURCE_DIR}/Table
        ${CMAKE_SOURCE_DIR}/View
        ${CMAKE_SOURCE_DIR}/Controller
        ${CMAKE_SOURCE_DIR}/Loader
)

target_link_libraries(TestObjects PUBLIC
        RepositoryLib
        EntityLib
        ServiceLib
        CommandLib
        FileSystemLib
        ViewLib
        TableLib
        ControllerLib
        LoaderLib
        yaml-cpp
)