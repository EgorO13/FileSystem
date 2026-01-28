#ifndef LAB3_USER_MAPPERS_H
#define LAB3_USER_MAPPERS_H

#include <memory>

#include "Entity/Mapper/mapper.h"
#include "Entity/Mapper/polymorphic_mapper.h"
#include "Entity/Mapper/dto.h"
#include "Entity/User/user.h"

/**
 * @brief Основной маппер для пользователей, преобразующий между User и UserDTO
 */
using UserMapper = Mapper<User, DTO::UserDTO, std::unique_ptr<User>>;

/**
 * @brief Конкретный маппер для подтипов пользователей
 * @tparam Subtype Конкретный тип пользователя
 */
template<typename Subtype>
using ConcreteUserMapper = SubtypeMapper<Subtype, DTO::UserDTO, std::unique_ptr<Subtype>>;

/**
 * @brief Адаптер маппера для приведения конкретного маппера пользователя к базовому типу
 * @tparam ConcreteMapper Конкретный тип маппера
 */
template<typename ConcreteMapper>
using SuperUserMapper = SubtypeMapperAdapter<User, DTO::UserDTO, ConcreteMapper>;

/**
 * @brief Полиморфный маппер для пользователей, определяющий тип пользователя по полю DTO
 */
class PolymorphicUserMapper final : public PolymorphicMapper<User, DTO::UserDTO, std::unique_ptr<User>> {
public:
    /**
     * @brief Получает ключ типа из DTO пользователя
     * @param dto DTO пользователя
     * @return Строка с типом пользователя
     */
    [[nodiscard]] std::string getKey(const DTO::UserDTO &dto) const override {
        return dto.type;
    }
};

#endif