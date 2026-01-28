add_library(UserMapperLib INTERFACE)

target_sources(UserMapperLib INTERFACE
        user_mapper.h
        user_mappers.h
)

target_include_directories(UserMapperLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/User
        ${CMAKE_SOURCE_DIR}/Entity/Mapper
)

target_link_libraries(UserMapperLib INTERFACE
        MapperBase
        UserLib
)