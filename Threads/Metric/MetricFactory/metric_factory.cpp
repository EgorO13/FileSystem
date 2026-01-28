#include "metric_factory.h"

#include <memory>
#include <vector>


std::vector<std::unique_ptr<IMetric>> MetricFactory::createDefaultSet() {
    std::vector<std::unique_ptr<IMetric>> metrics;
    metrics.push_back(std::make_unique<TypeCounterMetric>());
    metrics.push_back(std::make_unique<SizeMetric>());
    metrics.push_back(std::make_unique<OwnerMetric>());
    return metrics;
}