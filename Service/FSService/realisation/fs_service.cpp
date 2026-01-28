#include "fs_service.h"
#include "Entity/File/realisation/file_descriptor.h"
#include "Entity/Directory/realisation/directory_descriptor.h"
#include "Repository/FSRep/realisation/Path/path.h"
#include <queue>

FileSystemService::FileSystemService(IFileSystemRepository& fsRepo, ISecurityService& secService, ISessionService& sessionServ)
    : fsRepository(fsRepo), securityService(secService), sessionService(sessionServ) {}

std::string FileSystemService::resolveUserPath(const std::string& path) const {
    IFileSystemObject* currentObj = dynamic_cast<IFileSystemObject*>(sessionService.getCurrentDirectory());
    if (!currentObj) return "";
    std::string currentPath = fsRepository.getPath(currentObj);
    if (currentPath.empty()) return "";
    return Path::resolvePath(currentPath, path);
}

IFileSystemObject* FileSystemService::getObject(const std::string& path) const {
    std::string resolvedPath = resolveUserPath(path);
    if (resolvedPath.empty()) return nullptr;
    return fsRepository.getObjectByPath(resolvedPath);
}

bool FileSystemService::validateOperationPath(const std::string& path) const {
    std::string resolvedPath = resolveUserPath(path);
    return Path::isValidPath(resolvedPath);
}

IDirectory* FileSystemService::changeDirectory(const User& user, const std::string& path, IDirectory* currentDir) {
    IDirectory* savedCurrent = sessionService.getCurrentDirectory();
    sessionService.setCurrentDirectory(currentDir);
    std::string resolvedPath = resolveUserPath(path);
    IDirectory* directory = fsRepository.getDirectoryByPath(resolvedPath);
    sessionService.setCurrentDirectory(savedCurrent);
    if (!directory) return nullptr;
    IFileSystemObject* fsObject = dynamic_cast<IFileSystemObject*>(directory);
    if (!fsObject) return nullptr;
    if (!securityService.canExecute(user, *fsObject)) return nullptr;
    sessionService.setCurrentDirectory(directory);
    return directory;
}

std::vector<FileInfo> FileSystemService::listDirectory(const User& user, const std::string& path) {
    std::vector<FileInfo> result;
    IDirectory* targetDir = nullptr;
    if (path.empty()) targetDir = sessionService.getCurrentDirectory();
    else {
        std::string resolvedPath = resolveUserPath(path);
        targetDir = fsRepository.getDirectoryByPath(resolvedPath);
    }
    if (!targetDir) return result;
    IFileSystemObject* fsObject = dynamic_cast<IFileSystemObject*>(targetDir);
    if (!fsObject || !securityService.canRead(user, *fsObject)) return result;
    std::vector<IFileSystemObject*> children = targetDir->listChild();
    for (IFileSystemObject* child : children) {
        if (!child) continue;
        FileInfo info;
        info.name = child->getName();
        if (dynamic_cast<IFile*>(child)) info.type = "file";
        else if (dynamic_cast<IDirectory*>(child)) info.type = "dir";
        else info.type = "unknown";
        result.push_back(info);
    }
    return result;
}

std::vector<std::string> FileSystemService::findFiles(const User& user, const std::string& pattern, const std::string& startPath) {
    std::vector<std::string> result;
    std::string resolvedStartPath;
    if (startPath.empty()) {
        IFileSystemObject* currentObj = dynamic_cast<IFileSystemObject*>(sessionService.getCurrentDirectory());
        if (!currentObj) return result;
        resolvedStartPath = fsRepository.getPath(currentObj);
    } else resolvedStartPath = resolveUserPath(startPath);
    if (resolvedStartPath.empty()) return result;
    IDirectory* startDir = fsRepository.getDirectoryByPath(resolvedStartPath);
    if (!startDir) return result;
    IFileSystemObject* startFsObject = dynamic_cast<IFileSystemObject*>(startDir);
    if (!startFsObject || !securityService.canRead(user, *startFsObject)) return result;
    std::vector<IFileSystemObject*> objects = fsRepository.findObjects(pattern, resolvedStartPath);
    for (IFileSystemObject* obj : objects) {
        if (!obj) continue;
        IFile* file = dynamic_cast<IFile*>(obj);
        if (file && securityService.canRead(user, *obj)) {
            std::string objPath = fsRepository.getPath(obj);
            if (!objPath.empty()) result.push_back(objPath);
        }
    }
    return result;
}

IFile* FileSystemService::createFile(const User& user, const std::string& path, const std::string& content) {
    if (!validateOperationPath(path)) return nullptr;
    std::string resolvedPath = resolveUserPath(path);
    if (resolvedPath.empty() || fsRepository.pathExists(resolvedPath)) return nullptr;
    std::string parentPath = Path::getParentPath(resolvedPath);
    IDirectory* parentDir = fsRepository.getDirectoryByPath(parentPath);
    if (!parentDir) return nullptr;
    IFileSystemObject* parentObject = dynamic_cast<IFileSystemObject*>(parentDir);
    if (!parentObject || !securityService.canWrite(user, *parentObject)) return nullptr;
    unsigned int address = fsRepository.getAddress();
    std::string fileName = Path::getFileName(resolvedPath);
    IFileSystemObject* parentFsObj = dynamic_cast<IFileSystemObject*>(parentDir);
    if (!parentFsObj) return nullptr;
    auto file = std::make_unique<FileDescriptor>(fileName, parentFsObj->getAddress(), user, address);
    if (!content.empty() && !file->writeContent(content)) return nullptr;
    if (!parentDir->addChild(file.get())) return nullptr;
    if (!fsRepository.saveObject(std::move(file))) {
        parentDir->removeChild(fileName);
        return nullptr;
    }
    return dynamic_cast<IFile*>(fsRepository.getObjectByAddress(address));
}

std::string FileSystemService::readFile(const User& user, const std::string& path) {
    IFileSystemObject* obj = getObject(path);
    if (!obj) return "";
    IFile* file = dynamic_cast<IFile*>(obj);
    if (!file) return "";
    if (!securityService.canRead(user, *obj)) return "";
    return std::string(file->readContent());
}

bool FileSystemService::writeFile(const User& user, const std::string& path, const std::string& content, bool append) {
    IFileSystemObject* obj = getObject(path);
    if (!obj) return false;
    IFile* file = dynamic_cast<IFile*>(obj);
    if (!file) return false;
    if (!securityService.canWrite(user, *obj)) return false;
    if (append) {
        std::string currentContent = file->readContent();
        return file->writeContent(currentContent + content);
    }
    return file->writeContent(content);
}

bool FileSystemService::deleteFile(const User& user, const std::string& path) {
    IFileSystemObject* obj = getObject(path);
    if (!obj) return false;
    IFile* file = dynamic_cast<IFile*>(obj);
    if (!file) return false;
    if (!securityService.canModify(user, *obj)) return false;
    IDirectory* parentDir = dynamic_cast<IDirectory*>(fsRepository.getObjectByAddress(obj->getParentDirectoryAddress()));
    if (!parentDir) return false;
    if (!parentDir->removeChild(obj->getName())) return false;
    return fsRepository.deleteObject(obj->getAddress());
}

bool FileSystemService::copyFile(const User& user, const std::string& source, const std::string& destination) {
    std::string sourcePath = resolveUserPath(source);
    if (sourcePath.empty()) return false;
    IFile* sourceFile = fsRepository.getFileByPath(sourcePath);
    if (!sourceFile) return false;
    IFileSystemObject* sourceFsObj = dynamic_cast<IFileSystemObject*>(sourceFile);
    if (!sourceFsObj || !securityService.canRead(user, *sourceFsObj)) return false;
    std::string content = sourceFile->readContent();
    return createFile(user, destination, content) != nullptr;
}

bool FileSystemService::moveFile(const User& user, const std::string& source, const std::string& destination) {
    if (!copyFile(user, source, destination)) return false;
    return deleteFile(user, source);
}

IDirectory* FileSystemService::createDirectory(const User& user, const std::string& path) {
    if (!validateOperationPath(path)) return nullptr;
    std::string resolvedPath = resolveUserPath(path);
    if (resolvedPath.empty() || fsRepository.pathExists(resolvedPath)) return nullptr;
    std::string parentPath = Path::getParentPath(resolvedPath);
    IDirectory* parentDir = fsRepository.getDirectoryByPath(parentPath);
    if (!parentDir) return nullptr;
    IFileSystemObject* parentObject = dynamic_cast<IFileSystemObject*>(parentDir);
    if (!parentObject || !securityService.canWrite(user, *parentObject)) return nullptr;
    unsigned int address = fsRepository.getAddress();
    std::string dirName = Path::getFileName(resolvedPath);
    IFileSystemObject* parentFsObj = dynamic_cast<IFileSystemObject*>(parentDir);
    if (!parentFsObj) return nullptr;
    auto dir = std::make_unique<DirectoryDescriptor>(dirName, parentFsObj->getAddress(), user, address);
    if (!parentDir->addChild(dir.get())) return nullptr;
    if (!fsRepository.saveObject(std::move(dir))) {
        parentDir->removeChild(dirName);
        return nullptr;
    }
    return dynamic_cast<IDirectory*>(fsRepository.getObjectByAddress(address));
}

bool FileSystemService::deleteDirectory(const User& user, const std::string& path, bool recursive) {
    IFileSystemObject* obj = getObject(path);
    if (!obj) return false;
    IDirectory* dir = dynamic_cast<IDirectory*>(obj);
    if (!dir) return false;
    if (!securityService.canModify(user, *obj)) return false;
    if (!recursive && dir->getChildCount() > 0) return false;
    IDirectory* parentDir = dynamic_cast<IDirectory*>(fsRepository.getObjectByAddress(obj->getParentDirectoryAddress()));
    if (!parentDir) return false;
    if (!parentDir->removeChild(obj->getName())) return false;
    return fsRepository.deleteObject(obj->getAddress());
}

bool FileSystemService::copyDirectory(const User& user, const std::string& source, const std::string& destination) {
    IDirectory* destDir = createDirectory(user, destination);
    if (!destDir) return false;
    std::string sourcePath = resolveUserPath(source);
    if (sourcePath.empty()) return false;
    IDirectory* sourceDir = fsRepository.getDirectoryByPath(sourcePath);
    if (!sourceDir) return false;
    IFileSystemObject* sourceFsObj = dynamic_cast<IFileSystemObject*>(sourceDir);
    if (!sourceFsObj || !securityService.canRead(user, *sourceFsObj)) return false;
    std::queue<std::pair<IDirectory*, IDirectory*>> dirsToCopy;
    dirsToCopy.push({sourceDir, destDir});
    while (!dirsToCopy.empty()) {
        auto [srcDir, dstDir] = dirsToCopy.front();
        dirsToCopy.pop();
        std::vector<IFileSystemObject*> children = srcDir->listChild();
        for (IFileSystemObject* child : children) {
            if (!child) continue;
            if (!securityService.canRead(user, *child)) continue;
            IFile* file = dynamic_cast<IFile*>(child);
            if (file) {
                std::string content = file->readContent();
                IFileSystemObject* dstFsObj = dynamic_cast<IFileSystemObject*>(dstDir);
                if (!dstFsObj) continue;
                std::string dstPath = fsRepository.getPath(dstFsObj) + "/" + child->getName();
                createFile(user, dstPath, content);
            } else {
                IDirectory* subDir = dynamic_cast<IDirectory*>(child);
                if (subDir) {
                    IFileSystemObject* dstFsObj = dynamic_cast<IFileSystemObject*>(dstDir);
                    if (!dstFsObj) continue;
                    std::string dstPath = fsRepository.getPath(dstFsObj) + "/" + child->getName();
                    IDirectory* newSubDir = createDirectory(user, dstPath);
                    if (newSubDir) dirsToCopy.push({subDir, newSubDir});
                }
            }
        }
    }
    return true;
}

bool FileSystemService::moveDirectory(const User& user, const std::string& source, const std::string& destination) {
    if (!copyDirectory(user, source, destination)) return false;
    return deleteDirectory(user, source, true);
}

bool FileSystemService::changePermissions(unsigned int id, SubjectType s_type, const std::string& path, const std::map<PermissionType, PermissionEffect>& permissions) {
    IFileSystemObject* obj = getObject(path);
    if (!obj) return false;
    const User* currentUser = sessionService.getCurrentUser();
    if (!currentUser) return false;
    if (!securityService.canChangePermissions(*currentUser, *obj)) return false;
    for (const auto& [perm, effect] : permissions) {
        std::vector<PermissionType> perms = {perm};
        obj->setPermissions(id, s_type, perms, effect);
    }
    obj->updateModificationTime();
    return true;
}

bool FileSystemService::changeOwner(const User& user, const std::string& path, const std::string& newOwnerUsername) {
    IFileSystemObject* obj = getObject(path);
    if (!obj) return false;
    if (!securityService.canChangePermissions(user, *obj)) return false;
    return false;
}

bool FileSystemService::lockFile(const User& user, const std::string& path, Lock lockType) {
    IFileSystemObject* obj = getObject(path);
    if (!obj) return false;
    ILockable* lockable = dynamic_cast<ILockable*>(obj);
    if (!lockable) return false;
    if (!securityService.canModify(user, *obj)) return false;
    lockable->setMode(lockType);
    return true;
}

bool FileSystemService::unlockFile(const User& user, const std::string& path) {
    return lockFile(user, path, Lock::NotLock);
}

bool FileSystemService::exists(const std::string& path) {
    std::string resolvedPath = resolveUserPath(path);
    return !resolvedPath.empty() && fsRepository.pathExists(resolvedPath);
}

bool FileSystemService::isFile(const std::string& path) {
    std::string resolvedPath = resolveUserPath(path);
    if (resolvedPath.empty()) return false;
    IFileSystemObject* obj = fsRepository.getObjectByPath(resolvedPath);
    return obj && dynamic_cast<IFile*>(obj) != nullptr;
}

bool FileSystemService::isDirectory(const std::string& path) {
    std::string resolvedPath = resolveUserPath(path);
    if (resolvedPath.empty()) return false;
    IFileSystemObject* obj = fsRepository.getObjectByPath(resolvedPath);
    return obj && dynamic_cast<IDirectory*>(obj) != nullptr;
}