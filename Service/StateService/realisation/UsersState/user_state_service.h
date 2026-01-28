#ifndef LAB3_USER_STATE_SERVICE_H
#define LAB3_USER_STATE_SERVICE_H

#include "Service/StateService/interface/i_state_service.h"
#include "Repository/UserRep/interface/i_user_repository.h"
#include <string>

/**
 * @brief Сервис для сохранения и загрузки состояния пользователей.
 *
 * Реализует интерфейс IStateService для работы с пользователями.
 * Сохраняет и загружает данные о пользователях и их членстве в группах в YAML-формате.
 */
class UserStateService final : public IStateService {
private:
    IUserRepository& userRepo_;  ///< Репозиторий пользователей

public:
    /**
     * @brief Конструктор сервиса состояния пользователей
     * @param userRepo Репозиторий пользователей
     */
    explicit UserStateService(IUserRepository& userRepo);

    /**
     * @brief Загрузить состояние пользователей из файла
     * @param path Путь к файлу с данными
     */
    void load(const std::string& path) override;

    /**
     * @brief Сохранить состояние пользователей в файл
     * @param path Путь к файлу для сохранения
     */
    void save(const std::string& path) override;
};

#endif