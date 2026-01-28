#include "user_repository.h"
#include <algorithm>

UserRepository::UserRepository() : nextId(1) {}

User* UserRepository::getUserById(unsigned int id) {
    auto it = usersById.find(id);
    if (it != usersById.end() && it->second) return it->second.get();
    return nullptr;
}

User* UserRepository::getUserByName(const std::string& name) {
    auto it = idByName.find(name);
    if (it != idByName.end()) return getUserById(it->second);
    return nullptr;
}

std::vector<User*> UserRepository::getAllUsers() {
    std::vector<User*> users;
    users.reserve(usersById.size());
    for (const auto& pair : usersById) {
        if (pair.second) users.push_back(pair.second.get());
    }
    return users;
}

bool UserRepository::saveUser(std::unique_ptr<User> user) {
    if (!user || user->getName().empty()) return false;
    unsigned int id = user->getId();
    const std::string& name = user->getName();
    if (usersById.find(id) != usersById.end()) return false;
    if (idByName.find(name) != idByName.end()) return false;
    usersById[id] = std::move(user);
    idByName[name] = id;
    if (id >= nextId) nextId = id + 1;
    return true;
}

bool UserRepository::deleteUser(unsigned int id) {
    auto it = usersById.find(id);
    if (it == usersById.end()) return false;
    const std::string& name = it->second->getName();
    idByName.erase(name);
    usersById.erase(it);
    return true;
}

bool UserRepository::userExists(unsigned int id) const {
    return usersById.find(id) != usersById.end();
}

bool UserRepository::userExists(const std::string& name) const {
    return idByName.find(name) != idByName.end();
}

User* UserRepository::authenticate(const std::string& username) {
    return getUserByName(username);
}

void UserRepository::clear() {
    usersById.clear();
    idByName.clear();
    nextId = 1;
}