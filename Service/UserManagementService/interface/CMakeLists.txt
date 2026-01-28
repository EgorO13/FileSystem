add_library(UserManagementServiceInterface INTERFACE)
target_include_directories(UserManagementServiceInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(UserManagementServiceInterface INTERFACE i_user_management_service.h)