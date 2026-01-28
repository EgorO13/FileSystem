#include "controller.h"
#include "Entity/User/user.h"
#include "../../Command/CommandService/realisation/command_service.h"
#include <sstream>
#include <algorithm>
#include <cctype>

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

Controller::Controller(std::unique_ptr<ILoader> loader) : isRun(true), view(loader->getView()), commandService(loader->getCommandService()), fileSystem(std::make_unique<FileSystem>(std::move(loader))) {
    initializeControllerCommands();
}

void Controller::trim(std::string& str) {
    ltrim(str);
    rtrim(str);
}

bool Controller::splitArguments(const std::string& input, std::vector<std::string>& args) {
    std::stringstream ss(input);
    std::string token;
    bool inQuotes = false;
    std::string currentArg;
    for (char c : input) {
        if (c == '"') inQuotes = !inQuotes;
        else if (c == ' ' && !inQuotes && !currentArg.empty()) {
            args.push_back(currentArg);
            currentArg.clear();
        }
        else currentArg += c;
    }
    if (!currentArg.empty()) args.push_back(currentArg);
    return true;
}

void Controller::parseCommand(const std::string& input, std::string& command, std::vector<std::string>& args) {
    command.clear();
    args.clear();
    std::string trimmedInput = input;
    trim(trimmedInput);
    if (trimmedInput.empty()) return;
    splitArguments(trimmedInput, args);
    if (!args.empty()) {
        command = args[0];
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        args.erase(args.begin());
    }
}

void Controller::initializeControllerCommands() {
    controllerCommands["help"] = [this](const std::vector<std::string>& args) { cmdHelp(args); };
    controllerCommands["exit"] = [this](const std::vector<std::string>& args) { cmdExit(args); };
    controllerCommands["quit"] = [this](const std::vector<std::string>& args) { cmdExit(args); };
    controllerCommands["logout"] = [this](const std::vector<std::string>& args) { cmdLogout(args); };
    controllerCommands["login"] = [this](const std::vector<std::string>& args) { cmdLogin(args); };
    controllerCommands["whoami"] = [this](const std::vector<std::string>& args) { cmdWhoami(args); };
    controllerCommands["pwd"] = [this](const std::vector<std::string>& args) { cmdPwd(args); };
    controllerCommands["man"] = [this](const std::vector<std::string>& args) { cmdMan(args); };
    controllerCommands["composite"] = [this](const std::vector<std::string>& args) { cmdComposite(args); };
}

void Controller::cmdHelp(const std::vector<std::string>& args) {
    std::vector<std::string> helpLines;

    helpLines.push_back("=== File System Commands ===");
    helpLines.push_back("  cd <path>                                   - Change directory");
    helpLines.push_back("  ls [path]                                   - List directory contents");
    helpLines.push_back("  man [cmd]                                   - Command manual");
    helpLines.push_back("  mkdir <path>                                - Create directory");
    helpLines.push_back("  touch <path>                                - Create file");
    helpLines.push_back("  cat <path>                                  - Read file");
    helpLines.push_back("  edit <path> <content> [--append]            - Read file");
    helpLines.push_back("  rm <path>                                   - Delete file");
    helpLines.push_back("  rmdir <path> [rec]                          - Delete file");
    helpLines.push_back("  cp <src> <dest>                             - Copy file");
    helpLines.push_back("  mv <src> <dest>                             - Move file");
    helpLines.push_back("  chmod <path> <perms>                        - Change permissions");
    helpLines.push_back("  chown <path> <owner>                        - Change owner");
    helpLines.push_back("  find <pattern>                              - Find files");
    helpLines.push_back("  mkrand <N>                                   - Create N random files and directories");
    helpLines.push_back("  stat <path>                                 - File statistics");
    helpLines.push_back("  save <filename>                             - Save entire filesystem state");
    helpLines.push_back("  load <filename>                             - Load filesystem state from file");
    helpLines.push_back("  composite create/add/remove/show/delete     - Read file");

    helpLines.push_back("");
    helpLines.push_back("=== User Management Commands (Admin only) ===");
    helpLines.push_back("  useradd <username>                          - Create user");
    helpLines.push_back("  groupadd <group>                            - Create group");
    helpLines.push_back("  userdel <username>                          - Delete user");
    helpLines.push_back("  groupdel <group>                            - Delete group");
    helpLines.push_back("  usermod <user> <group>                      - Add user to group");

    helpLines.push_back("");
    helpLines.push_back("=== System Commands ===");
    helpLines.push_back("  login <username>                            - Login");
    helpLines.push_back("  logout                                      - Logout");
    helpLines.push_back("  whoami                                      - Show current user");
    helpLines.push_back("  pwd                                         - Show current path");
    helpLines.push_back("  help                                        - This help");
    helpLines.push_back("  exit/quit                                   - Exit program");

    view.displayMessage("Available commands:");
    for (const auto& line : helpLines) {
        view.displayMessage(line);
    }
}

void Controller::cmdExit(const std::vector<std::string>& args) {
    view.displayMessage("Exiting...");
    stop();
}

void Controller::cmdLogin(const std::vector<std::string>& args) {
    if (args.empty()) {
        view.displayError("Usage: login <username>");
        return;
    }
    auto result = fileSystem->login(args[0]);
    if (result.success) {
        for (const auto& msg : result.messages) {
            view.displayMessage(msg);
        }
        view.displayMessage("Current directory: " + fileSystem->getCurrentPath());
    } else view.displayError(result.error);
}

void Controller::cmdLogout(const std::vector<std::string>& args) {
    fileSystem->logout();
    view.displayMessage("Logged out");
}

void Controller::cmdWhoami(const std::vector<std::string>& args) {
    if (!fileSystem->isLoggedIn()) {
        view.displayMessage("Not logged in");
        return;
    }
    auto user = fileSystem->getCurrentUser();
    view.displayMessage("Current user: " + user->getName());
}

void Controller::cmdPwd(const std::vector<std::string>& args) {
    view.displayMessage("Current path: " + fileSystem->getCurrentPath());
}

void Controller::cmdMan(const std::vector<std::string>& args) {
    if (!fileSystem->isLoggedIn()) {
        view.displayMessage("Not logged in");
        return;
    }
    ICommand* cmd = commandService.getCommand(args[0]);
    if (!cmd) view.displayMessage("No manual entry");
    else view.displayMessage("Descriotion: " + cmd->getDescription() + "\nUsage: " + cmd->getUsage());
}

void Controller::executeFSCommand(const std::string& command, const std::vector<std::string>& args) {
    CommandResult result = commandService.executeCommand(command, args, *fileSystem);
    if (result.success) {
        for (const auto& msg : result.message) {
            view.displayMessage(msg);
        }
    } else view.displayError(result.error);
}

void Controller::showWelcomeMessage() {
    view.displayMessage("=== File System Console ===");
    view.displayMessage("Type 'help' for list of commands");
    view.displayMessage("Type 'login <username>' to start");
    view.displayMessage("Default admin: Administrator");
    view.displayMessage("");
}

void Controller::run() {
    view.showWelcome();
    while (isRun) {
        std::string prompt;
        if (fileSystem->isLoggedIn()) {
            auto user = fileSystem->getCurrentUser();
            prompt = user->getName();
            prompt += ":" + fileSystem->getCurrentPath() + "> ";
        }
        else prompt = "login> ";
        std::string input = view.getInput(prompt);
        std::string command;
        std::vector<std::string> args;
        parseCommand(input, command, args);
        if (command.empty()) continue;
        try {
            auto it = controllerCommands.find(command);
            if (it != controllerCommands.end()) it->second(args);
            else executeFSCommand(command, args);
        }
        catch (const std::exception& e) { view.displayError(std::string(e.what())); }
        catch (...) { view.displayError("Unknown error"); }
    }
    view.showGoodbye();
}

std::vector<std::string> Controller::getAvailableCommands() const {
    std::vector<std::string> commands;
    for (const auto& [cmd, _] : controllerCommands) {
        commands.push_back(cmd);
    }
    return commands;
}

void Controller::cmdComposite(const std::vector<std::string>& args) {
    if (args.empty()) {
        view.displayError("Usage: composite <action> [args...]");
        return;
    }
    const std::string& action = args[0];
    try {
        if (action == "create" && args.size() >= 3) {
            std::string name = args[1];
            std::string description = args[2];
            if (commandService.createCompositeCommand(name, description)) view.displayMessage("Composite command created: " + name);
            else view.displayError("Failed to create composite command");
        }
        else if (action == "add" && args.size() >= 3) {
            std::string compositeName = args[1];
            std::string commandName = args[2];
            std::vector<std::string> fixedArgs;
            std::vector<int> dynamicIndices;
            for (size_t i = 3; i < args.size(); i++) {
                if (args[i].size() > 1 && args[i][0] == '$') {
                    try {
                        int index = std::stoi(args[i].substr(1));
                        dynamicIndices.push_back(index);
                    } catch (...) {
                        fixedArgs.push_back(args[i]);
                    }
                } else fixedArgs.push_back(args[i]);
            }
            if (commandService.addToComposite(compositeName, commandName, fixedArgs, dynamicIndices)) view.displayMessage("Command added to composite");
            else view.displayError("Failed to add command to composite");
        }
        else if (action == "remove" && args.size() == 3) {
            std::string compositeName = args[1];
            size_t index;
            try {
                index = std::stoul(args[2]) - 1;
            } catch (...) {
                view.displayError("Invalid index: " + args[2]);
                return;
            }
            if (commandService.removeFromComposite(compositeName, index)) view.displayMessage("Command removed from composite");
            else view.displayError("Failed to remove command from composite");
        }
        else if (action == "list" && args.size() == 1) {
            auto composites = commandService.getCompositeCommands();
            if (composites.empty()) view.displayMessage("No composite commands found");
            else {
                view.displayMessage("Composite commands:");
                for (const auto& name : composites) {
                    view.displayMessage("  " + name);
                }
            }
        }
        else if (action == "show" && args.size() == 2) {
            std::string name = args[1];
            auto info = commandService.getCompositeInfo(name);
            view.displayMessage(info);
        }
        else if (action == "delete" && args.size() == 2) {
            std::string name = args[1];
            if (commandService.deleteCompositeCommand(name)) view.displayMessage("Composite command deleted: " + name);
            else view.displayError("Failed to delete composite command");
        }
        else view.displayError("Unknown composite action: " + action);
    } catch (const std::exception& e) {
        view.displayError(std::string("Error: ") + e.what());
    }
}