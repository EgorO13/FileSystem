add_subdirectory(interface)
add_subdirectory(realisation)

add_library(FileSystemLib STATIC)

target_link_libraries(FileSystemLib PUBLIC
        FileSystemRealisation
        EntityLib
        RepositoryLib
        ServiceLib
        FileSystemInterface
)

target_include_directories(FileSystemLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/Repository
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/FileSystem/interface
)

set_target_properties(FileSystemLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)