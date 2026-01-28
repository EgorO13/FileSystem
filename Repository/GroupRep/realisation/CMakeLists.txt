add_library(GroupRepRealisationObjects STATIC
        group_repository.cpp
        group_repository.h
)

target_include_directories(GroupRepRealisationObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Repository/GroupRep/interface
)

target_link_libraries(GroupRepRealisationObjects PUBLIC
        GroupRepInterface
)

set_target_properties(GroupRepRealisationObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)