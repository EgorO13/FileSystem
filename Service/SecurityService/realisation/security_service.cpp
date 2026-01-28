#include "security_service.h"
#include <algorithm>

SecurityService::SecurityService(IUserRepository& userRepo, IGroupRepository& groupRepo)
    : userRepository(userRepo), groupRepository(groupRepo) {}

std::vector<unsigned int> SecurityService::getUserGroupIds(const User& user) const {
    std::vector<unsigned int> groupIds = user.getGroups();
    std::vector<unsigned int> allGroupIds = groupIds;
    for (unsigned int groupId : groupIds) {
        std::vector<unsigned int> parentGroups = groupRepository.getAllParentGroups(groupId);
        allGroupIds.insert(allGroupIds.end(), parentGroups.begin(), parentGroups.end());
    }
    std::sort(allGroupIds.begin(), allGroupIds.end());
    allGroupIds.erase(std::unique(allGroupIds.begin(), allGroupIds.end()), allGroupIds.end());
    return allGroupIds;
}

bool SecurityService::checkExplicitPermission(const User& user, const IFileSystemObject& object, PermissionType permission) const {
    std::vector<unsigned int> groupIds = getUserGroupIds(user);
    return object.checkPermission(user.getId(), groupIds, permission);
}

bool SecurityService::checkPermission(const User& user, const IFileSystemObject& object, PermissionType permission) {
    if (isOwner(user, object)) {
        std::vector<unsigned int> groupIds = getUserGroupIds(user);
        return object.checkPermission(user.getId(), groupIds, permission);
    }
    return checkExplicitPermission(user, object, permission);
}

std::map<PermissionType, bool> SecurityService::getEffectivePermissions(const User& user, const IFileSystemObject& object) {
    std::map<PermissionType, bool> result;
    result[PermissionType::Read] = canRead(user, object);
    result[PermissionType::Write] = canWrite(user, object);
    result[PermissionType::Execute] = canExecute(user, object);
    result[PermissionType::Modify] = canModify(user, object);
    result[PermissionType::ModifyMetadata] = canModifyMetadata(user, object);
    result[PermissionType::ChangePermissions] = canChangePermissions(user, object);
    return result;
}

bool SecurityService::canRead(const User& user, const IFileSystemObject& object) {
    return checkPermission(user, object, PermissionType::Read);
}

bool SecurityService::canWrite(const User& user, const IFileSystemObject& object) {
    return checkPermission(user, object, PermissionType::Write);
}

bool SecurityService::canExecute(const User& user, const IFileSystemObject& object) {
    return checkPermission(user, object, PermissionType::Execute);
}

bool SecurityService::canModify(const User& user, const IFileSystemObject& object) {
    return checkPermission(user, object, PermissionType::Modify);
}

bool SecurityService::canModifyMetadata(const User& user, const IFileSystemObject& object) {
    return checkPermission(user, object, PermissionType::ModifyMetadata);
}

bool SecurityService::canChangePermissions(const User& user, const IFileSystemObject& object) {
    return checkPermission(user, object, PermissionType::ChangePermissions);
}

User* SecurityService::authenticate(const std::string& username) {
    return userRepository.getUserByName(username);
}

bool SecurityService::isAdministrator(const User& user) {
    Group* adminGroup = groupRepository.getGroupByName(ADMIN_GROUP_NAME);
    if (adminGroup == nullptr) return false;
    return groupRepository.isUserInGroupRecursive(user.getId(), adminGroup->getId());
}

bool SecurityService::isOwner(const User& user, const IFileSystemObject& object) {
    return object.getOwner().getId() == user.getId();
}