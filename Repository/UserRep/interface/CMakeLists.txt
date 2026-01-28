add_library(UserRepInterface INTERFACE)
target_include_directories(UserRepInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}
)

target_sources(UserRepInterface INTERFACE i_user_repository.h)