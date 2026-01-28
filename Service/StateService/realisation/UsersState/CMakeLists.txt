add_library(UsersStateLib STATIC
        user_state_service.cpp
        user_state_service.h
)

target_include_directories(UsersStateLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(UsersStateLib PUBLIC yaml-cpp)