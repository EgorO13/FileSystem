#ifndef LAB3_SECURITY_SERVICE_H
#define LAB3_SECURITY_SERVICE_H

#include "../interface/i_security_service.h"
#include "../../../Repository/UserRep/interface/i_user_repository.h"
#include "../../../Repository/GroupRep/interface/i_group_repository.h"
#include <vector>

/**
 * @brief Реализация сервиса безопасности.
 *
 * Класс предоставляет функционал для проверки прав доступа пользователей,
 * управления аутентификацией и проверки привилегий в файловой системе.
 * Интегрируется с репозиториями пользователей и групп для определения прав.
 */
class SecurityService : public ISecurityService {
private:
    IUserRepository& userRepository;                        ///< Ссылка на репозиторий пользователей
    IGroupRepository& groupRepository;                      ///< Ссылка на репозиторий групп
    const std::string ADMIN_GROUP_NAME = "Administrators";  ///< Имя группы администраторов

    /**
     * @brief Получить все идентификаторы групп пользователя (включая родительские)
     * @param user Пользователь
     * @return Вектор идентификаторов групп
     */
    std::vector<unsigned int> getUserGroupIds(const User& user) const;

    /**
     * @brief Проверить явное разрешение без учета прав владельца
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @param permission Тип проверяемого разрешения
     * @return true если у пользователя есть явное разрешение, иначе false
     */
    bool checkExplicitPermission(const User& user, const IFileSystemObject& object, PermissionType permission) const;

public:
    /**
     * @brief Конструктор сервиса безопасности
     * @param userRepo Репозиторий пользователей
     * @param groupRepo Репозиторий групп
     */
    SecurityService(IUserRepository& userRepo, IGroupRepository& groupRepo);

    /**
     * @brief Проверить наличие конкретного разрешения у пользователя для объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @param permission Тип проверяемого разрешения
     * @return true если у пользователя есть разрешение, иначе false
     */
    bool checkPermission(const User& user, const IFileSystemObject& object, PermissionType permission) override;

    /**
     * @brief Получить все эффективные разрешения пользователя для объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return Карта типов разрешений и их состояний (true/false)
     */
    std::map<PermissionType, bool> getEffectivePermissions(const User& user, const IFileSystemObject& object) override;

    /**
     * @brief Проверить право чтения объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право чтения, иначе false
     */
    bool canRead(const User& user, const IFileSystemObject& object) override;

    /**
     * @brief Проверить право записи в объект
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право записи, иначе false
     */
    bool canWrite(const User& user, const IFileSystemObject& object) override;

    /**
     * @brief Проверить право выполнения объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право выполнения, иначе false
     */
    bool canExecute(const User& user, const IFileSystemObject& object) override;

    /**
     * @brief Проверить право модификации объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право модификации, иначе false
     */
    bool canModify(const User& user, const IFileSystemObject& object) override;

    /**
     * @brief Проверить право изменения метаданных объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право изменения метаданных, иначе false
     */
    bool canModifyMetadata(const User& user, const IFileSystemObject& object) override;

    /**
     * @brief Проверить право изменения разрешений объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право изменения разрешений, иначе false
     */
    bool canChangePermissions(const User& user, const IFileSystemObject& object) override;

    /**
     * @brief Аутентифицировать пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если аутентификация не удалась
     */
    User* authenticate(const std::string& username) override;

    /**
     * @brief Проверить, является ли пользователь администратором
     * @param user Проверяемый пользователь
     * @return true если пользователь является администратором, иначе false
     */
    bool isAdministrator(const User& user) override;

    /**
     * @brief Проверить, является ли пользователь владельцем объекта
     * @param user Проверяемый пользователь
     * @param object Объект файловой системы
     * @return true если пользователь является владельцем объекта, иначе false
     */
    bool isOwner(const User& user, const IFileSystemObject& object) override;
};

#endif