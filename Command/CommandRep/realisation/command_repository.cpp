#include "command_repository.h"
#include "Command/Commands/realisation/Basic/basic_commands.h"
#include "Command/Commands/realisation/Composite/composite_command.h"


CommandRepository::CommandRepository(){
    initializeDefaultCommands();
}

void CommandRepository::initializeDefaultCommands() {
    auto cd = std::make_unique<BasicCommands::ChangeDirectoryCommand>();
    auto ls = std::make_unique<BasicCommands::ListDirectoryCommand>();
    auto mkdir = std::make_unique<BasicCommands::MakeDirectoryCommand>();
    auto touch = std::make_unique<BasicCommands::CreateFileCommand>();
    auto cat = std::make_unique<BasicCommands::ReadFileCommand>();
    auto rm = std::make_unique<BasicCommands::DeleteFileCommand>();
    auto rmdir = std::make_unique<BasicCommands::DeleteDirectoryCommand>();
    auto cp = std::make_unique<BasicCommands::CopyCommand>();
    auto mv = std::make_unique<BasicCommands::MoveCommand>();
    auto chmod = std::make_unique<BasicCommands::ChangePermissionsCommand>();
    auto chown = std::make_unique<BasicCommands::ChangeOwnerCommand>();
    auto stat = std::make_unique<BasicCommands::StatisticsCommand>();
    auto find = std::make_unique<BasicCommands::FindCommand>();
    auto useradd = std::make_unique<BasicCommands::CreateUserCommand>();
    auto groupadd = std::make_unique<BasicCommands::CreateGroupCommand>();
    auto usermod = std::make_unique<BasicCommands::AddUserToGroupCommand>();
    auto groupdel = std::make_unique<BasicCommands::DeleteGroupCommand>();
    auto userdel = std::make_unique<BasicCommands::DeleteUserCommand>();
    auto edit = std::make_unique<BasicCommands::EditFileCommand>();
    auto mkrand = std::make_unique<BasicCommands::CreateRandomElementsCommand>();
    auto save = std::make_unique<BasicCommands::SaveProjectCommand>();
    auto load = std::make_unique<BasicCommands::LoadProjectCommand>();


    saveCommand("cd", std::move(cd));
    saveCommand("ls", std::move(ls));
    saveCommand("mkdir", std::move(mkdir));
    saveCommand("touch", std::move(touch));
    saveCommand("cat", std::move(cat));
    saveCommand("rm", std::move(rm));
    saveCommand("rmdir", std::move(rmdir));
    saveCommand("cp", std::move(cp));
    saveCommand("mv", std::move(mv));
    saveCommand("chmod", std::move(chmod));
    saveCommand("chown", std::move(chown));
    saveCommand("stat", std::move(stat));
    saveCommand("find", std::move(find));
    saveCommand("useradd", std::move(useradd));
    saveCommand("groupadd", std::move(groupadd));
    saveCommand("usermod", std::move(usermod));
    saveCommand("groupdel", std::move(groupdel));
    saveCommand("userdel", std::move(userdel));
    saveCommand("edit", std::move(edit));
    saveCommand("mkrand", std::move(mkrand));
    saveCommand("save", std::move(save));
    saveCommand("load", std::move(load));
}

bool CommandRepository::saveCommand(const std::string& name, std::unique_ptr<ICommand> command) {
    if (!command) return false;
    commandStorage[name] = std::move(command);
    return true;
}

bool CommandRepository::deleteCommand(const std::string& name) {
    auto it = commandStorage.find(name);
    if (it == commandStorage.end()) return false;
    commandStorage.erase(it);
    return true;
}

ICommand* CommandRepository::getCommand(const std::string& name) const {
    auto it = commandStorage.find(name);
    if (it == commandStorage.end()) return nullptr;
    return it->second.get();
}

bool CommandRepository::commandExists(const std::string& name) const {
    return commandStorage.find(name) != commandStorage.end();
}

std::vector<std::string> CommandRepository::getCommandNames() const {
    std::vector<std::string> names;
    for (const auto& [name, cmd] : commandStorage) {
        names.push_back(name);
    }
    return names;
}

bool CommandRepository::createCompositeCommand(const std::string& name, const std::string& description) {
    auto composite = std::make_unique<CompositeCommand>(name, description);
    return saveCommand(name, std::move(composite));
}

std::map<std::string, ICommand*> CommandRepository::getAllCommands() const {
    std::map<std::string, ICommand*> result;
    for (const auto& [name, cmd] : commandStorage) {
        result[name] = cmd.get();
    }
    return result;
}