#ifndef LAB3_I_USER_MANAGEMENT_SERVICE_H
#define LAB3_I_USER_MANAGEMENT_SERVICE_H

#include "Entity/User/user.h"
#include "Entity/Group/group.h"
#include "../../../Entity/Directory/interface/i_directory.h"
#include <vector>
#include <string>

/**
 * @brief Интерфейс сервиса управления пользователями и группами.
 *
 * Определяет операции для создания, удаления, модификации пользователей и групп,
 * управления членством пользователей в группах и проверки существования.
 */
class IUserManagementService {
public:
    virtual ~IUserManagementService() = default;

    /**
     * @brief Создать нового пользователя
     * @param username Имя нового пользователя
     * @param root Корневая директория для установки прав
     * @param isAdmin Флаг создания пользователя с правами администратора
     * @return true если пользователь успешно создан, иначе false
     */
    virtual bool createUser(const std::string& username, IDirectory* root, bool isAdmin = false) = 0;

    /**
     * @brief Удалить пользователя
     * @param username Имя удаляемого пользователя
     * @return true если пользователь успешно удален, иначе false
     */
    virtual bool deleteUser(const std::string& username) = 0;

    /**
     * @brief Изменить данные пользователя
     * @param username Текущее имя пользователя
     * @param newUsername Новое имя пользователя (пустая строка - не изменять)
     * @return true если изменения успешно применены, иначе false
     */
    virtual bool modifyUser(const std::string& username, const std::string& newUsername = "") = 0;

    /**
     * @brief Получить пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    virtual User* getUser(const std::string& username) = 0;

    /**
     * @brief Получить всех пользователей
     * @return Вектор указателей на всех пользователей
     */
    virtual std::vector<User*> getAllUsers() = 0;

    /**
     * @brief Проверить существование пользователя по имени
     * @param username Имя пользователя
     * @return true если пользователь существует, иначе false
     */
    virtual bool userExists(const std::string& username) const = 0;

    /**
     * @brief Создать новую группу
     * @param groupName Имя новой группы
     * @return true если группа успешно создана, иначе false
     */
    virtual bool createGroup(const std::string& groupName) = 0;

    /**
     * @brief Удалить группу
     * @param groupName Имя удаляемой группы
     * @return true если группа успешно удалена, иначе false
     */
    virtual bool deleteGroup(const std::string& groupName) = 0;

    /**
     * @brief Получить группу по имени
     * @param groupName Имя группы
     * @return Указатель на группу или nullptr если не найдена
     */
    virtual Group* getGroup(const std::string& groupName) = 0;

    /**
     * @brief Получить все группы
     * @return Вектор указателей на все группы
     */
    virtual std::vector<Group*> getAllGroups() = 0;

    /**
     * @brief Проверить существование группы по имени
     * @param groupName Имя группы
     * @return true если группа существует, иначе false
     */
    virtual bool groupExists(const std::string& groupName) const = 0;

    /**
     * @brief Добавить пользователя в группу
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return true если операция успешна, иначе false
     */
    virtual bool addUserToGroup(const std::string& username, const std::string& groupName) = 0;

    /**
     * @brief Удалить пользователя из группы
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return true если операция успешна, иначе false
     */
    virtual bool removeUserFromGroup(const std::string& username, const std::string& groupName) = 0;

    /**
     * @brief Получить список групп пользователя
     * @param username Имя пользователя
     * @return Вектор имен групп пользователя
     */
    virtual std::vector<std::string> getUserGroups(const std::string& username) = 0;

    /**
     * @brief Проверить, состоит ли пользователь в группе
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return true если пользователь состоит в группе, иначе false
     */
    virtual bool isUserInGroup(const std::string& username, const std::string& groupName) = 0;
};

#endif