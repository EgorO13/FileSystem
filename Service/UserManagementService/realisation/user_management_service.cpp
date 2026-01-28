#include "user_management_service.h"
#include "../../../Entity/Directory/realisation/directory_descriptor.h"
#include "iostream"
UserManagementService::UserManagementService(IUserRepository& userRepo, IGroupRepository& groupRepo, ISecurityService& secService)
    : userRepository(userRepo), groupRepository(groupRepo), securityService(secService) {
}

bool UserManagementService::validateUsername(const std::string& username) const {
    if (username.empty()) return false;
    if (username.length() > 255) return false;
    if (username.find_first_of("|:;\\/") != std::string::npos) return false;
    return true;
}

bool UserManagementService::validateGroupName(const std::string& groupName) const {
    if (groupName.empty()) return false;
    if (groupName.length() > 255) return false;
    if (groupName.find_first_of("|:;\\/") != std::string::npos) return false;
    return true;
}

bool UserManagementService::createUser(const std::string& username, IDirectory* root, bool isAdmin) {
    if (!validateUsername(username)) return false;
    if (userRepository.userExists(username)) return false;
    auto user = std::make_unique<User>(userRepository.getNextId(), username);
    User* userPtr = user.get();
    if (!userRepository.saveUser(std::move(user))) return false;
    addUserToGroup(username, "All");
    if (isAdmin) addUserToGroup(username, "Administrators");
    if (root) {
        IFileSystemObject* rootFsObj = dynamic_cast<IFileSystemObject*>(root);
        if (rootFsObj) {
            std::vector<PermissionType> perm = {PermissionType::Read, PermissionType::Write, PermissionType::Execute};
            rootFsObj->setPermissions(userPtr->getId(), SubjectType::User, perm, PermissionEffect::Allow);
        }
    }
    return true;
}

bool UserManagementService::deleteUser(const std::string& username) {
    User* user = userRepository.getUserByName(username);
    if (!user) return false;
    return userRepository.deleteUser(user->getId());
}

bool UserManagementService::modifyUser(const std::string& username, const std::string& newUsername) {
    User* user = userRepository.getUserByName(username);
    if (!user) return false;
    if (newUsername.empty()) return true;
    if (!validateUsername(newUsername)) return false;
    if (userRepository.userExists(newUsername)) return false;
    std::vector<unsigned int> groupIds = user->getGroups();
    unsigned int userId = user->getId();
    auto updatedUser = std::make_unique<User>(*user);
    updatedUser->setName(newUsername);
    for (auto groupId : groupIds) {
        updatedUser->addToGroup(groupId);
    }
    if (!userRepository.deleteUser(userId)) return false;
    if (!userRepository.saveUser(std::move(updatedUser))) return false;
    for (auto groupId : groupIds) {
        groupRepository.addUserToGroup(userId, groupId);
    }
    return true;
}

User* UserManagementService::getUser(const std::string& username) {
    return userRepository.getUserByName(username);
}

std::vector<User*> UserManagementService::getAllUsers() {
    return userRepository.getAllUsers();
}

bool UserManagementService::userExists(const std::string& username) const {
    return userRepository.userExists(username);
}

bool UserManagementService::createGroup(const std::string& groupName) {
    if (!validateGroupName(groupName)) return false;
    if (groupRepository.groupExists(groupName)) return false;

    auto group = std::make_unique<Group>(groupRepository.getNextId(), groupName);
    return groupRepository.saveGroup(std::move(group));
}

bool UserManagementService::deleteGroup(const std::string& groupName) {
    Group* group = groupRepository.getGroupByName(groupName);
    if (!group) return false;
    if (groupName == "Administrators" || groupName == "All") return false;
    return groupRepository.deleteGroup(group->getId());
}

Group* UserManagementService::getGroup(const std::string& groupName) {
    return groupRepository.getGroupByName(groupName);
}

std::vector<Group*> UserManagementService::getAllGroups() {
    return groupRepository.getAllGroups();
}

bool UserManagementService::groupExists(const std::string& groupName) const {
    return groupRepository.groupExists(groupName);
}

bool UserManagementService::addUserToGroup(const std::string& username, const std::string& groupName) {
    User* user = userRepository.getUserByName(username);
    if (!user) return false;
    Group* group = groupRepository.getGroupByName(groupName);
    if (!group) return false;
    user->addToGroup(group->getId());
    group->addUser(user->getId());
    return groupRepository.addUserToGroup(user->getId(), group->getId());
}

bool UserManagementService::removeUserFromGroup(const std::string& username, const std::string& groupName) {
    User* user = userRepository.getUserByName(username);
    if (!user) return false;

    Group* group = groupRepository.getGroupByName(groupName);
    if (!group) return false;

    return groupRepository.removeUserFromGroup(user->getId(), group->getId());
}

std::vector<std::string> UserManagementService::getUserGroups(const std::string& username) {
    User* user = userRepository.getUserByName(username);
    if (!user) return {};

    std::vector<unsigned int> groupIds = user->getGroups();
    std::vector<std::string> groupNames;

    for (unsigned int id : groupIds) {
        Group* group = groupRepository.getGroupById(id);
        if (group) {
            groupNames.push_back(group->getName());
        }
    }

    return groupNames;
}

bool UserManagementService::isUserInGroup(const std::string& username, const std::string& groupName) {
    User* user = userRepository.getUserByName(username);
    if (!user) return false;

    Group* group = groupRepository.getGroupByName(groupName);
    if (!group) return false;

    return groupRepository.isUserInGroupRecursive(user->getId(), group->getId());
}