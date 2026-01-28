add_library(ViewLib STATIC
        view.cpp
        view.h
)

target_include_directories(ViewLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/View/interface
)

set_target_properties(ViewLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(ViewLib PUBLIC ViewInterface)