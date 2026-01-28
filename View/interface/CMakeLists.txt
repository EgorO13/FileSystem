add_library(ViewInterface INTERFACE)

target_include_directories(ViewInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(ViewInterface INTERFACE i_view.h)
