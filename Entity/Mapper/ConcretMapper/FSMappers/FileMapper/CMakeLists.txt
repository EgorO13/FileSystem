add_library(FileMapperLib STATIC
        file_mapper.cpp
        file_mapper.h
)

target_include_directories(FileMapperLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/File
        ${CMAKE_SOURCE_DIR}/Entity/Mapper
)

target_link_libraries(FileMapperLib PUBLIC
        MapperBase
        FileLib
        FileInterface
        ACLSerializerLib
)