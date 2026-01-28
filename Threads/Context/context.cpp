#include "context.h"

Context::Context(std::atomic<int>& counter, int maxThreads, const std::vector<std::unique_ptr<IMetric>>& templateMetrics)
    : threadCounter(counter), maxThreads(maxThreads) {
    for (const auto& metric : templateMetrics) {
        localMetrics.push_back(metric->createEmptyClone());
    }
}

int Context::getCurrentThreadCount() const {
    return threadCounter.load(std::memory_order_acquire);
}

void Context::incrementThreadCounter(int count) {
    threadCounter.fetch_add(count, std::memory_order_acq_rel);
}

void Context::decrementThreadCounter(int count) {
    threadCounter.fetch_sub(count, std::memory_order_acq_rel);
}

void Context::processObject(IFileSystemObject* obj, const ProcessingContext& procContext) {
    for (auto& metric : localMetrics) {
        metric->process(obj, procContext);
    }
}

void Context::processObjectGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& procContext) {
    for (auto& metric : localMetrics) {
        metric->processGroup(objects, procContext);
    }
}

void Context::mergeFromChild(const Context& childContext) {
    std::lock_guard<std::mutex> lock(mergeMutex);
    for (size_t i = 0; i < localMetrics.size() && i < childContext.localMetrics.size(); ++i) {
        localMetrics[i]->mergeFrom(*childContext.localMetrics[i]);
    }
}

std::vector<std::vector<std::string>> Context::getResults() const {
    std::lock_guard<std::mutex> lock(mergeMutex);
    std::vector<std::vector<std::string>> allResults;
    for (const auto& metric : localMetrics) {
        allResults.push_back(metric->getResults());
    }
    return allResults;
}