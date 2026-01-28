add_library(ACLLib STATIC
        acl_class.cpp
        acl_class.h
)

target_include_directories(ACLLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)