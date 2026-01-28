add_library(PathObjects STATIC
        path.cpp
        path.h
)

target_include_directories(PathObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Repository/FSRep
)

set_target_properties(PathObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)