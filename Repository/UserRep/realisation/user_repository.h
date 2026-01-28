#ifndef LAB3_USER_REPOSITORY_H
#define LAB3_USER_REPOSITORY_H

#include "../interface/i_user_repository.h"
#include <map>
#include <unordered_map>
#include <vector>

/**
 * @brief Репозиторий пользователей.
 *
 * Реализует интерфейс IUserRepository для управления пользователями
 * системы с использованием ID и имен, поддерживает аутентификацию
 * и генерацию уникальных идентификаторов.
 */
class UserRepository : public IUserRepository {
private:
    std::map<unsigned int, std::unique_ptr<User>> usersById;    ///< Карта пользователей по ID
    std::unordered_map<std::string, unsigned int> idByName;     ///< Карта ID по имени пользователя
    unsigned int nextId;                                        ///< Следующий доступный ID

public:
    /**
     * @brief Конструктор репозитория
     */
    UserRepository();

    /**
     * @brief Деструктор репозитория
     */
    ~UserRepository() = default;

    /**
     * @brief Получить пользователя по ID
     * @param id ID пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    User* getUserById(unsigned int id) override;

    /**
     * @brief Получить пользователя по имени
     * @param name Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    User* getUserByName(const std::string& name) override;

    /**
     * @brief Получить всех пользователей
     * @return Вектор указателей на всех пользователей
     */
    std::vector<User*> getAllUsers() override;

    /**
     * @brief Сохранить пользователя в репозитории
     * @param user Уникальный указатель на пользователя для сохранения
     * @return true если сохранение успешно, иначе false
     */
    bool saveUser(std::unique_ptr<User> user) override;

    /**
     * @brief Удалить пользователя по ID
     * @param id ID удаляемого пользователя
     * @return true если удаление успешно, иначе false
     */
    bool deleteUser(unsigned int id) override;

    /**
     * @brief Проверить существование пользователя по ID
     * @param id ID пользователя
     * @return true если пользователь существует, иначе false
     */
    bool userExists(unsigned int id) const override;

    /**
     * @brief Проверить существование пользователя по имени
     * @param name Имя пользователя
     * @return true если пользователь существует, иначе false
     */
    bool userExists(const std::string& name) const override;

    /**
     * @brief Аутентифицировать пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    User* authenticate(const std::string& username) override;

    /**
     * @brief Получить следующий доступный ID
     * @return Следующий ID
     */
    unsigned int getNextId() override { return nextId++; }

    /**
     * @brief Очистить репозиторий
     */
    void clear() override;
};

#endif