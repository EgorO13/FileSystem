#include "composite_command.h"
#include <stdexcept>

CompositeCommand::CompositeCommand(const std::string& name, const std::string& description)
    : BaseCommand(name, description, name + " [args...]\nComposite command: " + description) {}

CommandResult CompositeCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    return CommandResult{false, {}, "Composite command cannot be executed directly"};
}

bool CompositeCommand::validateArgs(const std::vector<std::string>& args) const {
    return true;
}

void CompositeCommand::addCommand(const std::string& commandName, const std::vector<std::string>& fixedArgs, const std::vector<int>& dynamicIndices) {
    if (commandName.empty()) throw std::invalid_argument("Command name cannot be empty");
    for (int index : dynamicIndices) {
        if (index < 0) throw std::invalid_argument("Dynamic argument index cannot be negative");
    }
    commands.emplace_back(commandName, fixedArgs, dynamicIndices);
}

bool CompositeCommand::removeCommand(size_t index) {
    if (index >= commands.size()) return false;
    commands.erase(commands.begin() + index);
    return true;
}

void CompositeCommand::clearCommands() {
    commands.clear();
}
