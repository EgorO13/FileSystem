add_library(FileSystemRealisation STATIC
        file_system.cpp
        file_system.h
)

target_include_directories(FileSystemRealisation PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/FileSystem/interface
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/Repository
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Loader
)

target_link_libraries(FileSystemRealisation PUBLIC
        FileSystemInterface
        EntityLib
        RepositoryLib
        ServiceLib
        LoaderLib
)