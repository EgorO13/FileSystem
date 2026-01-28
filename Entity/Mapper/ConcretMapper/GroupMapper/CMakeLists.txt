add_library(GroupMapperLib INTERFACE)

target_sources(GroupMapperLib INTERFACE
        group_mapper.h
        group_mappers.h
)

target_include_directories(GroupMapperLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/Group
        ${CMAKE_SOURCE_DIR}/Entity/Mapper
)

target_link_libraries(GroupMapperLib INTERFACE
        MapperBase
        GroupLib
)