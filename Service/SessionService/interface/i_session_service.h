#ifndef LAB3_I_SESSION_SERVICE_H
#define LAB3_I_SESSION_SERVICE_H

#include "Entity/User/user.h"
#include "../../../Entity/Directory/interface/i_directory.h"

/**
 * @brief Интерфейс сервиса управления сессиями пользователей.
 *
 * Определяет операции для управления текущей сессией пользователя.
 */
class ISessionService {
public:
    virtual ~ISessionService() = default;

    /**
     * @brief Получить текущего пользователя
     * @return Указатель на текущего пользователя или nullptr если пользователь не аутентифицирован
     */
    virtual User* getCurrentUser() const = 0;

    /**
     * @brief Получить текущую директорию
     * @return Указатель на текущую директорию
     */
    virtual IDirectory* getCurrentDirectory() const = 0;

    /**
     * @brief Установить текущего пользователя
     * @param user Указатель на пользователя
     */
    virtual void setCurrentUser(User* user) = 0;

    /**
     * @brief Установить текущую директорию
     * @param dir Указатель на директорию
     */
    virtual void setCurrentDirectory(IDirectory* dir) = 0;

    /**
     * @brief Аутентифицировать пользователя по имени
     * @param username Имя пользователя
     * @return true если аутентификация успешна, иначе false
     */
    virtual bool login(const std::string& username) = 0;

    /**
     * @brief Завершить текущую сессию пользователя
     */
    virtual void logout() = 0;

    /**
     * @brief Проверить, аутентифицирован ли пользователь
     * @return true если пользователь аутентифицирован, иначе false
     */
    virtual bool isLoggedIn() const = 0;
};

#endif