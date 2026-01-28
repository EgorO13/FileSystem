add_library(LoaderLib STATIC
        loader.cpp
        loader.h
)

target_include_directories(LoaderLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Loader/interface
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Repository
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/Command
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/View
        ${CMAKE_SOURCE_DIR}/Controller
)

target_link_libraries(LoaderLib PUBLIC
        LoaderInterface
        EntityLib
        RepositoryLib
        ServiceLib
        CommandLib
        FileSystemLib
        ViewLib
        ControllerLib
)

set_target_properties(LoaderLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)