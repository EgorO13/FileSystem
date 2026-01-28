#ifndef LAB3_ACL_H
#define LAB3_ACL_H

#include "../../base.h"
#include <map>
#include <vector>
#include <utility>

/**
 * @brief Запись ACL (Access Control List) для одного субъекта.
 *
 * Хранит идентификатор субъекта, тип субъекта и карту разрешений
 * с соответствующими эффектами (разрешить/запретить).
 */
struct ACLEntry {
    unsigned int subjectId;         ///< Идентификатор субъекта
    SubjectType subjectType;        ///< Тип субъекта (пользователь или группа)
    std::map<PermissionType, PermissionEffect> permissions;  ///< Карта разрешений и их эффектов

    /**
     * @brief Установить разрешение для субъекта
     * @param p_type Тип разрешения
     * @param effect Эффект (разрешить/запретить)
     */
    void setPermission(PermissionType p_type, PermissionEffect effect) { permissions[p_type] = effect; }

    /**
     * @brief Удалить разрешение у субъекта
     * @param p_type Тип разрешения для удаления
     */
    void removePermission(PermissionType p_type) { permissions.erase(p_type); }

    /**
     * @brief Проверить наличие разрешения у субъекта
     * @param p_type Тип разрешения
     * @return true если разрешение существует, иначе false
     */
    [[nodiscard]] bool hasPermission(PermissionType p_type) const {
        return permissions.find(p_type) != permissions.end();
    }

    /**
     * @brief Получить эффект для конкретного разрешения
     * @param p_type Тип разрешения
     * @return Эффект разрешения, или Deny если разрешение не найдено
     */
    [[nodiscard]] PermissionEffect getPermissionEffect(PermissionType p_type) const {
        auto it = permissions.find(p_type);
        if (it != permissions.end()) return it->second;
        return PermissionEffect::Deny;
    }
};

using ACLKey = std::pair<unsigned int, SubjectType>;  ///< Ключ для карты записей ACL

/**
 * @brief Класс ACL (Access Control List) для управления правами доступа.
 *
 * Реализует механизм контроля доступа на основе списков управления доступом
 * с поддержкой наследования прав от групп и явных запретов.
 */
class ACL {
private:
    unsigned int ownerId;                           ///< Идентификатор владельца
    std::map<ACLKey, ACLEntry> entries;            ///< Карта записей ACL

    /**
     * @brief Проверить наличие явного запрета для пользователя и его групп
     * @param userId Идентификатор пользователя
     * @param userGroups Группы пользователя
     * @param p_type Тип разрешения
     * @return true если найден явный запрет, иначе false
     */
    [[nodiscard]] bool hasExplicitDeny(unsigned int userId, const std::vector<unsigned int>& userGroups, PermissionType p_type) const;

public:
    /**
     * @brief Конструктор
     */
    ACL(unsigned int oId) : ownerId(oId) {}

    /**
     * @brief Получить идентификатор владельца
     * @return Идентификатор владельца
     */
    [[nodiscard]] unsigned int getOwner() const { return ownerId; }

    /**
     * @brief Установить идентификатор владельца
     * @param id Новый идентификатор владельца
     */
    void setOwnerId(unsigned int id) { ownerId = id; }

    /**
     * @brief Установить одно разрешение для субъекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param p_type Тип разрешения
     * @param effect Эффект (разрешить/запретить)
     */
    void setPermission(unsigned int id, SubjectType s_type, PermissionType p_type, PermissionEffect effect);

    /**
     * @brief Установить несколько разрешений для субъекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param p_types Список типов разрешений
     * @param effect Эффект (разрешить/запретить)
     */
    void setPermissions(unsigned int id, SubjectType s_type, const std::vector<PermissionType>& p_types, PermissionEffect effect);

    /**
     * @brief Удалить разрешение у субъекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param p_type Тип разрешения для удаления
     */
    void removePermission(unsigned int id, SubjectType s_type, PermissionType p_type);

    /**
     * @brief Проверить наличие разрешения у пользователя с учетом групп
     * @param userId Идентификатор пользователя
     * @param userGroups Группы пользователя
     * @param p_type Тип разрешения
     * @return true если разрешение есть, иначе false
     */
    [[nodiscard]] bool checkPermission(unsigned int userId, const std::vector<unsigned int>& userGroups, PermissionType p_type) const;

    /**
     * @brief Получить все эффективные разрешения для пользователя
     * @param userId Идентификатор пользователя
     * @param userGroups Группы пользователя
     * @return Карта "тип разрешения -> наличие"
     */
    [[nodiscard]] std::map<PermissionType, bool> getEffectivePermissions(unsigned int userId, const std::vector<unsigned int>& userGroups) const;

    /**
     * @brief Получить все записи ACL
     * @return Вектор записей ACL
     */
    std::vector<ACLEntry> getEntries() const;

    /**
     * @brief Установить все записи ACL
     * @param newEntries Новый набор записей ACL
     */
    void setEntries(const std::vector<ACLEntry>& newEntries);
};

#endif