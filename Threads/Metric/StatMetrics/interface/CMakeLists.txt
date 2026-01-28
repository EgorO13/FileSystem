add_library(StatMetricsInterface INTERFACE)

target_include_directories(StatMetricsInterface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_sources(StatMetricsInterface INTERFACE i_metric.h)