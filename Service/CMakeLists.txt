add_subdirectory(FSService)
add_subdirectory(SecurityService)
add_subdirectory(SessionService)
add_subdirectory(StateService)
add_subdirectory(UserManagementService)

add_library(ServiceLib STATIC)

target_link_libraries(ServiceLib PUBLIC
        FSServiceLib
        SecurityServiceLib
        SessionServiceLib
        StateServiceLib
        UserManagementServiceLib
)

target_include_directories(ServiceLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Repository
)

set_target_properties(ServiceLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(ServiceLib PUBLIC EntityLib RepositoryLib yaml-cpp)