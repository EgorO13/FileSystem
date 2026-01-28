#include "acl_class.h"


void ACL::setPermission(unsigned int id, SubjectType s_type, PermissionType p_type, PermissionEffect effect) {
    ACLKey key = std::make_pair(id, s_type);
    if (entries.find(key) == entries.end()) {
        ACLEntry acl;
        acl.subjectId = id;
        acl.subjectType = s_type;
        acl.setPermission(p_type, effect);
        entries[key] = acl;
    } else entries[key].setPermission(p_type, effect);
}

void ACL::setPermissions(unsigned int id, SubjectType s_type, const std::vector<PermissionType> &p_types, PermissionEffect effect) {
    for (PermissionType p_type: p_types) setPermission(id, s_type, p_type, effect);
}

void ACL::removePermission(unsigned int id, SubjectType s_type, PermissionType p_type) {
    ACLKey key = std::make_pair(id, s_type);
    auto it = entries.find(key);
    if (it != entries.end()) {
        it->second.removePermission(p_type);
        if (it->second.permissions.empty()) entries.erase(it);
    }
}

bool ACL::hasExplicitDeny(unsigned int userId, const std::vector<unsigned int> &userGroups, PermissionType p_type) const {
    ACLKey userKey = std::make_pair(userId, SubjectType::User);
    auto userIt = entries.find(userKey);
    if (userIt != entries.end() && userIt->second.hasPermission(p_type) && userIt->second.getPermissionEffect(p_type) == PermissionEffect::Deny) {
        return true;
    }
    for (unsigned int groupId: userGroups) {
        ACLKey groupKey = std::make_pair(groupId, SubjectType::Group);
        auto groupIt = entries.find(groupKey);
        if (groupIt != entries.end() && groupIt->second.hasPermission(p_type) && groupIt->second.
            getPermissionEffect(p_type) == PermissionEffect::Deny) {
            return true;
        }
    }
    return false;
}

bool ACL::checkPermission(unsigned int userId, const std::vector<unsigned int> &userGroups, PermissionType p_type) const {
    if (userId == ownerId) return !hasExplicitDeny(userId, userGroups, p_type);
    bool hasAllow = false, hasDeny = false;
    ACLKey userKey = std::make_pair(userId, SubjectType::User);
    auto userIt = entries.find(userKey);
    if (userIt != entries.end() && userIt->second.hasPermission(p_type)) {
        if (userIt->second.getPermissionEffect(p_type) == PermissionEffect::Deny) hasDeny = true;
        else hasAllow = true;
    }
    for (unsigned int groupId: userGroups) {
        ACLKey groupKey = std::make_pair(groupId, SubjectType::Group);
        auto groupIt = entries.find(groupKey);
        if (groupIt != entries.end() && groupIt->second.hasPermission(p_type)) {
            if (groupIt->second.getPermissionEffect(p_type) == PermissionEffect::Deny) hasDeny = true;
            else hasAllow = true;
        }
    }
    if (hasDeny) return false;
    return hasAllow;
}

std::map<PermissionType, bool> ACL::getEffectivePermissions(unsigned int userId, const std::vector<unsigned int> &userGroups) const {
    std::map<PermissionType, bool> result;
    int minPerm = static_cast<int>(PermissionType::Read);
    int maxPerm = static_cast<int>(PermissionType::ChangePermissions);
    for (int i = minPerm; i <= maxPerm; i++) {
        PermissionType perm = static_cast<PermissionType>(i);
        result[perm] = checkPermission(userId, userGroups, perm);
    }
    return result;
}

std::vector<ACLEntry> ACL::getEntries() const {
    std::vector<ACLEntry> result;
    for (const auto& [key, entry] : entries) {
        result.push_back(entry);
    }
    return result;
}

void ACL::setEntries(const std::vector<ACLEntry>& newEntries) {
    entries.clear();
    for (const auto& entry : newEntries) {
        ACLKey key(entry.subjectId, entry.subjectType);
        entries[key] = entry;
    }
}
