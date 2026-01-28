add_library(ControllerLib STATIC
        controller.cpp
        controller.h
)

target_include_directories(ControllerLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Controller/interface
        ${CMAKE_SOURCE_DIR}/View
        ${CMAKE_SOURCE_DIR}/Command
        ${CMAKE_SOURCE_DIR}/FileSystem
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/Repository
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Loader
)

set_target_properties(ControllerLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(ControllerLib PUBLIC
        ControllerInterface
        ViewLib
        CommandLib
        FileSystemLib
        ServiceLib
        RepositoryLib
        EntityLib
        LoaderLib
)