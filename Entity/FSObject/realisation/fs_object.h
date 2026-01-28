#ifndef LAB3_FS_OBJECT_H
#define LAB3_FS_OBJECT_H

#include "../interface/i_fs_object.h"
#include "../../ACL/acl_class.h"
#include "../../../base.h"
#include "../../User/user.h"
#include <string>
#include <vector>
#include <chrono>

/**
 * @brief Базовый класс объектов файловой системы.
 *
 * Содержит общую функциональность для файлов и директорий:
 * управление метаданными, правами доступа и временными метками.
 */
class FileSystemObject : public IFileSystemObject {
protected:
    std::string name;                                   ///< Имя объекта
    unsigned int address;                               ///< Адрес в файловой системе
    unsigned int parentAddress;                         ///< Адрес родительской директории
    User owner;                                         ///< Владелец объекта
    ACL acl;                                            ///< Список контроля доступа
    std::chrono::system_clock::time_point creationTime; ///< Время создания
    std::chrono::system_clock::time_point lastModifyTime; ///< Время последнего изменения

    /**
     * @brief Проверить валидность имени объекта
     * @param filename Проверяемое имя
     * @return true если имя валидно, иначе false
     */
    static bool isValidName(const std::string& filename);

public:
    /**
     * @brief Конструктор объекта файловой системы
     * @param n Имя объекта
     * @param parAddress Адрес родительской директории
     * @param owner Владелец объекта
     * @param adr Адрес объекта в файловой системе
     */
    FileSystemObject(const std::string& n, unsigned int parAddress, const User& owner, unsigned int adr) :
    name(n), parentAddress(parAddress), owner(owner), address(adr), acl(owner.getId()),
    creationTime(std::chrono::system_clock::now()), lastModifyTime(std::chrono::system_clock::now()) {}

    /**
     * @brief Установить несколько разрешений для субъекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param p_types Список типов разрешений
     * @param effect Эффект (разрешить/запретить)
     */
    void setPermissions(unsigned int id, SubjectType s_type, std::vector<PermissionType> p_types, PermissionEffect effect = PermissionEffect::Allow) override;

    /**
     * @brief Установить одно разрешение для субъекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param p_types Тип разрешения
     * @param effect Эффект (разрешить/запретить)
     */
    void setPermission(unsigned int id, SubjectType s_type, PermissionType p_types, PermissionEffect effect = PermissionEffect::Allow) override;

    /**
     * @brief Изменить имя объекта
     * @param newName Новое имя объекта
     * @return true если имя изменено, иначе false
     */
    bool setName(const std::string& newName) override;

    /**
     * @brief Установить адрес объекта
     * @param newAddress Новый адрес объекта
     */
    void setAddress(unsigned int newAddress) override;

    /**
     * @brief Изменить владельца объекта
     * @param newOwner Новый владелец
     */
    void setOwner(const User& newOwner) override;

    /**
     * @brief Установить адрес родительской директории
     * @param parentAdr Новый адрес родительской директории
     */
    void setParentDirectoryAddress(unsigned int parentAdr) override;

    /**
     * @brief Получить время создания объекта
     * @return Время создания
     */
    std::chrono::system_clock::time_point getCreateTime() const override;

    /**
     * @brief Получить время последнего изменения
     * @return Время последнего изменения
     */
    std::chrono::system_clock::time_point getLastModifyTime() const override;

    /**
     * @brief Получить имя объекта
     * @return Имя объекта
     */
    std::string getName() const override;

    /**
     * @brief Получить адрес объекта
     * @return Адрес объекта
     */
    unsigned int getAddress() const override;

    /**
     * @brief Получить владельца объекта
     * @return Константная ссылка на владельца
     */
    const User& getOwner() const override;

    /**
     * @brief Проверить разрешение для пользователя
     * @param userId Идентификатор пользователя
     * @param userGroups Группы пользователя
     * @param perm Тип разрешения
     * @return true если разрешение есть, иначе false
     */
    bool checkPermission(unsigned int userId, const std::vector<unsigned int>& userGroups, PermissionType perm) const override;

    /**
     * @brief Получить адрес родительской директории
     * @return Адрес родительской директории
     */
    unsigned int getParentDirectoryAddress() const override { return parentAddress; }

    /**
     * @brief Получить список ACL объекта
     * @return Вектор записей ACL
     */
    std::vector<ACLEntry> getACL() const override;

    /**
     * @brief Установить список ACL объекта
     * @param acl Новый список ACL
     */
    void setACL(const std::vector<ACLEntry>& acl) override;

    /**
     * @brief Установить время создания объекта
     * @param time Новое время создания
     */
    void setCreateTime(std::chrono::system_clock::time_point time) override;

    /**
     * @brief Установить время последнего изменения
     * @param time Новое время последнего изменения
     */
    void setLastModifyTime(std::chrono::system_clock::time_point time) override;

    /**
     * @brief Обновить время последнего изменения на текущее
     */
    void updateModificationTime() override;
};

#endif