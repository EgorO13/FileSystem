add_library(SessionServiceLib STATIC
        session_service.cpp
        session_service.h
)

target_include_directories(SessionServiceLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Service/SessionService/interface
)

target_link_libraries(SessionServiceLib PUBLIC SessionServiceInterface)