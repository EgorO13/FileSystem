add_library(UserRepRealisationObjects STATIC
        user_repository.cpp
        user_repository.h
)

target_include_directories(UserRepRealisationObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Repository/UserRep/interface
)

target_link_libraries(UserRepRealisationObjects PUBLIC
        UserRepInterface
)

set_target_properties(UserRepRealisationObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)