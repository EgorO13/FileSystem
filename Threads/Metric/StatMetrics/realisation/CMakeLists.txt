add_library(StatMetricsRealisation STATIC
        stat_metrics.cpp
        stat_metrics.h
)

target_include_directories(StatMetricsRealisation PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../interface
)

target_link_libraries(StatMetricsRealisation PUBLIC
        StatMetricsInterface
        EntityLib
        RepositoryLib
)