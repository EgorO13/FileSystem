#ifndef LAB3_I_USER_REPOSITORY_H
#define LAB3_I_USER_REPOSITORY_H

#include "Entity/User/user.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @brief Интерфейс репозитория пользователей.
 *
 * Для управления пользователями системы:
 * создание, удаление, поиск пользователей и аутентификация.
 */
class IUserRepository {
public:
    virtual ~IUserRepository() = default;

    /**
     * @brief Получить пользователя по ID
     * @param id ID пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    virtual User* getUserById(unsigned int id) = 0;

    /**
     * @brief Получить пользователя по имени
     * @param name Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    virtual User* getUserByName(const std::string& name) = 0;

    /**
     * @brief Получить всех пользователей
     * @return Вектор указателей на всех пользователей
     */
    virtual std::vector<User*> getAllUsers() = 0;

    /**
     * @brief Сохранить пользователя в репозитории
     * @param user Уникальный указатель на пользователя для сохранения
     * @return true если сохранение успешно, иначе false
     */
    virtual bool saveUser(std::unique_ptr<User> user) = 0;

    /**
     * @brief Удалить пользователя по ID
     * @param id ID удаляемого пользователя
     * @return true если удаление успешно, иначе false
     */
    virtual bool deleteUser(unsigned int id) = 0;

    /**
     * @brief Проверить существование пользователя по ID
     * @param id ID пользователя
     * @return true если пользователь существует, иначе false
     */
    virtual bool userExists(unsigned int id) const = 0;

    /**
     * @brief Проверить существование пользователя по имени
     * @param name Имя пользователя
     * @return true если пользователь существует, иначе false
     */
    virtual bool userExists(const std::string& name) const = 0;

    /**
     * @brief Аутентифицировать пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    virtual User* authenticate(const std::string& username) = 0;

    /**
     * @brief Получить следующий доступный ID
     * @return Следующий ID
     */
    virtual unsigned int getNextId() = 0;

    /**
     * @brief Очистить репозиторий
     */
    virtual void clear() = 0;
};

#endif