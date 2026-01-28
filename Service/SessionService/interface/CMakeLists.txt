add_library(SessionServiceInterface INTERFACE)
target_include_directories(SessionServiceInterface INTERFACE   ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(SessionServiceInterface INTERFACE i_session_service.h)