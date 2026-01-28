add_library(StateServiceInterface INTERFACE)
target_include_directories(StateServiceInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(StateServiceInterface INTERFACE i_state_service.h)