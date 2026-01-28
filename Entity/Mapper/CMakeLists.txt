add_library(MapperBase INTERFACE)

target_sources(MapperBase INTERFACE
        mapper.h
        polymorphic_mapper.h
        dto.h
)

target_include_directories(MapperBase INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

add_subdirectory(ConcretMapper)

add_library(MapperLib INTERFACE)

target_link_libraries(MapperLib INTERFACE
        MapperBase
        FSMappersLib
        GroupMapperLib
        UserMapperLib
)