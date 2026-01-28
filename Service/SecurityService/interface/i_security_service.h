#ifndef LAB3_I_SECURITY_SERVICE_H
#define LAB3_I_SECURITY_SERVICE_H

#include "Entity/User/user.h"
#include "../../../Entity/FSObject/interface/i_fs_object.h"
#include "base.h"
#include <map>

/**
 * @brief Интерфейс сервиса безопасности.
 *
 * Определяет операции для проверки прав доступа пользователей к объектам файловой системы.
 */
class ISecurityService {
public:
    virtual ~ISecurityService() = default;

    /**
     * @brief Проверить наличие конкретного разрешения у пользователя для объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @param permission Тип проверяемого разрешения
     * @return true если у пользователя есть разрешение, иначе false
     */
    virtual bool checkPermission(const User& user, const IFileSystemObject& object, PermissionType permission) = 0;

    /**
     * @brief Получить все эффективные разрешения пользователя для объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return Карта типов разрешений и их состояний (true/false)
     */
    virtual std::map<PermissionType, bool> getEffectivePermissions(const User& user, const IFileSystemObject& object) = 0;

    /**
     * @brief Проверить право чтения объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право чтения, иначе false
     */
    virtual bool canRead(const User& user, const IFileSystemObject& object) = 0;

    /**
     * @brief Проверить право записи в объект
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право записи, иначе false
     */
    virtual bool canWrite(const User& user, const IFileSystemObject& object) = 0;

    /**
     * @brief Проверить право выполнения объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право выполнения, иначе false
     */
    virtual bool canExecute(const User& user, const IFileSystemObject& object) = 0;

    /**
     * @brief Проверить право модификации объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право модификации, иначе false
     */
    virtual bool canModify(const User& user, const IFileSystemObject& object) = 0;

    /**
     * @brief Проверить право изменения метаданных объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право изменения метаданных, иначе false
     */
    virtual bool canModifyMetadata(const User& user, const IFileSystemObject& object) = 0;

    /**
     * @brief Проверить право изменения разрешений объекта
     * @param user Пользователь для проверки
     * @param object Объект файловой системы
     * @return true если у пользователя есть право изменения разрешений, иначе false
     */
    virtual bool canChangePermissions(const User& user, const IFileSystemObject& object) = 0;

    /**
     * @brief Аутентифицировать пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если аутентификация не удалась
     */
    virtual User* authenticate(const std::string& username) = 0;

    /**
     * @brief Проверить, является ли пользователь администратором
     * @param user Проверяемый пользователь
     * @return true если пользователь является администратором, иначе false
     */
    virtual bool isAdministrator(const User& user) = 0;

    /**
     * @brief Проверить, является ли пользователь владельцем объекта
     * @param user Проверяемый пользователь
     * @param object Объект файловой системы
     * @return true если пользователь является владельцем объекта, иначе false
     */
    virtual bool isOwner(const User& user, const IFileSystemObject& object) = 0;
};

#endif