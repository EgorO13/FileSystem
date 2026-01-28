add_library(CommandStateServiceObjects STATIC
        command_state_service.cpp
        command_state_service.h
)

target_include_directories(CommandStateServiceObjects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity
        ${CMAKE_SOURCE_DIR}/Service
        ${CMAKE_SOURCE_DIR}/Command/Commands/interface
        ${CMAKE_SOURCE_DIR}/Command/Commands/realisation/Composite
        ${CMAKE_SOURCE_DIR}/Command/CommandRep/interface
        ${CMAKE_SOURCE_DIR}/Command/CommandStateService/CommandMapper
)

target_link_libraries(CommandStateServiceObjects PUBLIC yaml-cpp)

set_target_properties(CommandStateServiceObjects PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)