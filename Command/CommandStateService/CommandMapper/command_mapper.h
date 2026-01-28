#ifndef LAB3_COMMAND_MAPPER_H
#define LAB3_COMMAND_MAPPER_H

#include "Entity/Mapper/polymorphic_mapper.h"
#include "Command/Commands/interface/i_command.h"
#include "Entity/Mapper/dto.h"
#include <memory>

/**
 * @brief Основной маппер для команд, преобразующий между ICommand и DTO
 */
using CommandMapper = Mapper<ICommand, DTO::CompositeCommandDTO, std::unique_ptr<ICommand>>;

/**
 * @brief Конкретный маппер для подтипов команд
 * @tparam Subtype Конкретный тип команды
 */
template<typename Subtype>
using ConcreteCommandMapper = SubtypeMapper<Subtype, DTO::CompositeCommandDTO, std::unique_ptr<Subtype>>;

/**
 * @brief Адаптер маппера для приведения конкретного маппера к базовому типу
 * @tparam ConcreteMapper Конкретный тип маппера
 */
template<typename ConcreteMapper>
using SuperCommandMapper = SubtypeMapperAdapter<ICommand, std::unique_ptr<ICommand>, ConcreteMapper>;

/**
 * @brief Полиморфный маппер для команд, определяющий тип команды по полю DTO
 */
class PolymorphicCommandMapper final : public PolymorphicMapper<ICommand, DTO::CompositeCommandDTO, std::unique_ptr<ICommand>> {
public:
    /**
     * @brief Получает ключ типа из DTO
     * @param dto DTO команды
     * @return Строка с типом команды
     */
    [[nodiscard]] std::string getKey(const DTO::CompositeCommandDTO& dto) const override {
        return dto.type;
    }
};

#endif