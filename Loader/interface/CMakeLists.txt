add_library(LoaderInterface INTERFACE)

target_include_directories(LoaderInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(LoaderInterface INTERFACE i_loader.h)