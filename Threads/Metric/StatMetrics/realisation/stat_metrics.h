#ifndef LAB3_STAT_METRICS_H
#define LAB3_STAT_METRICS_H
#include "Threads/Metric/StatMetrics/interface/i_metric.h"
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>


/**
 * @brief Метрика для сбора статистики по размерам файлов.
 *
 * Собирает общий размер, количество файлов, средний размер
 * и информацию о самом большом файле.
 */
class SizeMetric : public IMetric {
private:
    unsigned int totalSize{0};         ///< Общий размер файлов в байтах
    unsigned int largestFileSize{0};   ///< Размер самого большого файла
    std::string largestFilePath;       ///< Путь к самому большому файлу
    unsigned int fileCount{0};         ///< Количество обработанных файлов

public:
    SizeMetric() = default;
    SizeMetric(const SizeMetric&) = delete;
    SizeMetric& operator=(const SizeMetric&) = delete;
    SizeMetric(SizeMetric&& other) noexcept = default;
    SizeMetric& operator=(SizeMetric&& other) noexcept = default;

    std::string getName() const override;
    void process(IFileSystemObject* obj, const ProcessingContext& context) override;
    void processGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& context) override;
    std::vector<std::string> getResults() const override;
    void reset() override;
    std::unique_ptr<IMetric> createEmptyClone() const override;
    void mergeFrom(const IMetric& other) override;
};

/**
 * @brief Метрика для сбора статистики по владельцам объектов.
 *
 * Собирает информацию о распределении объектов по владельцам,
 * включая топ-5 владельцев и процентное соотношение.
 */
class OwnerMetric : public IMetric {
private:
    std::unordered_map<std::string, unsigned int> ownerStats; ///< Статистика по владельцам
    unsigned int totalObjects{0}; ///< Общее количество объектов

public:
    OwnerMetric() = default;
    OwnerMetric(const OwnerMetric&) = delete;
    OwnerMetric& operator=(const OwnerMetric&) = delete;
    OwnerMetric(OwnerMetric&& other) noexcept = default;
    OwnerMetric& operator=(OwnerMetric&& other) noexcept = default;

    std::string getName() const override;
    void process(IFileSystemObject* obj, const ProcessingContext& context) override;
    void processGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& context) override;
    std::vector<std::string> getResults() const override;
    void reset() override;
    std::unique_ptr<IMetric> createEmptyClone() const override;
    void mergeFrom(const IMetric& other) override;
};

/**
 * @brief Метрика для подсчета типов объектов файловой системы.
 *
 * Собирает статистику по типам объектов (файлы, директории и т.д.)
 * с процентным соотношением каждого типа.
 */
class TypeCounterMetric : public IMetric {
private:
    std::map<std::string, unsigned int> typeCounts; ///< Количество объектов по типам
    unsigned int totalObjects{0}; ///< Общее количество объектов

public:
    TypeCounterMetric() = default;

    TypeCounterMetric(const TypeCounterMetric&) = delete;
    TypeCounterMetric& operator=(const TypeCounterMetric&) = delete;
    TypeCounterMetric(TypeCounterMetric&& other) noexcept = default;
    TypeCounterMetric& operator=(TypeCounterMetric&& other) noexcept = default;

    std::string getName() const override;

    void process(IFileSystemObject* obj, const ProcessingContext& context) override;

    void processGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& context) override;

    void mergeFrom(const IMetric& other) override;

    std::vector<std::string> getResults() const override;

    std::unique_ptr<IMetric> createEmptyClone() const override;

    void reset() override;
};



#endif