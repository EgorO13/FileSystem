#ifndef LAB3_GROUP_MAPPERS_H
#define LAB3_GROUP_MAPPERS_H

#include <memory>

#include "../../mapper.h"
#include "../../polymorphic_mapper.h"
#include "../../dto.h"
#include "../../../Group/group.h"

/**
 * @brief Основной маппер для групп, преобразующий между Group и GroupDTO
 */
using GroupMapper = Mapper<Group, DTO::GroupDTO, std::unique_ptr<Group>>;

/**
 * @brief Конкретный маппер для подтипов групп
 * @tparam Subtype Конкретный тип группы
 */
template<typename Subtype>
using ConcreteGroupMapper = SubtypeMapper<Subtype, DTO::GroupDTO, std::unique_ptr<Subtype>>;

/**
 * @brief Адаптер маппера для приведения конкретного маппера группы к базовому типу
 * @tparam ConcreteMapper Конкретный тип маппера
 */
template<typename ConcreteMapper>
using SuperGroupMapper = SubtypeMapperAdapter<Group, DTO::GroupDTO, ConcreteMapper>;

/**
 * @brief Полиморфный маппер для групп, определяющий тип группы по полю DTO
 */
class PolymorphicGroupMapper final : public PolymorphicMapper<Group, DTO::GroupDTO, std::unique_ptr<Group>> {
public:
    /**
     * @brief Получает ключ типа из DTO группы
     * @param dto DTO группы
     * @return Строка с типом группы
     */
    [[nodiscard]] std::string getKey(const DTO::GroupDTO &dto) const override {
        return dto.type;
    }
};

#endif