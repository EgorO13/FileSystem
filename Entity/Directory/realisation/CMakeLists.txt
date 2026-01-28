add_library(DirectoryLib STATIC
        directory_descriptor.cpp
        directory_descriptor.h
)

target_include_directories(DirectoryLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/FSObject/realisation
        ${CMAKE_SOURCE_DIR}/Entity/User
        ${CMAKE_SOURCE_DIR}/Entity/Directory/interface
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(DirectoryLib PUBLIC
        DirectoryInterface
        FSObjectLib
        UserLib
)