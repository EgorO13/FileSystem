#include "directory_descriptor.h"
#include "Entity/User/user.h"
#include "Entity/FSObject/realisation/fs_object.h"

#include <string>
#include <vector>

DirectoryDescriptor::DirectoryDescriptor(const std::string &name, unsigned int parentAddress, const User &owner, unsigned int adr)
    : FileSystemObject(name, parentAddress, owner, adr) {}

bool DirectoryDescriptor::addChild(IFileSystemObject* obj) {
    if (!obj || children.contains(obj->getName())) return false;
    children.insert(TablePair<std::string, IFileSystemObject *>(obj->getName(), obj));
    updateModificationTime();
    return true;
}

bool DirectoryDescriptor::removeChild(const std::string &name) {
    if (name.empty()) return false;
    if (!children.erase(name)) return false;
    updateModificationTime();
    return true;
}

IFileSystemObject* DirectoryDescriptor::getChild(const std::string &name) const {
    auto it = children.find(name);
    if (it != children.end()) return it->value;
    return nullptr;
}

int DirectoryDescriptor::getChildCount() const {
    return static_cast<int>(children.size());
}

std::vector<IFileSystemObject*> DirectoryDescriptor::listChild() const {
    std::vector<IFileSystemObject*> result;
    result.reserve(children.size());
    for (auto it = children.begin(); it != children.end(); ++it) {
        result.push_back(it->value);
    }
    return result;
}

bool DirectoryDescriptor::containChild(const std::string &name) const {
    if (name.empty()) return false;
    return children.contains(name);
}