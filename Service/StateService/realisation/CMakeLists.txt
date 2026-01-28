add_subdirectory(FSObjectsState)
add_subdirectory(GroupsState)
add_subdirectory(UsersState)

add_library(StateServiceLib STATIC)

target_link_libraries(StateServiceLib PUBLIC
        FSObjectsStateLib
        GroupsStateLib
        UsersStateLib
)

target_include_directories(StateServiceLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Service/StateService/interface
)

target_link_libraries(StateServiceLib PUBLIC yaml-cpp StateServiceInterface)