#include "composite_command_mapper.h"
#include "Entity/Mapper/dto.h"
#include <stdexcept>

DTO::CompositeCommandDTO CompositeCommandMapper::mapTo(const CompositeCommand& command) const {
    DTO::CompositeCommandDTO dto(
        "composite",
        command.getName(),
        command.getDescription(),
        command.getUsage(),
        command.isOnlyForAdmin()
    );
    for (const auto& entry : command.getSubCommands()) {
        DTO::SubCommandEntryDTO subDto(entry.commandName, entry.fixedArgs, entry.dynamicArgIndices);
        dto.subCommands.push_back(subDto);
    }
    return dto;
}

std::unique_ptr<CompositeCommand> CompositeCommandMapper::mapFrom(const DTO::CompositeCommandDTO& dto) const {
    if (dto.type != "composite") throw std::invalid_argument("DTO is not for a composite command");
    auto composite = std::make_unique<CompositeCommand>(dto.name, dto.description);
    for (const auto& cmd : dto.subCommands) {
        composite->addCommand(cmd.commandName, cmd.fixedArgs, cmd.dynamicIndices);
    }
    return std::move(composite);
}