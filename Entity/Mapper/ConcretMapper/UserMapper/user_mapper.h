#ifndef LAB3_USER_MAPPER_H
#define LAB3_USER_MAPPER_H

#include "Entity/User/user.h"
#include <yaml-cpp/yaml.h>
#include <memory>

/**
 * @brief Абстрактный интерфейс маппера для преобразования объектов User в YAML и обратно.
 *
 * Определяет базовые операции для сериализации и десериализации
 * объектов пользователей в формат YAML.
 */
class UserMapper {
public:
    virtual ~UserMapper() = default;

    /**
     * @brief Преобразует объект User в YAML узел
     * @param user Пользователь для преобразования
     * @return YAML узел с данными пользователя
     */
    virtual YAML::Node mapTo(const User& user) const = 0;

    /**
     * @brief Преобразует YAML узел в объект User
     * @param node YAML узел с данными пользователя
     * @return Уникальный указатель на созданный объект User
     */
    virtual std::unique_ptr<User> mapFrom(const YAML::Node& node) const = 0;

    /**
     * @brief Получает тип пользователя, обрабатываемого маппером
     * @return Строка с типом пользователя
     */
    virtual std::string getType() const = 0;
};

#endif