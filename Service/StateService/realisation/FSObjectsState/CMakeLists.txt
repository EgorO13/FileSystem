add_library(FSObjectsStateLib STATIC
        fs_state_service.cpp
        fs_state_service.h
)

target_include_directories(FSObjectsStateLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(FSObjectsStateLib PUBLIC yaml-cpp)