add_library(DirectoryMapperLib STATIC
        directory_mapper.cpp
        directory_mapper.h
)

target_include_directories(DirectoryMapperLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/Directory
        ${CMAKE_SOURCE_DIR}/Entity/Mapper
)

target_link_libraries(DirectoryMapperLib PUBLIC
        MapperBase
        DirectoryLib
        DirectoryInterface
        ACLSerializerLib
)