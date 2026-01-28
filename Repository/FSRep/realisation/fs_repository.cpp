#include "fs_repository.h"
#include "Entity/File/realisation/file_descriptor.h"
#include "Entity/Directory/realisation/directory_descriptor.h"
#include <sstream>
#include <iostream>
#include <memory>
#include "Repository/FSRep/realisation/Path/path.h"

FileSystemRepository::FileSystemRepository() : nextAddress(1), rootDirectory(nullptr) {
    User adminUser(1, "Administrator");
    auto rootDir = std::make_unique<DirectoryDescriptor>("/", 0, adminUser, 0);
    rootDirectory = dynamic_cast<IDirectory*>(rootDir.get());
    objectsByAddress[0] = std::move(rootDir);
    initializeDefaultData();
}

FileSystemRepository::~FileSystemRepository() {}

void FileSystemRepository::initializeDefaultData() {}

void FileSystemRepository::findObjectsInDirectory(const std::string& pattern, IDirectory* directory, std::vector<IFileSystemObject*>& results) const {
    if (!directory) return;
    auto children = directory->listChild();
    for (auto* child : children) {
        if (child && Path::matchesPattern(child->getName(), pattern)) results.push_back(child);
        auto* childDir = dynamic_cast<IDirectory*>(child);
        if (childDir) findObjectsInDirectory(pattern, childDir, results);
    }
}

void FileSystemRepository::setRootDirectory(IDirectory* rootDir) {
    if (rootDir) {
        auto* fsObject = dynamic_cast<IFileSystemObject*>(rootDir);
        if (fsObject && fsObject->getAddress() == 0) rootDirectory = rootDir;
    }
}

IDirectory* FileSystemRepository::getRootDirectory() const {
    return rootDirectory;
}

std::vector<IFileSystemObject*> FileSystemRepository::getAllObjects() const {
    std::vector<IFileSystemObject*> result;
    result.reserve(objectsByAddress.size());
    for (const auto& pair : objectsByAddress) {
        if (pair.second) result.push_back(pair.second.get());
    }
    return result;
}

IFileSystemObject* FileSystemRepository::getObjectByAddress(unsigned int address) const {
    auto it = objectsByAddress.find(address);
    if (it != objectsByAddress.end() && it->second) return it->second.get();
    return nullptr;
}

IFileSystemObject* FileSystemRepository::getObjectByPath(const std::string& path) const {
    if (!rootDirectory) return nullptr;
    std::string normalizedPath = Path::normalizePath(path);
    if (normalizedPath == "/") return dynamic_cast<IFileSystemObject*>(rootDirectory);
    std::string withoutSlash = normalizedPath.substr(1);
    IDirectory* currentDir = rootDirectory;
    std::istringstream iss(withoutSlash);
    std::string segment;
    std::vector<std::string> segments;
    while (std::getline(iss, segment, '/')) {
        segments.push_back(segment);
    }
    for (size_t i = 0; i < segments.size(); i++) {
        const std::string& seg = segments[i];
        if (seg.empty() || seg == ".") continue;
        if (seg == "..") {
            auto* currentObj = dynamic_cast<IFileSystemObject*>(currentDir);
            if (!currentObj) break;
            auto* parent = getDirectoryByPath(Path::getParentPath(normalizedPath));
            if (!parent) currentDir = rootDirectory;
            else currentDir = parent;
            continue;
        }
        IFileSystemObject* child = currentDir->getChild(seg);
        if (!child) return nullptr;
        if (i == segments.size() - 1) return child;
        auto* childDir = dynamic_cast<IDirectory*>(child);
        if (!childDir) return nullptr;
        currentDir = childDir;
    }
    return dynamic_cast<IFileSystemObject*>(currentDir);
}

IDirectory* FileSystemRepository::getDirectoryByPath(const std::string& path) const {
    auto* obj = getObjectByPath(path);
    if (!obj) return nullptr;
    return dynamic_cast<IDirectory*>(obj);
}

IFile* FileSystemRepository::getFileByPath(const std::string& path) const {
    auto* obj = getObjectByPath(path);
    if (!obj) return nullptr;
    return dynamic_cast<IFile*>(obj);
}

bool FileSystemRepository::saveObject(std::unique_ptr<IFileSystemObject> object) {
    if (!object) return false;
    unsigned int address = object->getAddress();
    objectsByAddress[address] = std::move(object);
    if (address >= nextAddress) nextAddress = address + 1;
    return true;
}

bool FileSystemRepository::deleteObject(unsigned int address) {
    if (address == 0) return false;
    auto it = objectsByAddress.find(address);
    if (it == objectsByAddress.end()) return false;
    auto* obj = it->second.get();
    if (obj) {
        unsigned int parentAddress = obj->getParentDirectoryAddress();
        auto parentIt = objectsByAddress.find(parentAddress);
        if (parentIt != objectsByAddress.end()) {
            auto* parentDir = dynamic_cast<IDirectory*>(parentIt->second.get());
            if (parentDir) parentDir->removeChild(obj->getName());
        }
    }
    objectsByAddress.erase(it);
    return true;
}

bool FileSystemRepository::objectExists(unsigned int address) const {
    return objectsByAddress.find(address) != objectsByAddress.end();
}

bool FileSystemRepository::pathExists(const std::string& path) const {
    return getObjectByPath(path) != nullptr;
}

std::vector<IFileSystemObject*> FileSystemRepository::findObjects(
    const std::string& pattern, const std::string& startPath) const {
    std::vector<IFileSystemObject*> results;
    IDirectory* startDir = nullptr;
    if (startPath.empty()) startDir = rootDirectory;
    else startDir = getDirectoryByPath(startPath);
    if (!startDir) return results;
    findObjectsInDirectory(pattern, startDir, results);
    return results;
}

unsigned int FileSystemRepository::getAddress() {
    return nextAddress++;
}

void FileSystemRepository::buildPathRecursive(IFileSystemObject* object, std::vector<std::string>& parts) const {
    if (!object) return;
    unsigned int parentAddress = object->getParentDirectoryAddress();
    if (parentAddress != 0) {
        auto* parent = getObjectByAddress(parentAddress);
        if (parent) {
            buildPathRecursive(parent, parts);
        }
    }
    parts.push_back(object->getName());
}

std::string FileSystemRepository::getPath(IFileSystemObject* object) const {
    if (!object) return "";
    if (object == dynamic_cast<IFileSystemObject*>(rootDirectory)) return "/";
    std::vector<std::string> parts;
    buildPathRecursive(object, parts);
    std::string path = "/";
    for (const auto& part : parts) {
        if (path.back() != '/') path += "/";
        path += part;
    }
    return path;
}

    void FileSystemRepository::clear() {
    auto rootIt = objectsByAddress.find(0);
    if (rootIt != objectsByAddress.end()) {
        auto rootDir = std::move(rootIt->second);
        objectsByAddress.clear();
        objectsByAddress[0] = std::move(rootDir);
        rootDirectory = dynamic_cast<IDirectory*>(objectsByAddress[0].get());
    }
    nextAddress = 1;
}