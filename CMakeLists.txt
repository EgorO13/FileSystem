cmake_minimum_required(VERSION 3.15)
project(lab3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(yaml-cpp REQUIRED)
find_package(Threads REQUIRED)
find_package(Catch2 3 REQUIRED)

add_subdirectory(Entity)
add_subdirectory(Repository)
add_subdirectory(Service)
add_subdirectory(Table)
add_subdirectory(Threads)
add_subdirectory(FileSystem)
add_subdirectory(View)
add_subdirectory(Command)
add_subdirectory(Controller)
add_subdirectory(Loader)

add_executable(tests
        Tests/EntityTest/test_acl.cpp
        Tests/EntityTest/test_file.cpp
        Tests/EntityTest/test_dir.cpp
        Tests/EntityTest/test_group.cpp
        Tests/EntityTest/test_user.cpp
        Tests/RepoTest/test_command_repo.cpp
        Tests/RepoTest/test_fs_repo.cpp
        Tests/RepoTest/test_user_repo.cpp
        Tests/RepoTest/test_group_repo.cpp
        Tests/ServiceTest/test_command_service.cpp
        Tests/ServiceTest/test_sec_service.cpp
        Tests/ServiceTest/test_session_service.cpp
        Tests/ServiceTest/test_user_management_service.cpp
        Tests/ServiceTest/test_fs_service.cpp
        Tests/CommandTest/test_base_command.cpp
        Tests/CommandTest/test_composite_commands.cpp
        Tests/TableTest/test_table.cpp
)

target_link_libraries(tests PRIVATE
        TableLib
        EntityLib
        RepositoryLib
        ServiceLib
        FileSystemLib
        CommandLib
        ViewLib
        ThreadLib
        ControllerLib
        LoaderLib
        yaml-cpp
        Catch2::Catch2WithMain
)

add_test(NAME UnitTests COMMAND tests)

add_executable(lab3 main.cpp)

target_link_libraries(lab3 PRIVATE
        LoaderLib
        ControllerLib
        ViewLib
        ThreadLib
        FileSystemLib
        CommandLib
        ServiceLib
        RepositoryLib
        EntityLib
        TableLib
        yaml-cpp
)