add_library(StatisticLib STATIC
        fs_stat.cpp
        fs_stat.h
)

target_link_libraries(StatisticLib PUBLIC
        ContextLib
        Threads::Threads
        StatMetricsInterface
)

target_include_directories(StatisticLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Threads/Metric/StatMetrics/interface
        ${CMAKE_SOURCE_DIR}/Threads/Context
)

set_target_properties(StatisticLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)