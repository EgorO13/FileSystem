add_subdirectory(ACLSerializer)
add_subdirectory(DirectoryMapper)
add_subdirectory(FileMapper)

add_library(FSMappersLib STATIC
        fs_object_mapper.h
)

target_link_libraries(FSMappersLib PUBLIC
        MapperBase
        ACLSerializerLib
        DirectoryMapperLib
        FileMapperLib
)

target_include_directories(FSMappersLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)