#include "Command/Commands/realisation/Basic/basic_commands.h"
#include "Service/FSService/realisation/fs_service.h"
#include "Service/SecurityService/realisation/security_service.h"
#include "FileSystem/interface/i_file_system.h"
#include <random>
#include <chrono>


using namespace BasicCommands;

// ========================================
ChangeDirectoryCommand::ChangeDirectoryCommand()
    : BaseCommand("cd", "Change current directory", "cd <path>") {}

bool ChangeDirectoryCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult ChangeDirectoryCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.changeDirectory(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
ListDirectoryCommand::ListDirectoryCommand()
    : BaseCommand("ls", "List directory contents", "ls [path]") {}

bool ListDirectoryCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() <= 1;
}

CommandResult ListDirectoryCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    std::string path = args.empty() ? "" : args[0];
    auto result = fs.listDirectory(path);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
CreateFileCommand::CreateFileCommand()
    : BaseCommand("touch", "Create empty file", "touch <path> [content]") {}

bool CreateFileCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() >= 1 && args.size() <= 2;
}

CommandResult CreateFileCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    std::string content = args.size() > 1 ? args[1] : "";
    auto result = fs.createFile(args[0], content);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
MakeDirectoryCommand::MakeDirectoryCommand()
    : BaseCommand("mkdir", "Create directory", "mkdir <path>") {}

bool MakeDirectoryCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult MakeDirectoryCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.createDirectory(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
DeleteDirectoryCommand::DeleteDirectoryCommand()
    : BaseCommand("rmdir", "Delete Directory", "rmdir <path> [-r = recursive]") {}

bool DeleteDirectoryCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1 || (args.size() == 2 && args[1] == "-r");
}

CommandResult DeleteDirectoryCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.deleteDirectory(args[0], args.size() == 2);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
ReadFileCommand::ReadFileCommand()
    : BaseCommand("cat", "Read file content", "cat <path>") {}

bool ReadFileCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult ReadFileCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.readFile(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
DeleteFileCommand::DeleteFileCommand()
    : BaseCommand("rm", "Delete file", "rm <path>") {}

bool DeleteFileCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult DeleteFileCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.deleteFile(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
CopyCommand::CopyCommand()
    : BaseCommand("cp", "Copy file or directory", "cp <source> <destination>") {}

bool CopyCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 2;
}

CommandResult CopyCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.copyFile(args[0], args[1]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
MoveCommand::MoveCommand()
    : BaseCommand("mv", "Move file or directory", "mv <source> <destination>") {}

bool MoveCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 2;
}

CommandResult MoveCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.moveFile(args[0], args[1]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
ChangePermissionsCommand::ChangePermissionsCommand()
    : BaseCommand("chmod", "Change file permissions", "chmod <path> <permissions> [-a for all]") {}

bool ChangePermissionsCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 2 || args.size() == 3;
}

static std::map<PermissionType, PermissionEffect> parsePermissions(const std::string& permStr) {
    std::map<PermissionType, PermissionEffect> perms;
    for (size_t i = 0; i < permStr.length(); ++i) {
        char c = permStr[i];
        PermissionType type = PermissionType::Read;
        PermissionEffect effect = PermissionEffect::Allow;
        switch (c) {
            case 'r': type = PermissionType::Read; break;
            case 'w': type = PermissionType::Write; break;
            case 'x': type = PermissionType::Execute; break;
            case 'm': type = PermissionType::Modify; break;
            case 'd': type = PermissionType::ModifyMetadata; break;
            case 'c': type = PermissionType::ChangePermissions; break;
            default: continue;
        }
        if (i + 1 < permStr.length()) {
            char next = permStr[i + 1];
            if (next == '+') effect = PermissionEffect::Allow;
            else if (next == '-') effect = PermissionEffect::Deny;
            i++;
        }
        perms[type] = effect;
    }
    return perms;
}

CommandResult ChangePermissionsCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto permissions = parsePermissions(args[1]);
    if (permissions.empty()) return CommandResult{false, {}, "Invalid permissions format. Use format like 'r+w-xm+d-c'"};
    auto result = fs.changePermissions(args[0], permissions, args[2] == "-a");
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
ChangeOwnerCommand::ChangeOwnerCommand()
    : BaseCommand("chown", "Change file owner", "chown <path> <new_owner>") {}

bool ChangeOwnerCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 2;
}

CommandResult ChangeOwnerCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.changeOwner(args[0], args[1]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
StatisticsCommand::StatisticsCommand()
    : BaseCommand("stat", "Show file system statistics",
                  "stat [path] [-n threads] [-i ignore permissions]") {}

bool StatisticsCommand::validateArgs(const std::vector<std::string>& args) const {
    size_t i = 0;
    bool hasPath = false;
    bool hasThreadFlag = false;
    bool hasIgnoreFlag = false;
    while (i < args.size()) {
        if (args[i] == "-n") {
            if (hasThreadFlag || i + 1 >= args.size()) return false;
            try {
                int threads = std::stoi(args[i + 1]);
                if (threads <= 0) return false;
                i++;
                hasThreadFlag = true;
            } catch (...) {
                return false;
            }
        } else if (args[i] == "-i" || args[i] == "--ignore-permissions") hasIgnoreFlag = true;
        else if (hasPath) return false;
        else hasPath = true;
        i++;
    }
    return true;
}

CommandResult StatisticsCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    std::string path;
    int threadCount = std::thread::hardware_concurrency();
    bool ignorePermissions = false;
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-n") {
            if (i + 1 < args.size()) {
                try {
                    threadCount = std::stoi(args[i + 1]);
                    if (threadCount <= 0) return CommandResult{false, {}, "Thread count must be positive"};
                    i++;
                } catch (...) {
                    return CommandResult{false, {}, "Invalid thread count: " + args[i + 1]};
                }
            }
            else return CommandResult{false, {}, "Missing thread count after -n"};
        }
        else if (args[i] == "-i" || args[i] == "--ignore-permissions") ignorePermissions = true;
        else path = args[i];
    }

    if (ignorePermissions) {
        User* user = fs.getCurrentUser();
        if (!user || !fs.getSecurityService().isAdministrator(*user)) {
            return CommandResult{false, {}, "Admin rights required for -i flag"};
        }
    }

    auto result = fs.getStatistics(threadCount, ignorePermissions);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
FindCommand::FindCommand()
    : BaseCommand("find", "Find files by pattern", "find <pattern> [start_path]") {}

bool FindCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() >= 1 && args.size() <= 2;
}

CommandResult FindCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    std::string startPath = args.size() > 1 ? args[1] : "";
    auto result = fs.find(args[0], startPath);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
CreateUserCommand::CreateUserCommand()
    : BaseCommand("useradd", "Create new user", "useradd <username> [--admin]", true) {}

bool CreateUserCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() >= 1 && args.size() <= 2;
}

CommandResult CreateUserCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    bool isAdmin = false;

    if (args.size() > 1) {
        if (args[1] == "--admin" || args[1] == "-a") {
            isAdmin = true;
        } else {
            return CommandResult{false, {}, "Invalid option. Use --admin or -a"};
        }
    }

    auto result = fs.createUser(args[0], isAdmin);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
CreateGroupCommand::CreateGroupCommand()
    : BaseCommand("groupadd", "Create new group", "groupadd <groupname>", true) {}

bool CreateGroupCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult CreateGroupCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.createGroup(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
AddUserToGroupCommand::AddUserToGroupCommand()
    : BaseCommand("usermod", "Add user to group", "usermod <username> <groupname>", true) {}

bool AddUserToGroupCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 2;
}

CommandResult AddUserToGroupCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.addUserToGroup(args[0], args[1]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
DeleteGroupCommand::DeleteGroupCommand()
    : BaseCommand("groupdel", "Delete group", "groupdel <groupname>", true) {}

bool DeleteGroupCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult DeleteGroupCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    std::string groupName = args[0];
    if (groupName == "Administrators") {
        return CommandResult{false, {}, "Cannot delete Administrators group"};
    }
    if (fs.getGroup(groupName)->getId() == 1) return CommandResult{false, {}, "Cannot delete Administrators"};
    auto result = fs.deleteGroup(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
DeleteUserCommand::DeleteUserCommand()
    : BaseCommand("userdel", "Delete user", "userdel <username>", true) {}

bool DeleteUserCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult DeleteUserCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    std::string userName = args[0];
    if (fs.getCurrentUser() == fs.getUser(userName)) return CommandResult{false, {}, "Cannot delete current user"};
    if (userName == "Administrator") return CommandResult{false, {}, "Cannot delete Administrator user"};
    auto result = fs.deleteUser(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

// ========================================
EditFileCommand::EditFileCommand()
    : BaseCommand("edit", "Edit file content", "edit <path> [-a append] [content]") {}

bool EditFileCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() >= 1;
}

CommandResult EditFileCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    std::string path = args[0];
    bool append = false;
    std::string content;
    for (size_t i = 1; i < args.size(); i++) {
        if (args[i] == "-a") append = true;
        else {
            if (!content.empty()) content += " ";
            content += args[i];
        }
    }
    if (append) {
        auto readResult = fs.readFile(path);
        if (!readResult.success) return CommandResult{false, {}, "Cannot read file: " + readResult.error};
        std::string existingContent;
        if (!readResult.messages.empty()) existingContent = readResult.messages[0];
        auto writeResult = fs.writeFile(path, existingContent + content);
        return CommandResult{writeResult.success, writeResult.messages, writeResult.error};
    }
    auto writeResult = fs.writeFile(path, content);
    return CommandResult{writeResult.success, writeResult.messages, writeResult.error};
}

CreateRandomElementsCommand::CreateRandomElementsCommand()
    : BaseCommand("mkrand", "Create N random files and directories", "mkrand <N>", true) {}

bool CreateRandomElementsCommand::validateArgs(const std::vector<std::string>& args) const {
    if (args.size() != 1) return false;
    try {
        int n = std::stoi(args[0]);
        return n > 0;
    } catch (...) {
        return false;
    }
}

CommandResult CreateRandomElementsCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    try {
        int n = std::stoi(args[0]);
        return fs.createRandomElements(n);
    } catch (const std::exception& e) {
        return CommandResult(false, {}, std::string("Error: ") + e.what());
    }
}

SaveProjectCommand::SaveProjectCommand()
    : BaseCommand("save", "Save entire file system to file", "save <filename>", true) {}

bool SaveProjectCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult SaveProjectCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.saveProject(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}

LoadProjectCommand::LoadProjectCommand()
    : BaseCommand("load", "Load file system from file", "load <filename>", true) {}

bool LoadProjectCommand::validateArgs(const std::vector<std::string>& args) const {
    return args.size() == 1;
}

CommandResult LoadProjectCommand::execute(const std::vector<std::string>& args, IFileSystem& fs) {
    auto result = fs.loadProject(args[0]);
    return CommandResult{result.success, result.messages, result.error};
}