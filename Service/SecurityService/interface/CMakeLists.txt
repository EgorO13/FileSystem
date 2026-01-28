add_library(SecurityServiceInterface INTERFACE)
target_include_directories(SecurityServiceInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(SecurityServiceInterface INTERFACE i_security_service.h)