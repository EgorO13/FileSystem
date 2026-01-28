add_library(ControllerInterface INTERFACE)

target_include_directories(ControllerInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(ControllerInterface INTERFACE i_controller.h)