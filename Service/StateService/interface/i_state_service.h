#ifndef LAB3_I_STATE_SERVICE_H
#define LAB3_I_STATE_SERVICE_H

#include <string>

/**
 * @brief Интерфейс сервиса сохранения состояния.
 *
 * Определяет методы для загрузки и сохранения состояния различных компонентов системы в файлы.
 */
class IStateService {
public:
    virtual ~IStateService() = default;

    /**
     * @brief Загрузить состояние из файла
     * @param path Путь к файлу с данными
     */
    virtual void load(const std::string& path) = 0;

    /**
     * @brief Сохранить состояние в файл
     * @param path Путь к файлу для сохранения
     */
    virtual void save(const std::string& path) = 0;
};

#endif