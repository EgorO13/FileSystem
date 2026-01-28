#ifndef LAB3_METRIC_FACTORY_H
#define LAB3_METRIC_FACTORY_H

#include "Threads/Metric/StatMetrics/realisation/stat_metrics.h"
#include <memory>
#include <vector>

/**
 * @brief Фабрика для создания стандартного набора метрик.
 *
 * Предоставляет статический метод для создания предопределенного
 * набора метрик для анализа файловой системы.
 */
class MetricFactory {
public:
    /**
     * @brief Создать стандартный набор метрик.
     * @return Вектор уникальных указателей на метрики
     */
    static std::vector<std::unique_ptr<IMetric>> createDefaultSet();
};

#endif