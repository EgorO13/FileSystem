add_library(GroupsStateLib STATIC
        group_state_service.cpp
        group_state_service.h
)

target_include_directories(GroupsStateLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(GroupsStateLib PUBLIC yaml-cpp)