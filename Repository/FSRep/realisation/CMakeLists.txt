add_subdirectory(Path)

add_library(FSRepRealisationObjects STATIC
        fs_repository.cpp
        fs_repository.h
)

target_include_directories(FSRepRealisationObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Repository/FSRep/interface
        ${CMAKE_SOURCE_DIR}/Repository/FSRep/realisation/Path
)

target_link_libraries(FSRepRealisationObjects PUBLIC
        FSRepInterface
        PathObjects
)

set_target_properties(FSRepRealisationObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)