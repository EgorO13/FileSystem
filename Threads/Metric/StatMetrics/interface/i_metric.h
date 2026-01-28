#ifndef LAB3_I_METRIC_H
#define LAB3_I_METRIC_H

#include "Entity/Mapper/ConcretMapper/FSMappers/fs_object_mapper.h"
#include "Repository/FSRep/interface/i_fs_repository.h"
#include <vector>
#include <string>
#include <memory>

/**
 * @brief Структура контекста обработки для метрик.
 *
 * Содержит все необходимые компоненты для обработки объектов
 * файловой системы в метриках.
 */
struct ProcessingContext {
    IFileSystemRepository& repository;      ///< Репозиторий файловой системы
    PolymorphicFSObjectMapper& mapper;      ///< Маппер объектов
    const User* currentUser;                ///< Текущий пользователь
    const std::vector<unsigned int>& userGroups; ///< Группы пользователя
    bool ignorePermissions;                 ///< Флаг игнорирования прав доступа

    /**
     * @brief Получить путь к объекту файловой системы.
     * @param obj Указатель на объект файловой системы
     * @return Строка с полным путем к объекту
     */
    std::string getPath(IFileSystemObject* obj) const {
        return repository.getPath(obj);
    }
};

/**
 * @brief Интерфейс для метрик файловой системы.
 *
 * Определяет базовые методы для сбора и агрегации статистики
 * по объектам файловой системы.
 */
class IMetric {
public:
    virtual ~IMetric() = default;

    /**
     * @brief Получить имя метрики.
     * @return Строка с именем метрики
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Обработать одиночный объект файловой системы.
     * @param obj Указатель на объект файловой системы
     * @param context Контекст обработки
     */
    virtual void process(IFileSystemObject* obj, const ProcessingContext& context) = 0;

    /**
     * @brief Обработать группу объектов файловой системы.
     * @param objects Вектор указателей на объекты файловой системы
     * @param context Контекст обработки
     */
    virtual void processGroup(const std::vector<IFileSystemObject*>& objects, const ProcessingContext& context) = 0;

    /**
     * @brief Получить результаты сбора метрики.
     * @return Вектор строк с результатами
     */
    virtual std::vector<std::string> getResults() const = 0;

    /**
     * @brief Сбросить состояние метрики.
     */
    virtual void reset() = 0;

    /**
     * @brief Создать пустой клон метрики.
     * @return Уникальный указатель на клон метрики
     */
    virtual std::unique_ptr<IMetric> createEmptyClone() const = 0;

    /**
     * @brief Объединить результаты из другой метрики.
     * @param other Ссылка на другую метрику того же типа
     */
    virtual void mergeFrom(const IMetric& other) = 0;
};

#endif