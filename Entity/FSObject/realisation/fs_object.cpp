#include "fs_object.h"

bool FileSystemObject::isValidName(const std::string& filename) {
    if (filename.empty() || filename.size() > 255) return false;
    if (filename.find('/') != std::string::npos && filename.length() != 1) return false;
    if (filename == "." || filename == ".." || filename == "") return false;
    return true;
}

void FileSystemObject::setPermissions(unsigned int id, SubjectType s_type, std::vector<PermissionType> p_types, PermissionEffect effect) {
    for (auto perm : p_types) {
        acl.setPermission(id, s_type, perm, effect);
    }
}

void FileSystemObject::setPermission(unsigned int id, SubjectType s_type, PermissionType p_types, PermissionEffect effect) {
    acl.setPermission(id, s_type, p_types, effect);
}


bool FileSystemObject::setName(const std::string& newName) {
    if (!isValidName(newName)) return false;
    name = newName;
    updateModificationTime();
    return true;
}

void FileSystemObject::setAddress(unsigned int newAddress) { address = newAddress; }

void FileSystemObject::setOwner(const User& newOwner) {
    owner = newOwner;
    acl.setOwnerId(newOwner.getId());
    updateModificationTime();
}

void FileSystemObject::setParentDirectoryAddress(unsigned int newAddress) {
    parentAddress = newAddress;
    updateModificationTime();
}

std::chrono::system_clock::time_point FileSystemObject::getCreateTime() const { return creationTime; }

std::chrono::system_clock::time_point FileSystemObject::getLastModifyTime() const { return lastModifyTime; }

std::string FileSystemObject::getName() const { return name; }

unsigned int FileSystemObject::getAddress() const { return address; }

const User& FileSystemObject::getOwner() const { return owner; }

bool FileSystemObject::checkPermission(unsigned int userId, const std::vector<unsigned int>& userGroups, PermissionType perm) const {
    return acl.checkPermission(userId, userGroups, perm);
}

void FileSystemObject::updateModificationTime() {
    lastModifyTime = std::chrono::system_clock::now();
}

std::vector<ACLEntry> FileSystemObject::getACL() const {
    return acl.getEntries();
}

void FileSystemObject::setACL(const std::vector<ACLEntry>& entries) {
    acl.setEntries(entries);
}

void FileSystemObject::setCreateTime(std::chrono::system_clock::time_point time) {
    creationTime = time;
}

void FileSystemObject::setLastModifyTime(std::chrono::system_clock::time_point time) {
    lastModifyTime = time;
}
