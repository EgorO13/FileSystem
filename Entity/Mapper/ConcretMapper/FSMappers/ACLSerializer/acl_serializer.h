#ifndef LAB3_ACL_SERIALIZER_H
#define LAB3_ACL_SERIALIZER_H

#include "Entity/ACL/acl_class.h"
#include <string>
#include <vector>

/**
 * @brief Класс для сериализации и десериализации списков контроля доступа (ACL).
 *
 * Преобразует вектор записей ACL в строковый формат для хранения
 * и обратно, используя текстовое представление типов субъектов,
 * разрешений и их эффектов.
 */
class ACLSerializer {
public:
    /**
     * @brief Сериализует вектор записей ACL в строку
     * @param acl Вектор записей ACL для сериализации
     * @return Строковое представление ACL
     */
    static std::string serialize(const std::vector<ACLEntry>& acl);

    /**
     * @brief Десериализует строку в вектор записей ACL
     * @param str Строковое представление ACL
     * @return Вектор записей ACL
     */
    static std::vector<ACLEntry> deserialize(const std::string& str);

private:
    /**
     * @brief Преобразует тип субъекта в строку
     * @param type Тип субъекта
     * @return Строковое представление типа субъекта
     */
    static std::string subjectTypeToString(SubjectType type);

    /**
     * @brief Преобразует строку в тип субъекта
     * @param str Строковое представление типа субъекта
     * @return Тип субъекта
     */
    static SubjectType stringToSubjectType(const std::string& str);

    /**
     * @brief Преобразует тип разрешения в строку
     * @param type Тип разрешения
     * @return Строковое представление типа разрешения
     */
    static std::string permissionTypeToString(PermissionType type);

    /**
     * @brief Преобразует строку в тип разрешения
     * @param str Строковое представление типа разрешения
     * @return Тип разрешения
     */
    static PermissionType stringToPermissionType(const std::string& str);

    /**
     * @brief Преобразует эффект разрешения в строку
     * @param effect Эффект разрешения
     * @return Строковое представление эффекта разрешения
     */
    static std::string permissionEffectToString(PermissionEffect effect);

    /**
     * @brief Преобразует строку в эффект разрешения
     * @param str Строковое представление эффекта разрешения
     * @return Эффект разрешения
     */
    static PermissionEffect stringToPermissionEffect(const std::string& str);
};

#endif