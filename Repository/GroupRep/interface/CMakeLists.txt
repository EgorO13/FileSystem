add_library(GroupRepInterface INTERFACE)
target_include_directories(GroupRepInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
)

target_sources(GroupRepInterface INTERFACE i_group_repository.h)