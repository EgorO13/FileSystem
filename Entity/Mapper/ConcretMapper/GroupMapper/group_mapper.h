#ifndef LAB3_GROUP_MAPPER_H
#define LAB3_GROUP_MAPPER_H

#include "group_mappers.h"
#include "Entity/Group/group.h"

/**
 * @brief Конкретная реализация маппера для объектов Group.
 *
 * Преобразует объекты Group в DTO::GroupDTO и обратно,
 * обеспечивая сериализацию и десериализацию групп пользователей.
 */
class GroupMapper final : public ConcreteGroupMapper<Group> {
public:
    /**
     * @brief Получает ключ типа для этого маппера
     * @return Строка "GROUP"
     */
    [[nodiscard]] std::string getKey() const override {
        return "GROUP";
    }

    /**
     * @brief Получает typeid для типа группы
     * @return typeid Group
     */
    [[nodiscard]] std::type_index getType() const override {
        return typeid(Group);
    }

    /**
     * @brief Преобразует объект Group в DTO
     * @param group Группа для преобразования
     * @return DTO группы
     */
    [[nodiscard]] DTO::GroupDTO mapTo(const Group& group) const override {
        DTO::GroupDTO dto;
        dto.id = group.getId();
        dto.name = group.getName();
        dto.userIds = group.getUsers();
        dto.subgroupIds = group.getSubgroups();
        return dto;
    }

    /**
     * @brief Преобразует DTO в объект Group
     * @param dto DTO группы
     * @return Уникальный указатель на созданный объект Group
     */
    [[nodiscard]] std::unique_ptr<Group> mapFrom(const DTO::GroupDTO& dto) const override {
        auto group = std::make_unique<Group>(dto.id, dto.name);
        for (unsigned int userId : dto.userIds) {
            group->addUser(userId);
        }
        for (unsigned int subgroupId : dto.subgroupIds) {
            group->addSubgroup(subgroupId);
        }
        return group;
    }
};

#endif