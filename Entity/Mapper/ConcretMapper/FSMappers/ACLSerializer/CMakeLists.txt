add_library(ACLSerializerLib STATIC
        acl_serializer.cpp
        acl_serializer.h
)

target_include_directories(ACLSerializerLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Entity/ACL
        ${CMAKE_SOURCE_DIR}/Entity/Mapper
)

target_link_libraries(ACLSerializerLib PUBLIC
        MapperBase
        ACLLib
)

set_target_properties(ACLSerializerLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)