#include "group_repository.h"
#include <algorithm>

GroupRepository::GroupRepository() : nextId(2) {
    initializeDefaultData();
}

void GroupRepository::initializeDefaultData() {
    auto allGroup = std::make_unique<Group>(0, "All");
    auto adminGroup = std::make_unique<Group>(1, "Administrators");

    groupsById[0] = std::move(allGroup);
    groupsById[1] = std::move(adminGroup);

    idByName["All"] = 0;
    idByName["Administrators"] = 1;
}

Group* GroupRepository::getGroupById(unsigned int id) {
    auto it = groupsById.find(id);
    if (it != groupsById.end() && it->second) return it->second.get();
    return nullptr;
}

Group* GroupRepository::getGroupByName(const std::string& name) {
    auto it = idByName.find(name);
    if (it != idByName.end()) return getGroupById(it->second);
    return nullptr;
}

std::vector<Group*> GroupRepository::getAllGroups() {
    std::vector<Group*> groups;
    groups.reserve(groupsById.size());
    for (const auto& pair : groupsById) {
        if (pair.second) groups.push_back(pair.second.get());
    }
    return groups;
}

bool GroupRepository::saveGroup(std::unique_ptr<Group> group) {
    if (!group) return false;
    unsigned int id = group->getId();
    const std::string& name = group->getName();
    if (groupsById.find(id) != groupsById.end()) return false;
    if (idByName.find(name) != idByName.end()) return false;
    groupsById[id] = std::move(group);
    idByName[name] = id;
    if (id >= nextId) nextId = id + 1;
    return true;
}

bool GroupRepository::deleteGroup(unsigned int id) {
    if (id == 0) return false;
    auto it = groupsById.find(id);
    if (it == groupsById.end()) return false;
    const std::string& name = it->second->getName();
    idByName.erase(name);
    auto userRange = groupToUsers.equal_range(id);
    for (auto userIt = userRange.first; userIt != userRange.second; ++userIt) {
        auto range = userToGroups.equal_range(userIt->second);
        for (auto it2 = range.first; it2 != range.second;) {
            if (it2->second == id) it2 = userToGroups.erase(it2);
            else it2++;
        }
    }
    groupToUsers.erase(id);
    auto parentRange = childToParentGroups.equal_range(id);
    for (auto parentIt = parentRange.first; parentIt != parentRange.second;) {
        unsigned int parentId = parentIt->second;
        auto range = parentToChildGroups.equal_range(parentId);
        for (auto it2 = range.first; it2 != range.second;) {
            if (it2->second == id) it2 = parentToChildGroups.erase(it2);
            else it2++;
        }
        parentIt = childToParentGroups.erase(parentIt);
    }
    auto childRange = parentToChildGroups.equal_range(id);
    for (auto childIt = childRange.first; childIt != childRange.second;) {
        unsigned int childId = childIt->second;
        auto range = childToParentGroups.equal_range(childId);
        for (auto it2 = range.first; it2 != range.second;) {
            if (it2->second == id) {
                it2 = childToParentGroups.erase(it2);
            } else it2++;
        }
        childIt = parentToChildGroups.erase(childIt);
    }
    groupsById.erase(it);
    return true;
}

bool GroupRepository::groupExists(unsigned int id) const {
    return groupsById.find(id) != groupsById.end();
}

bool GroupRepository::groupExists(const std::string& name) const {
    return idByName.find(name) != idByName.end();
}

bool GroupRepository::addUserToGroup(unsigned int userId, unsigned int groupId) {
    if (!groupExists(groupId)) return false;
    auto range = userToGroups.equal_range(userId);
    for (auto it = range.first; it != range.second; it++) {
        if (it->second == groupId) return false;
    }
    userToGroups.insert({userId, groupId});
    groupToUsers.insert({groupId, userId});
    return true;
}

bool GroupRepository::removeUserFromGroup(unsigned int userId, unsigned int groupId) {
    auto range = userToGroups.equal_range(userId);
    bool found = false;
    for (auto it = range.first; it != range.second;) {
        if (it->second == groupId) {
            it = userToGroups.erase(it);
            found = true;
        } else it++;
    }
    if (!found) return false;
    auto range2 = groupToUsers.equal_range(groupId);
    for (auto it = range2.first; it != range2.second;) {
        if (it->second == userId) {
            it = groupToUsers.erase(it);
            break;
        } else it++;
    }
    return true;
}

bool GroupRepository::addSubgroup(unsigned int parentGroupId, unsigned int childGroupId) {
    if (!groupExists(parentGroupId) || !groupExists(childGroupId)) return false;
    if (parentGroupId == childGroupId) return false;
    if (wouldCreateCycle(parentGroupId, childGroupId)) return false;
    auto range = parentToChildGroups.equal_range(parentGroupId);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == childGroupId)return false;
    }
    parentToChildGroups.insert({parentGroupId, childGroupId});
    childToParentGroups.insert({childGroupId, parentGroupId});
    return true;
}

bool GroupRepository::removeSubgroup(unsigned int parentGroupId, unsigned int childGroupId) {
    bool removed = false;
    auto range = parentToChildGroups.equal_range(parentGroupId);
    for (auto it = range.first; it != range.second;) {
        if (it->second == childGroupId) {
            it = parentToChildGroups.erase(it);
            removed = true;
            break;
        } else it++;
    }
    if (!removed) return false;
    auto range2 = childToParentGroups.equal_range(childGroupId);
    for (auto it = range2.first; it != range2.second;) {
        if (it->second == parentGroupId) {
            it = childToParentGroups.erase(it);
            break;
        } else it++;
    }
    return true;
}

bool GroupRepository::setNextId(unsigned int id) {
    if (id < nextId) return false;
    nextId = id;
    return true;
}

std::vector<unsigned int> GroupRepository::getDirectSubgroups(unsigned int groupId) {
    std::vector<unsigned int> result;
    auto range = parentToChildGroups.equal_range(groupId);
    for (auto it = range.first; it != range.second; ++it) {
        result.push_back(it->second);
    }
    return result;
}

std::vector<unsigned int> GroupRepository::getDirectParentGroups(unsigned int groupId) {
    std::vector<unsigned int> result;
    auto range = childToParentGroups.equal_range(groupId);
    for (auto it = range.first; it != range.second; ++it) {
        result.push_back(it->second);
    }
    return result;
}

std::vector<unsigned int> GroupRepository::getAllParentGroups(unsigned int groupId) {
    std::vector<unsigned int> result;
    std::set<unsigned int> visited;
    std::vector<unsigned int> stack = {groupId};
    while (!stack.empty()) {
        unsigned int current = stack.back();
        stack.pop_back();
        auto range = childToParentGroups.equal_range(current);
        for (auto it = range.first; it != range.second; ++it) {
            unsigned int parentId = it->second;
            if (visited.find(parentId) == visited.end()) {
                visited.insert(parentId);
                result.push_back(parentId);
                stack.push_back(parentId);
            }
        }
    }
    return result;
}

std::vector<unsigned int> GroupRepository::getAllSubgroups(unsigned int groupId) {
    std::vector<unsigned int> result;
    std::set<unsigned int> visited;
    std::vector<unsigned int> stack = {groupId};
    while (!stack.empty()) {
        unsigned int current = stack.back();
        stack.pop_back();
        auto range = parentToChildGroups.equal_range(current);
        for (auto it = range.first; it != range.second; ++it) {
            unsigned int childId = it->second;
            if (visited.find(childId) == visited.end()) {
                visited.insert(childId);
                result.push_back(childId);
                stack.push_back(childId);
            }
        }
    }
    return result;
}

std::vector<unsigned int> GroupRepository::getAllGroupsOfUser(unsigned int userId) {
    std::vector<unsigned int> result;
    auto range = userToGroups.equal_range(userId);
    for (auto it = range.first; it != range.second; ++it) {
        result.push_back(it->second);
    }
    return result;
}

bool GroupRepository::isUserInGroupRecursive(unsigned int userId, unsigned int groupId) {
    std::vector<unsigned int> userGroups = getAllGroupsOfUser(userId);
    for (unsigned int userGroupId : userGroups) {
        if (userGroupId == groupId) return true;
        std::vector<unsigned int> parentGroups = getAllParentGroups(userGroupId);
        if (std::find(parentGroups.begin(), parentGroups.end(), groupId) != parentGroups.end()) return true;
    }
    return false;
}

bool GroupRepository::isSubgroupRecursive(unsigned int parentGroupId, unsigned int childGroupId) {
    if (parentGroupId == childGroupId) return false;
    std::vector<unsigned int> allParents = getAllParentGroups(childGroupId);
    return std::find(allParents.begin(), allParents.end(), parentGroupId) != allParents.end();
}

bool GroupRepository::validateNoCycle(unsigned int parentId, unsigned int childId, std::set<unsigned int>& visited) const {
    if (parentId == childId) return false;
    if (visited.find(parentId) != visited.end()) return true;
    visited.insert(parentId);
    auto range = parentToChildGroups.equal_range(parentId);
    for (auto it = range.first; it != range.second; ++it) {
        if (!validateNoCycle(it->second, childId, visited)) return false;
    }
    return true;
}

bool GroupRepository::wouldCreateCycle(unsigned int parentId, unsigned int childId) {
    std::set<unsigned int> visited;
    return !validateNoCycle(childId, parentId, visited);
}

void GroupRepository::clear() {
    auto allGroupIt = groupsById.find(0);
    auto adminGroupIt = groupsById.find(1);
    std::unique_ptr<Group> allGroup, adminGroup;
    if (allGroupIt != groupsById.end()) allGroup = std::move(allGroupIt->second);
    if (adminGroupIt != groupsById.end()) adminGroup = std::move(adminGroupIt->second);
    groupsById.clear();
    idByName.clear();
    userToGroups.clear();
    groupToUsers.clear();
    parentToChildGroups.clear();
    childToParentGroups.clear();
    if (allGroup) {
        groupsById[0] = std::move(allGroup);
        idByName["All"] = 0;
    }
    if (adminGroup) {
        groupsById[1] = std::move(adminGroup);
        idByName["Administrators"] = 1;
    }
    nextId = 2;
}