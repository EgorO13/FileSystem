add_library(MetricFactoryLib STATIC
        metric_factory.cpp
        metric_factory.h
)

target_include_directories(MetricFactoryLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../StatMetrics/interface
        ${CMAKE_CURRENT_SOURCE_DIR}/../StatMetrics/realisation
)

target_link_libraries(MetricFactoryLib PUBLIC
        StatMetricsRealisation
        StatMetricsInterface
)