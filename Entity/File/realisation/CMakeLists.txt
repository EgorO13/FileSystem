add_library(FileLib STATIC
        file_descriptor.cpp
        file_descriptor.h
)

target_include_directories(FileLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/FSObject/realisation
        ${CMAKE_SOURCE_DIR}/Entity/User
        ${CMAKE_SOURCE_DIR}/Entity/File/interface
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(FileLib PUBLIC
        FileInterface
        FSObjectLib
        UserLib
)