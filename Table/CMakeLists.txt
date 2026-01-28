cmake_minimum_required(VERSION 3.15)
project(TableLib)

add_library(TableLib INTERFACE)

set_target_properties(TableLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(TableLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)