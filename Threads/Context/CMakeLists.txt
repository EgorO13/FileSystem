add_library(ContextLib STATIC
        context.cpp
        context.h
)

target_link_libraries(ContextLib PUBLIC
        Threads::Threads
        StatMetricsInterface
)

target_include_directories(ContextLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)