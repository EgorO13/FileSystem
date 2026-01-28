#ifndef LAB3_CONTEXT_H
#define LAB3_CONTEXT_H

#include "../Metric/StatMetrics/interface/i_metric.h"
#include <atomic>
#include <mutex>
#include <vector>
#include <memory>

/**
 * @brief Класс контекста для сбора и агрегации метрик.
 *
 * Обеспечивает потокобезопасное управление метриками, подсчет активных потоков
 * и объединение результатов из дочерних контекстов.
 */
class Context {
private:
    std::atomic<int>& threadCounter;                    ///< Счетчик активных потоков
    int maxThreads;                                     ///< Максимальное количество потоков
    std::vector<std::unique_ptr<IMetric>> localMetrics; ///< Локальные копии метрик
    mutable std::mutex mergeMutex;                      ///< Мьютекс для синхронизации слияния

public:
    /**
     * @brief Конструктор контекста.
     * @param counter Ссылка на атомарный счетчик потоков
     * @param maxThreads Максимальное количество потоков
     * @param templateMetrics Шаблонные метрики для клонирования
     */
    Context(std::atomic<int>& counter, int maxThreads, const std::vector<std::unique_ptr<IMetric>>& templateMetrics);

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;
    ~Context() = default;

    /**
     * @brief Получить текущее количество активных потоков.
     * @return Текущее значение счетчика потоков
     */
    int getCurrentThreadCount() const;

    /**
     * @brief Увеличить счетчик потоков.
     * @param count Количество для увеличения
     */
    void incrementThreadCounter(int count);

    /**
     * @brief Уменьшить счетчик потоков.
     * @param count Количество для уменьшения
     */
    void decrementThreadCounter(int count);

    /**
     * @brief Получить максимальное количество потоков.
     * @return Максимальное количество потоков
     */
    int getMaxThreads() const { return maxThreads; }

    /**
     * @brief Обработать одиночный объект файловой системы.
     * @param obj Указатель на объект файловой системы
     * @param procContext Контекст обработки
     */
    void processObject(IFileSystemObject* obj, const ProcessingContext& procContext);

    /**
     * @brief Обработать группу объектов файловой системы.
     * @param objects Вектор указателей на объекты файловой системы
     * @param procContext Контекст обработки
     */
    void processObjectGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& procContext);

    /**
     * @brief Объединить результаты из дочернего контекста.
     * @param childContext Ссылка на дочерний контекст
     */
    void mergeFromChild(const Context& childContext);

    /**
     * @brief Получить результаты всех метрик.
     * @return Вектор векторов строк с результатами
     */
    std::vector<std::vector<std::string>> getResults() const;
};

#endif