add_library(SecurityServiceLib STATIC
        security_service.cpp
        security_service.h
)

target_include_directories(SecurityServiceLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Service/SecurityService/interface
)

target_link_libraries(SecurityServiceLib PUBLIC SecurityServiceInterface)