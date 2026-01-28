#ifndef LAB3_USER_MANAGEMENT_SERVICE_H
#define LAB3_USER_MANAGEMENT_SERVICE_H

#include "../interface/i_user_management_service.h"
#include "../../SecurityService/interface/i_security_service.h"
#include "../../../Repository/UserRep/interface/i_user_repository.h"
#include "../../../Repository/GroupRep/interface/i_group_repository.h"

/**
 * @brief Реализация сервиса управления пользователями и группами.
 *
 * Класс предоставляет функционал для управления пользователями, группами
 * и их взаимосвязями. Интегрируется с репозиториями пользователей и групп,
 * а также сервисом безопасности для проверки прав доступа.
 */
class UserManagementService : public IUserManagementService {
private:
    IUserRepository& userRepository;        ///< Ссылка на репозиторий пользователей
    IGroupRepository& groupRepository;      ///< Ссылка на репозиторий групп
    ISecurityService& securityService;      ///< Ссылка на сервис безопасности

    /**
     * @brief Проверить валидность имени пользователя
     * @param username Проверяемое имя пользователя
     * @return true если имя валидно, иначе false
     */
    bool validateUsername(const std::string& username) const;

    /**
     * @brief Проверить валидность имени группы
     * @param groupName Проверяемое имя группы
     * @return true если имя валидно, иначе false
     */
    bool validateGroupName(const std::string& groupName) const;

public:
    /**
     * @brief Конструктор сервиса управления пользователями
     * @param userRepo Репозиторий пользователей
     * @param groupRepo Репозиторий групп
     * @param secService Сервис безопасности
     */
    UserManagementService(IUserRepository& userRepo, IGroupRepository& groupRepo, ISecurityService& secService);

    /**
     * @brief Создать нового пользователя
     * @param username Имя нового пользователя
     * @param root Корневая директория для установки прав
     * @param isAdmin Флаг создания пользователя с правами администратора
     * @return true если пользователь успешно создан, иначе false
     */
    bool createUser(const std::string& username, IDirectory* root, bool isAdmin = false) override;

    /**
     * @brief Удалить пользователя
     * @param username Имя удаляемого пользователя
     * @return true если пользователь успешно удален, иначе false
     */
    bool deleteUser(const std::string& username) override;

    /**
     * @brief Изменить данные пользователя
     * @param username Текущее имя пользователя
     * @param newUsername Новое имя пользователя (пустая строка - не изменять)
     * @return true если изменения успешно применены, иначе false
     */
    bool modifyUser(const std::string& username, const std::string& newUsername = "") override;

    /**
     * @brief Получить пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    User* getUser(const std::string& username) override;

    /**
     * @brief Получить всех пользователей
     * @return Вектор указателей на всех пользователей
     */
    std::vector<User*> getAllUsers() override;

    /**
     * @brief Проверить существование пользователя по имени
     * @param username Имя пользователя
     * @return true если пользователь существует, иначе false
     */
    bool userExists(const std::string& username) const override;

    /**
     * @brief Создать новую группу
     * @param groupName Имя новой группы
     * @return true если группа успешно создана, иначе false
     */
    bool createGroup(const std::string& groupName) override;

    /**
     * @brief Удалить группу
     * @param groupName Имя удаляемой группы
     * @return true если группа успешно удалена, иначе false
     */
    bool deleteGroup(const std::string& groupName) override;

    /**
     * @brief Получить группу по имени
     * @param groupName Имя группы
     * @return Указатель на группу или nullptr если не найдена
     */
    Group* getGroup(const std::string& groupName) override;

    /**
     * @brief Получить все группы
     * @return Вектор указателей на все группы
     */
    std::vector<Group*> getAllGroups() override;

    /**
     * @brief Проверить существование группы по имени
     * @param groupName Имя группы
     * @return true если группа существует, иначе false
     */
    bool groupExists(const std::string& groupName) const override;

    /**
     * @brief Добавить пользователя в группу
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return true если операция успешна, иначе false
     */
    bool addUserToGroup(const std::string& username, const std::string& groupName) override;

    /**
     * @brief Удалить пользователя из группы
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return true если операция успешна, иначе false
     */
    bool removeUserFromGroup(const std::string& username, const std::string& groupName) override;

    /**
     * @brief Получить список групп пользователя
     * @param username Имя пользователя
     * @return Вектор имен групп пользователя
     */
    std::vector<std::string> getUserGroups(const std::string& username) override;

    /**
     * @brief Проверить, состоит ли пользователь в группе
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return true если пользователь состоит в группе, иначе false
     */
    bool isUserInGroup(const std::string& username, const std::string& groupName) override;
};

#endif