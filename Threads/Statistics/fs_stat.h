#ifndef LAB3_FS_STAT_H
#define LAB3_FS_STAT_H

#include "FileSystem/interface/i_file_system.h"
#include "Entity/Mapper/ConcretMapper/FSMappers/fs_object_mapper.h"
#include "Threads/Context/context.h"
#include "Threads/Metric/StatMetrics/interface/i_metric.h"
#include <atomic>
#include <future>
#include <vector>
#include <memory>
#include <mutex>

/**
 * @brief Сканер файловой системы с поддержкой многопоточности.
 *
 * Осуществляет рекурсивный обход файловой системы, сбор статистики
 * через метрики и параллельную обработку поддиректорий.
 */
class FileSystemScanner {
private:
    std::atomic<int> activeThreadCounter{0}; ///< Счетчик активных потоков
    int maxThreads;                          ///< Максимальное количество потоков
    IFileSystemRepository& repository;       ///< Репозиторий файловой системы
    PolymorphicFSObjectMapper& mapper;       ///< Маппер объектов
    const User* currentUser;                 ///< Текущий пользователь
    const std::vector<unsigned int>& userGroups; ///< Группы пользователя
    bool ignorePermissions;                  ///< Флаг игнорирования прав доступа
    std::vector<std::unique_ptr<IMetric>> templateMetrics; ///< Шаблонные метрики
    mutable std::mutex templateMetricsMutex; ///< Мьютекс для защиты шаблонных метрик

    /**
     * @brief Рекурсивно сканировать директорию.
     * @param directory Указатель на директорию для сканирования
     * @param context Контекст для сбора метрик
     */
    void scanDirectory(IDirectory* directory, Context& context);

public:
    /**
     * @brief Конструктор сканера файловой системы.
     * @param maxThreads Максимальное количество потоков
     * @param repo Репозиторий файловой системы
     * @param mapper Маппер объектов
     * @param user Текущий пользователь
     * @param groups Группы пользователя
     * @param ignorePerms Флаг игнорирования прав доступа
     */
    FileSystemScanner(int maxThreads, IFileSystemRepository& repo, PolymorphicFSObjectMapper& mapper,
                        const User* user, const std::vector<unsigned int>& groups, bool ignorePerms)
        : maxThreads(maxThreads > 0 ? maxThreads : 1), repository(repo), mapper(mapper),
          currentUser(user), userGroups(groups), ignorePermissions(ignorePerms) {}

    /**
     * @brief Запустить сканирование файловой системы.
     * @param rootDirectory Указатель на корневую директорию
     * @param metrics Вектор метрик для сбора статистики
     * @return Вектор векторов строк с результатами всех метрик
     */
    std::vector<std::vector<std::string>> scan(IDirectory* rootDirectory, const std::vector<std::unique_ptr<IMetric>>& metrics);
};

#endif