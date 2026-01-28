#include "command_service.h"
#include "Command/Commands/realisation/Basic/basic_commands.h"
#include "Command/Commands/realisation/Composite/composite_command.h"
#include "FileSystem/realisation/file_system.h"
#include <sstream>
#include <iostream>
#include <memory>

CommandService::CommandService(ICommandRepository& cmdRepo, ISecurityService& secService)
    : commandRepository(cmdRepo), securityService(secService) {
}

CommandResult CommandService::executeCommand(const std::string& commandName, const std::vector<std::string>& args, IFileSystem& fs) {
    if (!fs.isLoggedIn()) return CommandResult{false, {}, "Not logged in"};
    ICommand* command = commandRepository.getCommand(commandName);
    if (!command) return CommandResult{false, {}, "Command not found: " + commandName};
    if (command->isOnlyForAdmin()) {
        User* currentUser = fs.getCurrentUser();
        if (!securityService.isAdministrator(*currentUser))
            return CommandResult{false, {}, "Admin rights required for command: " + commandName};
    }
    if (!command->validateArgs(args)) return CommandResult{false, {}, "Invalid arguments for command: " + commandName};
    ICompositable* compositable = dynamic_cast<ICompositable*>(command);
    if (compositable) return executeCompositeCommand(compositable, args, fs);
    return command->execute(args, fs);
}

CommandResult CommandService::executeCompositeCommand(ICompositable* composite, const std::vector<std::string>& args, IFileSystem& fs) {
    ICommand* compositeCommand = dynamic_cast<ICommand*>(composite);
    if (!compositeCommand) return CommandResult{false, {}, "Invalid composite command"};
    std::set<std::string> visited;
    if (checkRecursion(compositeCommand->getName(), composite, visited)) {
        return CommandResult{false, {},"Recursive composite command detected: " + compositeCommand->getName()};
    }
    CommandResult finalResult{true, {}, ""};
    for (size_t i = 0; i < composite->getCommandCount(); i++) {
        const auto& entry = composite->getSubCommands()[i];
        ICommand* subCommand = commandRepository.getCommand(entry.commandName);
        if (!subCommand) {
            finalResult.success = false;
            finalResult.error = "Command not found in composite: " + entry.commandName;
            break;
        }
        if (subCommand->isOnlyForAdmin()) {
            User* currentUser = fs.getCurrentUser();
            if (!securityService.isAdministrator(*currentUser)) {
                finalResult.success = false;
                finalResult.error = "Admin rights required for subcommand: " + entry.commandName;
                break;
            }
        }
        std::vector<std::string> resolvedArgs;
        for (const auto& fixedArg : entry.fixedArgs) {
            resolvedArgs.push_back(fixedArg);
        }
        for (int index : entry.dynamicArgIndices) {
            if (index >= 0 && static_cast<size_t>(index) < args.size()) resolvedArgs.push_back(args[index]);
            else {
                finalResult.success = false;
                finalResult.error = "Dynamic argument index out of range: $" + std::to_string(index) +
                                   " (args count: " + std::to_string(args.size()) + ")";
                break;
            }
        }
        if (!finalResult.success) break;
        if (!subCommand->validateArgs(resolvedArgs)) {
            finalResult.success = false;
            finalResult.error = "Invalid arguments for subcommand: " + entry.commandName;
            break;
        }
        CommandResult stepResult;
        ICompositable* subCompositable = dynamic_cast<ICompositable*>(subCommand);
        if (subCompositable) stepResult = executeCompositeCommand(subCompositable, resolvedArgs, fs);
        else stepResult = subCommand->execute(resolvedArgs, fs);
        finalResult.message.insert(finalResult.message.end(), stepResult.message.begin(), stepResult.message.end());
        if (!stepResult.success) {
            finalResult.success = false;
            finalResult.error = "Composite failed at step " + std::to_string(i + 1) +
                               " (" + entry.commandName + "): " + stepResult.error;
            break;
        }
    }
    return finalResult;
}

bool CommandService::checkRecursion(const std::string& commandName, ICompositable* composite, std::set<std::string>& visited) const {
    ICommand* compositeCommand = dynamic_cast<ICommand*>(composite);
    if (!compositeCommand) return false;
    std::string compositeName = compositeCommand->getName();
    if (compositeName == commandName && composite->getCommandCount() == 0) return false;
    return checkRecursionInternal(commandName, composite, visited);
}

bool CommandService::checkRecursionInternal(const std::string& targetName, ICompositable* composite, std::set<std::string>& visited) const {
    ICommand* compositeCommand = dynamic_cast<ICommand*>(composite);
    if (!compositeCommand) return false;
    std::string compositeName = compositeCommand->getName();
    if (visited.find(compositeName) != visited.end()) return true;
    visited.insert(compositeName);
    for (const auto& entry : composite->getSubCommands()) {
        if (entry.commandName == targetName) {
            visited.erase(compositeName);
            return true;
        }
        ICommand* subCommand = commandRepository.getCommand(entry.commandName);
        if (!subCommand) continue;
        ICompositable* subCompositable = dynamic_cast<ICompositable*>(subCommand);
        if (subCompositable) {
            if (checkRecursionInternal(targetName, subCompositable, visited)) {
                visited.erase(compositeName);
                return true;
            }
        }
    }
    visited.erase(compositeName);
    return false;
}

bool CommandService::registerCommand(std::unique_ptr<ICommand> command) {
    if (!command) return false;
    std::string commandName = command->getName();
    if (commandRepository.commandExists(commandName)) return false;
    ICommand* rawCommand = command.get();
    ICompositable* compositable = dynamic_cast<ICompositable*>(rawCommand);
    if (compositable) {
        std::set<std::string> visited;
        if (checkRecursion(commandName, compositable, visited)) return false;
    }
    return commandRepository.saveCommand(commandName, std::move(command));
}

bool CommandService::unregisterCommand(const std::string& commandName) {
    return commandRepository.deleteCommand(commandName);
}

bool CommandService::createCompositeCommand(const std::string& name, const std::string& description) {
    if (commandRepository.commandExists(name)) return false;
    auto composite = std::make_unique<CompositeCommand>(name, description);
    return registerCommand(std::move(composite));
}

bool CommandService::deleteCompositeCommand(const std::string& name) {
    ICommand* command = commandRepository.getCommand(name);
    if (!command) return false;
    ICompositable* compositable = dynamic_cast<ICompositable*>(command);
    if (!compositable) return false;
    return unregisterCommand(name);
}

bool CommandService::addToComposite(const std::string& compositeName, const std::string& subCommandName, const std::vector<std::string>& fixedArgs, const std::vector<int>& dynamicIndices) {
    ICommand* command = commandRepository.getCommand(compositeName);
    ICompositable* composite = dynamic_cast<ICompositable*>(command);
    if (!composite) return false;
    if (!commandRepository.commandExists(subCommandName)) return false;
    if (subCommandName == compositeName) return false;
    std::set<std::string> visited;
    if (checkRecursion(compositeName, composite, visited)) return false;
    try {
        composite->addCommand(subCommandName, fixedArgs, dynamicIndices);
        return true;
    } catch (const std::exception&) { return false; }
}

bool CommandService::removeFromComposite(const std::string& compositeName, size_t index) {
    ICommand* command = commandRepository.getCommand(compositeName);
    ICompositable* composite = dynamic_cast<ICompositable*>(command);
    if (!composite) return false;
    return composite->removeCommand(index);
}

ICommand* CommandService::getCompositeCommand(const std::string& name) const {
    ICommand* command = commandRepository.getCommand(name);
    if (!command) return nullptr;
    ICompositable* compositable = dynamic_cast<ICompositable*>(command);
    if (!compositable) return nullptr;
    return command;
}

std::vector<std::string> CommandService::getAvailableCommands() const {
    return commandRepository.getCommandNames();
}

ICommand* CommandService::getCommand(const std::string& name) const {
    return commandRepository.getCommand(name);
}

bool CommandService::commandExists(const std::string& name) const {
    return commandRepository.commandExists(name);
}

std::vector<std::string> CommandService::getCompositeCommands() const {
    std::vector<std::string> composites;
    auto allCommands = getAvailableCommands();
    for (const auto& name : allCommands) {
        ICommand* command = getCompositeCommand(name);
        if (command) composites.push_back(name);
    }
    return composites;
}

std::string CommandService::getCompositeInfo(const std::string& name) const {
    ICommand* command = getCompositeCommand(name);
    if (!command) return "Not a composite command: " + name;
    ICompositable* composite = dynamic_cast<ICompositable*>(command);
    if (!composite) return "Not a composite command: " + name;
    std::stringstream ss;
    ss << "Composite command: " << name << "\n";
    ss << "Description: " << command->getDescription() << "\n";
    ss << "Usage: " << command->getUsage() << "\n";
    ss << "Admin only: " << (command->isOnlyForAdmin() ? "Yes" : "No") << "\n";
    ss << "Subcommands (" << composite->getCommandCount() << "):\n";
    int index = 1;
    for (const auto& entry : composite->getSubCommands()) {
        ss << "  " << index++ << ". " << entry.commandName;
        bool hasArgs = false;
        if (!entry.fixedArgs.empty() || !entry.dynamicArgIndices.empty()) {
            ss << " [";
            hasArgs = true;
        }
        for (const auto& arg : entry.fixedArgs) ss << arg << " ";
        for (int idx : entry.dynamicArgIndices) ss << "$" << idx << " ";
        if (hasArgs) {
            std::string str = ss.str();
            if (!str.empty() && str.back() == ' ') {
                str.pop_back();
                ss.str(str);
                ss.seekp(0, std::ios::end);
            }
            ss << "]";
        }
        ss << "\n";
    }
    return ss.str();
}