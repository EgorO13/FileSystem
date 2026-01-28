#ifndef LAB3_I_FS_OBJECT_H
#define LAB3_I_FS_OBJECT_H

#include "../../User/user.h"
#include "../../../base.h"
#include "../../ACL/acl_class.h"
#include <string>
#include <chrono>
#include <vector>


/**
 * @brief Интерфейс объекта файловой системы.
 *
 * Определяет общий интерфейс для всех объектов файловой системы
 * (файлов и директорий), включая управление метаданными,
 * правами доступа и временными метками.
 */
class IFileSystemObject {
public:
    virtual ~IFileSystemObject() = default;

    /**
     * @brief Обновить время последнего изменения на текущее
     */
    virtual void updateModificationTime() = 0;

    /**
     * @brief Установить несколько разрешений для субъекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param p_types Список типов разрешений
     * @param effect Эффект (разрешить/запретить)
     */
    virtual void setPermissions(unsigned int id, SubjectType s_type, std::vector<PermissionType> p_types, PermissionEffect effect = PermissionEffect::Allow) = 0;

    /**
     * @brief Установить одно разрешение для субъекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param p_types Тип разрешения
     * @param effect Эффект (разрешить/запретить)
     */
    virtual void setPermission(unsigned int id, SubjectType s_type, PermissionType p_types, PermissionEffect effect = PermissionEffect::Allow) = 0;

    /**
     * @brief Изменить имя объекта
     * @param newName Новое имя объекта
     * @return true если имя изменено, иначе false
     */
    virtual bool setName(const std::string& newName) = 0;

    /**
     * @brief Установить адрес объекта
     * @param newAddress Новый адрес объекта
     */
    virtual void setAddress(unsigned int newAddress) = 0;

    /**
     * @brief Изменить владельца объекта
     * @param newOwner Новый владелец
     */
    virtual void setOwner(const User& newOwner) = 0;

    /**
     * @brief Установить адрес родительской директории
     * @param parentAdr Новый адрес родительской директории
     */
    virtual void setParentDirectoryAddress(unsigned int parentAdr) = 0;

    /**
     * @brief Получить время создания объекта
     * @return Время создания
     */
    virtual std::chrono::system_clock::time_point getCreateTime() const = 0;

    /**
     * @brief Получить время последнего изменения
     * @return Время последнего изменения
     */
    virtual std::chrono::system_clock::time_point getLastModifyTime() const = 0;

    /**
     * @brief Получить имя объекта
     * @return Имя объекта
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Получить адрес объекта
     * @return Адрес объекта
     */
    virtual unsigned int getAddress() const = 0;

    /**
     * @brief Получить владельца объекта
     * @return Константная ссылка на владельца
     */
    virtual const User& getOwner() const = 0;

    /**
     * @brief Получить адрес родительской директории
     * @return Адрес родительской директории
     */
    virtual unsigned int getParentDirectoryAddress() const = 0;

    /**
     * @brief Получить список ACL объекта
     * @return Вектор записей ACL
     */
    virtual std::vector<ACLEntry> getACL() const = 0;

    /**
     * @brief Установить список ACL объекта
     * @param acl Новый список ACL
     */
    virtual void setACL(const std::vector<ACLEntry>& acl) = 0;

    /**
     * @brief Установить время создания объекта
     * @param time Новое время создания
     */
    virtual void setCreateTime(std::chrono::system_clock::time_point time) = 0;

    /**
     * @brief Установить время последнего изменения
     * @param time Новое время последнего изменения
     */
    virtual void setLastModifyTime(std::chrono::system_clock::time_point time) = 0;

    /**
     * @brief Проверить разрешение для пользователя
     * @param userId Идентификатор пользователя
     * @param userGroups Группы пользователя
     * @param perm Тип разрешения
     * @return true если разрешение есть, иначе false
     */
    virtual bool checkPermission(unsigned int userId, const std::vector<unsigned int>& userGroups, PermissionType perm) const = 0;
};

#endif