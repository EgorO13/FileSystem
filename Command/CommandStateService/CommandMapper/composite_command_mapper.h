#ifndef LAB3_COMPOSITE_COMMAND_MAPPER_H
#define LAB3_COMPOSITE_COMMAND_MAPPER_H

#include "command_mapper.h"
#include "Command/Commands/realisation/Composite/composite_command.h"
#include <memory>

/**
 * @brief Конкретный маппер для составных команд
 *
 * Преобразует между объектами CompositeCommand и DTO::CompositeCommandDTO
 */
class CompositeCommandMapper final : public ConcreteCommandMapper<CompositeCommand> {
public:
    /**
     * @brief Получает ключ типа для этого маппера
     * @return Строка "composite"
     */
    [[nodiscard]] std::string getKey() const override {
        return "composite";
    }

    /**
     * @brief Получает typeid для типа команды
     * @return typeid CompositeCommand
     */
    [[nodiscard]] std::type_index getType() const override {
        return typeid(CompositeCommand);
    }

    /**
     * @brief Преобразует объект CompositeCommand в DTO
     * @param command Составная команда
     * @return DTO команды
     */
    [[nodiscard]] DTO::CompositeCommandDTO mapTo(const CompositeCommand& command) const override;

    /**
     * @brief Преобразует DTO в объект CompositeCommand
     * @param dto DTO команды
     * @return Указатель на составную команду
     * @throws std::invalid_argument Если тип DTO не "composite"
     */
    [[nodiscard]] std::unique_ptr<CompositeCommand> mapFrom(const DTO::CompositeCommandDTO& dto) const override;
};

#endif