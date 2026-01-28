#include "file_system.h"
#include "../../Threads/Statistics/fs_stat.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include <algorithm>

FileSystem::FileSystem(std::unique_ptr<ILoader> loader) : loader_(std::move(loader)) {
    if (!loader_) throw std::runtime_error("Loader cannot be null");
    createDefaultData();
}

void FileSystem::createDefaultData() {
    auto& userManagementService = loader_->getUserManagementService();
    auto& fsRepository = loader_->getFsRepository();
    userManagementService.createUser("Administrator", fsRepository.getRootDirectory(), true);
    userManagementService.createGroup("Administrators");
    userManagementService.addUserToGroup("Administrator", "Administrators");
}

FileSystemResult FileSystem::login(const std::string& username) {
    auto& sessionService = loader_->getSessionService();
    if (sessionService.login(username)) return FileSystemResult{true, {"Logged in as " + username}};
    return FileSystemResult{false, {}, "Login failed"};
}

void FileSystem::logout() {
    auto& sessionService = loader_->getSessionService();
    sessionService.logout();
}

bool FileSystem::isLoggedIn() const {
    auto& sessionService = loader_->getSessionService();
    return sessionService.isLoggedIn();
}

User* FileSystem::getCurrentUser() const {
    auto& sessionService = loader_->getSessionService();
    return sessionService.getCurrentUser();
}

ISecurityService& FileSystem::getSecurityService() const {
    return loader_->getSecurityService();
}

std::string FileSystem::getCurrentPath() const {
    auto& sessionService = loader_->getSessionService();
    auto currentDir = sessionService.getCurrentDirectory();
    if (currentDir) {
        auto& fsRepository = loader_->getFsRepository();
        return fsRepository.getPath(dynamic_cast<IFileSystemObject*>(currentDir));
    }
    return "/";
}

FileSystemResult FileSystem::createFile(const std::string& path, const std::string& content) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    IFile* file = fsService.createFile(*user, path, content);
    if (file) return FileSystemResult{true, {"File created: " + path}};
    return FileSystemResult{false, {}, "Failed to create file"};
}

FileSystemResult FileSystem::readFile(const std::string& path) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    std::string content = fsService.readFile(*user, path);
    if (!content.empty() || fsService.exists(path)) {
        return FileSystemResult{true, {content}};
    }
    return FileSystemResult{false, {}, "Failed to read file"};
}

FileSystemResult FileSystem::writeFile(const std::string& path, const std::string& content) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    if (fsService.writeFile(*user, path, content, false)) {
        return FileSystemResult{true, {"File written: " + path}};
    }
    return FileSystemResult{false, {}, "Failed to write file"};
}

FileSystemResult FileSystem::deleteFile(const std::string& path) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    if (fsService.deleteFile(*user, path)) {
        return FileSystemResult{true, {"File deleted: " + path}};
    }
    return FileSystemResult{false, {}, "Failed to delete file"};
}

FileSystemResult FileSystem::copyFile(const std::string& source, const std::string& dest) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    if (fsService.copyFile(*user, source, dest)) {
        return FileSystemResult{true, {"File copied from " + source + " to " + dest}};
    }
    return FileSystemResult{false, {}, "Failed to copy file"};
}

FileSystemResult FileSystem::moveFile(const std::string& source, const std::string& dest) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    if (fsService.moveFile(*user, source, dest)) {
        return FileSystemResult{true, {"File moved from " + source + " to " + dest}};
    }
    return FileSystemResult{false, {}, "Failed to move file"};
}

FileSystemResult FileSystem::createDirectory(const std::string& path) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    IDirectory* dir = fsService.createDirectory(*user, path);
    if (dir) return FileSystemResult{true, {"Directory created: " + path}};
    return FileSystemResult{false, {}, "Failed to create directory"};
}

FileSystemResult FileSystem::deleteDirectory(const std::string& path, bool recursive) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    if (fsService.deleteDirectory(*user, path, recursive)) {
        return FileSystemResult{true, {"Directory deleted: " + path}};
    }
    return FileSystemResult{false, {}, "Failed to delete directory"};
}

FileSystemResult FileSystem::listDirectory(const std::string& path) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    auto& fsRepository = loader_->getFsRepository();
    if (!fsRepository.getDirectoryByPath(path)) {
        return FileSystemResult{false, {}, "No directory"};
    }
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    auto fileInfos = fsService.listDirectory(*user, path);
    std::vector<std::string> messages;
    messages.push_back("Contents of " + (path.empty() ? getCurrentPath() : path) + ":");
    for (const auto& info : fileInfos) {
        messages.push_back(info.type + " " + info.name);
    }
    return FileSystemResult{true, messages};
}

FileSystemResult FileSystem::changeDirectory(const std::string& path) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& sessionService = loader_->getSessionService();
    auto& fsService = loader_->getFsService();
    IDirectory* currentDir = sessionService.getCurrentDirectory();
    IDirectory* newDir = fsService.changeDirectory(*user, path, currentDir);
    if (newDir) {
        sessionService.setCurrentDirectory(newDir);
        return FileSystemResult{true, {"Changed directory to: " + getCurrentPath()}};
    }
    return FileSystemResult{false, {}, "Failed to change directory"};
}

FileSystemResult FileSystem::changePermissions(const std::string& path, const std::map<PermissionType, PermissionEffect>& perms, bool forAll) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    auto& fsService = loader_->getFsService();

    if (forAll) {
        if (fsService.changePermissions(0, SubjectType::Group, path, perms)) {
            std::stringstream ss;
            ss << "Permissions changed for: " << path;
            return FileSystemResult{true, {ss.str()}};
        }
        return FileSystemResult{false, {}, "Failed to change permissions"};
    }

    User* user = getCurrentUser();
    if (fsService.changePermissions(user->getId(), SubjectType::User, path, perms)) {
        std::stringstream ss;
        ss << "Permissions changed for: " << path;
        return FileSystemResult{true, {ss.str()}};
    }
    return FileSystemResult{false, {}, "Failed to change permissions"};
}

FileSystemResult FileSystem::changeOwner(const std::string& path, const std::string& newOwner) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    if (fsService.changeOwner(*user, path, newOwner)) {
        return FileSystemResult{true, {"Owner changed to " + newOwner + " for: " + path}};
    }
    return FileSystemResult{false, {}, "Failed to change owner"};
}

User* FileSystem::getUser(const std::string& username) {
    if (!isLoggedIn()) return nullptr;
    auto& userRepository = loader_->getUserRepository();
    return userRepository.getUserByName(username);
}

FileSystemResult FileSystem::createUser(const std::string& username, bool isAdmin) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    if (!loader_->getSecurityService().isAdministrator(*user)) return FileSystemResult{false, {}, "Admin rights required"};
    auto& userManagementService = loader_->getUserManagementService();
    if (userManagementService.createUser(username, loader_->getFsRepository().getRootDirectory(), isAdmin)) {
        std::string msg = "User created: " + username;
        if (isAdmin) msg += " (admin)";
        return FileSystemResult{true, {msg}};
    }
    return FileSystemResult{false, {}, "Failed to create user"};
}

FileSystemResult FileSystem::deleteUser(const std::string& username) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    if (!loader_->getSecurityService().isAdministrator(*user)) return FileSystemResult{false, {}, "Admin rights required"};
    auto& userManagementService = loader_->getUserManagementService();
    if (userManagementService.deleteUser(username)) return FileSystemResult{true, {"User deleted: " + username}};
    return FileSystemResult{false, {}, "Failed to delete user"};
}

Group* FileSystem::getGroup(const std::string& groupname) {
    if (!isLoggedIn()) return nullptr;
    auto& groupRepository = loader_->getGroupRepository();
    return groupRepository.getGroupByName(groupname);
}

FileSystemResult FileSystem::createGroup(const std::string& groupName) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    if (!loader_->getSecurityService().isAdministrator(*user)) return FileSystemResult{false, {}, "Admin rights required"};
    auto& userManagementService = loader_->getUserManagementService();
    if (userManagementService.createGroup(groupName)) return FileSystemResult{true, {"Group created: " + groupName}};
    return FileSystemResult{false, {}, "Failed to create group"};
}

FileSystemResult FileSystem::deleteGroup(const std::string& groupName) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    if (!loader_->getSecurityService().isAdministrator(*user)) return FileSystemResult{false, {}, "Admin rights required"};
    auto& userManagementService = loader_->getUserManagementService();
    if (userManagementService.deleteGroup(groupName)) return FileSystemResult{true, {"Group deleted: " + groupName}};
    return FileSystemResult{false, {}, "Failed to delete group"};
}

FileSystemResult FileSystem::addUserToGroup(const std::string& username, const std::string& groupName) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    if (!loader_->getSecurityService().isAdministrator(*user)) return FileSystemResult{false, {}, "Admin rights required"};
    auto& userManagementService = loader_->getUserManagementService();
    if (userManagementService.addUserToGroup(username, groupName)) return FileSystemResult{true, {"User " + username + " added to group " + groupName}};
    return FileSystemResult{false, {}, "Failed to add user to group"};
}

FileSystemResult FileSystem::getFileInfo(const std::string& path) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsRepository = loader_->getFsRepository();
    IFileSystemObject* obj = fsRepository.getObjectByPath(path);
    if (!obj) return FileSystemResult{false, {}, "File not found"};
    std::vector<std::string> messages;
    messages.push_back("File info for: " + path);
    messages.push_back("Name: " + obj->getName());
    messages.push_back("Address: " + std::to_string(obj->getAddress()));
    messages.push_back("Owner: " + obj->getOwner().getName());
    auto createTime = obj->getCreateTime();
    auto modifyTime = obj->getLastModifyTime();
    std::time_t createTimeT = std::chrono::system_clock::to_time_t(createTime);
    std::time_t modifyTimeT = std::chrono::system_clock::to_time_t(modifyTime);
    std::tm createTm = *std::localtime(&createTimeT);
    std::tm modifyTm = *std::localtime(&modifyTimeT);
    std::stringstream ssCreate, ssModify;
    ssCreate << std::put_time(&createTm, "%Y-%m-%d %H:%M:%S");
    ssModify << std::put_time(&modifyTm, "%Y-%m-%d %H:%M:%S");
    messages.push_back("Created: " + ssCreate.str());
    messages.push_back("Modified: " + ssModify.str());
    if (auto file = dynamic_cast<IFile*>(obj)) {
        messages.push_back("Type: File");
        messages.push_back("Size: " + std::to_string(file->getSize()) + " bytes");
    } else if (auto dir = dynamic_cast<IDirectory*>(obj)) {
        messages.push_back("Type: Directory");
        messages.push_back("Items: " + std::to_string(dir->getChildCount()));
    }
    auto& securityService = loader_->getSecurityService();
    auto perms = securityService.getEffectivePermissions(*user, *obj);
    std::string permStr = "Permissions: ";
    for (const auto& [perm, allowed] : perms) {
        std::string permName;
        switch (perm) {
            case PermissionType::Read: permName = "R"; break;
            case PermissionType::Write: permName = "W"; break;
            case PermissionType::Execute: permName = "X"; break;
            case PermissionType::Modify: permName = "M"; break;
            case PermissionType::ModifyMetadata: permName = "MM"; break;
            case PermissionType::ChangePermissions: permName = "CP"; break;
        }
        permStr += permName + ":" + (allowed ? "+" : "-") + " ";
    }
    messages.push_back(permStr);
    return FileSystemResult{true, messages};
}

FileSystemResult FileSystem::find(const std::string& pattern, const std::string& startPath) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    auto& fsService = loader_->getFsService();
    auto files = fsService.findFiles(*user, pattern, startPath);
    std::vector<std::string> messages;
    if (files.empty()) {
        messages.push_back("No files found matching pattern: " + pattern);
    } else {
        messages.push_back("Found " + std::to_string(files.size()) + " files:");
        for (const auto& file : files) {
            messages.push_back(file);
        }
    }
    return FileSystemResult{true, messages};
}

FileSystemResult FileSystem::getStatistics(int threadCount, bool ignorePermissions) {
    if (!isLoggedIn() && !ignorePermissions) return FileSystemResult{false, {}, "Not logged in"};
    const User* currentUser = nullptr;
    std::vector<unsigned int> userGroups;
    if (!ignorePermissions) {
        currentUser = getCurrentUser();
        if (!currentUser) return FileSystemResult{false, {}, "Cannot get current user"};
        userGroups = currentUser->getGroups();
    }
    try {
        auto metrics = MetricFactory::createDefaultSet();
        auto& repository = getRepository();
        auto* rootDirectory = dynamic_cast<IDirectory*>(repository.getRootDirectory());
        if (!rootDirectory) return FileSystemResult{false, {}, "Root directory not found"};
        FileSystemScanner scanner(
            threadCount > 0 ? threadCount : 1, repository, loader_->getFsObjectMapper(),
            currentUser, userGroups, ignorePermissions
        );
        auto startTime = std::chrono::steady_clock::now();
        auto allResults = scanner.scan(rootDirectory, metrics);
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::vector<std::string> messages;
        messages.push_back("=== File System Statistics ===");
        messages.push_back("Threads used: " + std::to_string(threadCount > 0 ? threadCount : 1));
        messages.push_back("Mode: " + std::string(ignorePermissions ? "Full access (ignoring permissions)" : "User access"));
        messages.push_back("");
        for (const auto& metricResults : allResults) {
            if (!metricResults.empty()) {
                for (const auto& line : metricResults) {
                    messages.push_back(line);
                }
                messages.push_back("");
            }
        }
        messages.push_back("=============================");
        messages.push_back("Execution time: " + std::to_string(duration.count()) + " ms");
        return FileSystemResult{true, messages};
    } catch (const std::exception& e) {
        return FileSystemResult{false, {}, "Error collecting statistics: " + std::string(e.what())};
    }
}

CommandResult FileSystem::createRandomElements(int count) {
    if (!isLoggedIn()) return CommandResult(false, {}, "Not logged in");
    User* currentUser = loader_->getSessionService().getCurrentUser();
    if (!currentUser) return CommandResult(false, {}, "Cannot get current user");
    Group* adminGroup = loader_->getUserManagementService().getGroup("Administrators");
    if (!adminGroup) return CommandResult(false, {}, "Administrators group not found");
    if (!currentUser->isInGroup(adminGroup->getId())) return CommandResult(false, {}, "Permission denied: admin rights required");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 9);
    std::uniform_int_distribution<> contentLengthDist(10, 100);

    int created = 0;
    int filesCreated = 0;
    int dirsCreated = 0;
    std::vector<unsigned int> directoryAddresses = {0};

    IDirectory* initialDir = loader_->getSessionService().getCurrentDirectory();
    std::string initialPath = getCurrentPath();
    int size = loader_->getFsRepository().getAllObjects().size();
    for (int i = size; i < count + size; i++) {
        std::uniform_int_distribution<> dirDist(0, directoryAddresses.size() - 1);
        unsigned int parentAddress = directoryAddresses[dirDist(gen)];
        IDirectory* parentDir = dynamic_cast<IDirectory*>(loader_->getFsRepository().getObjectByAddress(parentAddress));
        if (!parentDir) continue;
        std::string name = std::to_string(i);
        std::string parentPath = loader_->getFsRepository().getPath(dynamic_cast<IFileSystemObject*>(parentDir));
        int type = typeDist(gen);
        if (type < 8) {
            std::uniform_int_distribution<> contentDist(1, 100);
            std::string content;
            int contentLength = contentLengthDist(gen);
            for (int j = 0; j < contentLength; j++) {
                content += static_cast<char>('a' + contentDist(gen) % 26);
            }
            FileSystemResult result = createFile(parentPath + "/" + name, content);
            if (result.success) {
                created++;
                filesCreated++;
            }
        }
        else {
            FileSystemResult result = createDirectory(parentPath + "/" + name);
            if (result.success) {
                created++;
                dirsCreated++;
                std::string newPath = parentPath + "/" + name;
                IFileSystemObject* newObj = loader_->getFsRepository().getObjectByPath(newPath);
                if (newObj) directoryAddresses.push_back(newObj->getAddress());
            }
        }
    }
    loader_->getSessionService().setCurrentDirectory(initialDir);

    return CommandResult(true,
        {
            "Created " + std::to_string(created) + " random elements out of " + std::to_string(count),
            "Files: " + std::to_string(filesCreated) + " (≈" + std::to_string((filesCreated * 100) / (created > 0 ? created : 1)) + "%)",
            "Directories: " + std::to_string(dirsCreated) + " (≈" + std::to_string((dirsCreated * 100) / (created > 0 ? created : 1)) + "%)"
        },
        "");
}

IFileSystemRepository& FileSystem::getRepository() const {
    return loader_->getFsRepository();
}

FileSystemResult FileSystem::saveProject(const std::string& filename) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    if (!loader_->getSecurityService().isAdministrator(*user)) return FileSystemResult{false, {}, "Admin rights required"};
    try {
        loader_->getFsStateService().save(filename + "_fs.yaml");
        loader_->getCommandStateService().save(filename + "_cmd.yaml");
        loader_->getUserStateService().save(filename + "_users.yaml");
        loader_->getGroupStateService().save(filename + "_groups.yaml");
        return FileSystemResult{true, {"Project saved successfully"}};
    } catch (const std::exception& e) {
        return FileSystemResult{false, {}, "Failed to save project: " + std::string(e.what())};
    } catch (...) {
        return FileSystemResult{false, {}, "Unknown error while saving project"};
    }
}

FileSystemResult FileSystem::loadProject(const std::string& filename) {
    if (!isLoggedIn()) return FileSystemResult{false, {}, "Not logged in"};
    User* user = getCurrentUser();
    if (!loader_->getSecurityService().isAdministrator(*user)) return FileSystemResult{false, {}, "Admin rights required"};
    auto& sessionServ = loader_->getSessionService();
    try {
        loader_->getUserStateService().load(filename + "_users.yaml");
        loader_->getGroupStateService().load(filename + "_groups.yaml");
        loader_->getCommandStateService().load(filename + "_cmd.yaml");
        loader_->getFsStateService().load(filename + "_fs.yaml");
        sessionServ.setCurrentUser(loader_->getUserRepository().getUserById(1));
        sessionServ.setCurrentDirectory(loader_->getFsRepository().getRootDirectory());
        return FileSystemResult{true, {"Project loaded successfully"}};
    } catch (const std::exception& e) {
        return FileSystemResult{false, {}, "Failed to load project: " + std::string(e.what())};
    } catch (...) {
        return FileSystemResult{false, {}, "Unknown error while loading project"};
    }
}