#include "command_state_service.h"
#include "Command/Commands/realisation/Composite/composite_command.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <memory>

CompositeStateService::CompositeStateService(ICommandRepository& repo, PolymorphicCommandMapper& mapper)
    : commandRepository_(repo), mapper_(mapper) {}

void CompositeStateService::load(const std::string& path) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) throw std::runtime_error("Cannot open file: " + path);
        YAML::Node root = YAML::Load(file);
        if (root["commands"]) {
            for (const auto& cmdNode : root["commands"]) {
                DTO::CompositeCommandDTO dto;
                dto.type = cmdNode["type"].as<std::string>("composite");
                dto.name = cmdNode["name"].as<std::string>("");
                dto.description = cmdNode["description"].as<std::string>("");
                dto.usage = cmdNode["usage"].as<std::string>("");
                dto.onlyForAdmin = cmdNode["onlyForAdmin"].as<bool>(false);
                if (cmdNode["subCommands"]) {
                    for (const auto& subNode : cmdNode["subCommands"]) {
                        DTO::SubCommandEntryDTO subDto;
                        subDto.commandName = subNode["commandName"].as<std::string>("");
                        if (subNode["fixedArgs"]) subDto.fixedArgs = subNode["fixedArgs"].as<std::vector<std::string>>();
                        if (subNode["dynamicIndices"]) subDto.dynamicIndices = subNode["dynamicIndices"].as<std::vector<int>>();
                        dto.subCommands.push_back(subDto);
                    }
                }

                if (!dto.name.empty() && dto.type == "composite") {
                    bool valid = true;
                    for (const auto& subCmd : dto.subCommands) {
                        if (subCmd.commandName.empty()) {
                            valid = false;
                            break;
                        }
                        for (int idx : subCmd.dynamicIndices) {
                            if (idx < 0) {
                                valid = false;
                                break;
                            }
                        }
                        if (!valid) break;
                    }
                    if (valid) {
                        auto command = mapper_.mapFrom(dto);
                        if (command) commandRepository_.saveCommand(dto.name, std::move(command));
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load commands from " + path + ": " + e.what());
    }
}

void CompositeStateService::save(const std::string& path) {
    try {
        auto allCommands = commandRepository_.getAllCommands();
        std::vector<DTO::CompositeCommandDTO> dtos;

        for (const auto& [name, command] : allCommands) {
            if (dynamic_cast<ICompositable*>(command)) {
                DTO::CompositeCommandDTO dto(
                    "composite",
                    command->getName(),
                    command->getDescription(),
                    command->getUsage(),
                    command->isOnlyForAdmin()
                );
                for (const auto& entry : dynamic_cast<ICompositable*>(command)->getSubCommands()) {
                    DTO::SubCommandEntryDTO subDto(entry.commandName, entry.fixedArgs, entry.dynamicArgIndices);
                    dto.subCommands.push_back(subDto);
                }
                dtos.push_back(dto);
            }
        }

        YAML::Emitter emitter;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "commands" << YAML::Value << YAML::BeginSeq;
        for (const auto& dto : dtos) {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "type" << YAML::Value << dto.type;
            emitter << YAML::Key << "name" << YAML::Value << dto.name;
            emitter << YAML::Key << "description" << YAML::Value << dto.description;
            emitter << YAML::Key << "usage" << YAML::Value << dto.usage;
            emitter << YAML::Key << "onlyForAdmin" << YAML::Value << dto.onlyForAdmin;
            emitter << YAML::Key << "subCommands" << YAML::Value << YAML::BeginSeq;
            for (const auto& subCmd : dto.subCommands) {
                emitter << YAML::BeginMap;
                emitter << YAML::Key << "commandName" << YAML::Value << subCmd.commandName;
                if (!subCmd.fixedArgs.empty()) {
                    emitter << YAML::Key << "fixedArgs" << YAML::Value << YAML::Flow << subCmd.fixedArgs;
                }
                if (!subCmd.dynamicIndices.empty()) {
                    emitter << YAML::Key << "dynamicIndices" << YAML::Value << YAML::Flow << subCmd.dynamicIndices;
                }
                emitter << YAML::EndMap;
            }
            emitter << YAML::EndSeq;
            emitter << YAML::EndMap;
        }
        emitter << YAML::EndSeq;
        emitter << YAML::EndMap;

        std::string yamlContent = emitter.c_str();
        std::ofstream file(path);
        if (!file.is_open()) throw std::runtime_error("Cannot open file for writing: " + path);
        file << yamlContent;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save commands to " + path + ": " + e.what());
    }
}