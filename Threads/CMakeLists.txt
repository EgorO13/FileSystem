add_subdirectory(Statistics)
add_subdirectory(Context)
add_subdirectory(Metric)

add_library(ThreadLib STATIC
        # Убраны файлы, которые теперь собираются в других библиотеках
)

target_link_libraries(ThreadLib PUBLIC
        StatisticLib
        ContextLib
        FileSystemLib
        EntityLib
        RepositoryLib
        ServiceLib
        StatMetricsRealisation
        MetricFactoryLib
)

target_include_directories(ThreadLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Context
        ${CMAKE_CURRENT_SOURCE_DIR}/Statistics
        ${CMAKE_CURRENT_SOURCE_DIR}/Metric
)

set_target_properties(ThreadLib PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)