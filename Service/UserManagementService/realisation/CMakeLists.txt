add_library(UserManagementServiceLib OBJECT
        user_management_service.cpp
        user_management_service.h
)

target_include_directories(UserManagementServiceLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Service/UserManagementService/interface
)

target_link_libraries(UserManagementServiceLib PUBLIC UserManagementServiceInterface)