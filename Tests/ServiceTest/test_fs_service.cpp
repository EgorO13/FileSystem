#include <catch2/catch_test_macros.hpp>
#include "Service/FSService/realisation/fs_service.h"
#include "Repository/FSRep/realisation/fs_repository.h"
#include "Service/UserManagementService/realisation/user_management_service.h"
#include "Service/SecurityService/realisation/security_service.h"
#include "Service/SessionService/realisation/session_service.h"
#include "Repository/UserRep/realisation/user_repository.h"
#include "Repository/GroupRep/realisation/group_repository.h"
#include <memory>

namespace {
void initializeTestEnvironment(UserRepository& userRepo, GroupRepository& groupRepo, IFileSystemRepository& fsRepo, SecurityService& securityService, User*& admin, User*& testUser) {
    auto adminUser = std::make_unique<User>(1, "Administrator");
    admin = adminUser.get();
    userRepo.saveUser(std::move(adminUser));

    auto adminGroup = std::make_unique<Group>(1, "Administrators");
    groupRepo.saveGroup(std::move(adminGroup));

    auto allGroup = std::make_unique<Group>(2, "All");
    groupRepo.saveGroup(std::move(allGroup));

    auto testUserObj = std::make_unique<User>(2, "testUser");
    testUser = testUserObj.get();
    userRepo.saveUser(std::move(testUserObj));

    groupRepo.addUserToGroup(admin->getId(), 1);
    groupRepo.addUserToGroup(testUser->getId(), 2);
    groupRepo.addUserToGroup(admin->getId(), 2);
}

class MockFileSystemRepository : public IFileSystemRepository {
private:
    FileSystemRepository realRepo;
    bool shouldFailSave = false;

public:
    void setShouldFailSave(bool fail) { shouldFailSave = fail; }

    IDirectory* getRootDirectory() const override { return realRepo.getRootDirectory(); }
    IFileSystemObject* getObjectByPath(const std::string& path) const override {
        return realRepo.getObjectByPath(path);
    }
    IDirectory* getDirectoryByPath(const std::string& path) const override {
        return realRepo.getDirectoryByPath(path);
    }
    IFile* getFileByPath(const std::string& path) const override {
        return realRepo.getFileByPath(path);
    }

    bool saveObject(std::unique_ptr<IFileSystemObject> object) override {
        if (shouldFailSave) {
            return false;
        }
        return realRepo.saveObject(std::move(object));
    }

    bool deleteObject(unsigned int address) override {
        return realRepo.deleteObject(address);
    }
    bool pathExists(const std::string& path) const override {
        return realRepo.pathExists(path);
    }
    std::vector<IFileSystemObject*> findObjects(const std::string& pattern,
                                               const std::string& startPath = "") const override {
        return realRepo.findObjects(pattern, startPath);
    }
    unsigned int getAddress() override {
        return realRepo.getAddress();
    }
    std::string getPath(IFileSystemObject* object) const override {
        return realRepo.getPath(object);
    }
    void setRootDirectory(IDirectory* rootDirectory) override { realRepo.setRootDirectory(rootDirectory); }
    std::vector<IFileSystemObject*> getAllObjects() const override { return realRepo.getAllObjects(); }
    IFileSystemObject* getObjectByAddress(unsigned int address) const override {
        return realRepo.getObjectByAddress(address);
    }
    bool objectExists(unsigned int address) const override { return realRepo.objectExists(address); }
    void clear() override { realRepo.clear(); }
};
}

TEST_CASE("FileSystemService") {
    SECTION("getCurrentDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        REQUIRE(fsService.getCurrentDirectory() != nullptr);
        REQUIRE(fsService.getRootDirectory() != nullptr);
        REQUIRE(fsService.getRootDirectory() == fsRepo->getRootDirectory());
    }

    SECTION("createFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        auto file = fsService.createFile(*admin, "/test1.txt", "Hello World");
        REQUIRE(file != nullptr);
        REQUIRE(fsService.exists("/test1.txt"));
        REQUIRE(fsService.isFile("/test1.txt"));

        auto duplicate = fsService.createFile(*admin, "/test1.txt", "Duplicate");
        REQUIRE(duplicate == nullptr);
    }

    SECTION("createDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        auto dir = fsService.createDirectory(*admin, "/testDir");
        REQUIRE(dir != nullptr);
        REQUIRE(fsService.exists("/testDir"));
        REQUIRE(fsService.isDirectory("/testDir"));
    }

    SECTION("readFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/readwrite.txt", "Initial");
        auto content = fsService.readFile(*admin, "/readwrite.txt");
        REQUIRE(content == "Initial");
    }

    SECTION("writeFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/readwrite.txt", "Initial");
        bool writeSuccess = fsService.writeFile(*admin, "/readwrite.txt", "Updated", false);
        REQUIRE(writeSuccess);
        REQUIRE(fsService.readFile(*admin, "/readwrite.txt") == "Updated");

        bool appendSuccess = fsService.writeFile(*admin, "/readwrite.txt", " Appended", true);
        REQUIRE(appendSuccess);
        REQUIRE(fsService.readFile(*admin, "/readwrite.txt") == "Updated Appended");
    }

    SECTION("listDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/file1.txt", "content1");
        fsService.createFile(*admin, "/file2.txt", "content2");
        fsService.createDirectory(*admin, "/dir1");

        auto listing = fsService.listDirectory(*admin, "/");
        REQUIRE(listing.size() >= 3);

        bool foundFile1 = false, foundFile2 = false, foundDir1 = false;
        for (const auto& info : listing) {
            if (info.name == "file1.txt" && info.type == "file") foundFile1 = true;
            if (info.name == "file2.txt" && info.type == "file") foundFile2 = true;
            if (info.name == "dir1" && info.type == "dir") foundDir1 = true;
        }
        REQUIRE(foundFile1);
        REQUIRE(foundFile2);
        REQUIRE(foundDir1);
    }

    SECTION("deleteFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/todelete.txt", "Content");
        REQUIRE(fsService.exists("/todelete.txt"));

        bool success = fsService.deleteFile(*admin, "/todelete.txt");
        REQUIRE(success);
        REQUIRE_FALSE(fsService.exists("/todelete.txt"));

        bool fail = fsService.deleteFile(*admin, "/nonexistent.txt");
        REQUIRE_FALSE(fail);
    }

    SECTION("deleteDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createDirectory(*admin, "/emptydir");
        REQUIRE(fsService.exists("/emptydir"));

        bool success = fsService.deleteDirectory(*admin, "/emptydir", false);
        REQUIRE(success);
        REQUIRE_FALSE(fsService.exists("/emptydir"));

        fsService.createDirectory(*admin, "/dirwithfile");
        fsService.createFile(*admin, "/dirwithfile/file.txt", "content");
        bool nonRecursiveFail = fsService.deleteDirectory(*admin, "/dirwithfile", false);
        REQUIRE_FALSE(nonRecursiveFail);

        bool recursiveSuccess = fsService.deleteDirectory(*admin, "/dirwithfile", true);
        REQUIRE(recursiveSuccess);
        REQUIRE_FALSE(fsService.exists("/dirwithfile"));
    }

    SECTION("copyFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/source.txt", "Source content");

        bool success = fsService.copyFile(*admin, "/source.txt", "/dest.txt");
        REQUIRE(success);
        REQUIRE(fsService.exists("/source.txt"));
        REQUIRE(fsService.exists("/dest.txt"));
        REQUIRE(fsService.readFile(*admin, "/source.txt") == fsService.readFile(*admin, "/dest.txt"));

        bool overwriteFail = fsService.copyFile(*admin, "/source.txt", "/dest.txt");
        REQUIRE_FALSE(overwriteFail);
    }

    SECTION("moveFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/tomove.txt", "Content to move");

        bool success = fsService.moveFile(*admin, "/tomove.txt", "/moved.txt");
        REQUIRE(success);
        REQUIRE_FALSE(fsService.exists("/tomove.txt"));
        REQUIRE(fsService.exists("/moved.txt"));
        REQUIRE(fsService.readFile(*admin, "/moved.txt") == "Content to move");
    }

    SECTION("copyDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createDirectory(*admin, "/sourceDir");
        fsService.createFile(*admin, "/sourceDir/file1.txt", "File1");
        fsService.createDirectory(*admin, "/sourceDir/subdir");
        fsService.createFile(*admin, "/sourceDir/subdir/file2.txt", "File2");

        bool success = fsService.copyDirectory(*admin, "/sourceDir", "/destDir");
        REQUIRE(success);
        REQUIRE(fsService.exists("/sourceDir"));
        REQUIRE(fsService.exists("/destDir"));
        REQUIRE(fsService.exists("/destDir/file1.txt"));
        REQUIRE(fsService.exists("/destDir/subdir"));
        REQUIRE(fsService.exists("/destDir/subdir/file2.txt"));
    }

    SECTION("moveDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createDirectory(*admin, "/tomoveDir");
        fsService.createFile(*admin, "/tomoveDir/file.txt", "Content");

        bool success = fsService.moveDirectory(*admin, "/tomoveDir", "/movedDir");
        REQUIRE(success);
        REQUIRE_FALSE(fsService.exists("/tomoveDir"));
        REQUIRE(fsService.exists("/movedDir"));
        REQUIRE(fsService.exists("/movedDir/file.txt"));
    }

    SECTION("changePermissions") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/permfile.txt", "Content");

        std::map<PermissionType, PermissionEffect> userPerms;
        userPerms[PermissionType::Read] = PermissionEffect::Allow;
        userPerms[PermissionType::Write] = PermissionEffect::Deny;

        bool success = fsService.changePermissions(testUser->getId(), SubjectType::User,
                                                   "/permfile.txt", userPerms);
        REQUIRE(success);

        std::map<PermissionType, PermissionEffect> groupPerms;
        groupPerms[PermissionType::Read] = PermissionEffect::Allow;
        groupPerms[PermissionType::Execute] = PermissionEffect::Allow;

        bool groupSuccess = fsService.changePermissions(2, SubjectType::Group,
                                                       "/permfile.txt", groupPerms);
        REQUIRE(groupSuccess);
    }

    SECTION("lockFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/lockfile.txt", "Content");

        bool lockSuccess = fsService.lockFile(*admin, "/lockfile.txt", Lock::ReadLock);
        REQUIRE(lockSuccess);
    }

    SECTION("unlockFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/lockfile.txt", "Content");
        fsService.lockFile(*admin, "/lockfile.txt", Lock::ReadLock);

        bool unlockSuccess = fsService.unlockFile(*admin, "/lockfile.txt");
        REQUIRE(unlockSuccess);

        bool writeLockSuccess = fsService.lockFile(*admin, "/lockfile.txt", Lock::WriteLock);
        REQUIRE(writeLockSuccess);
    }

    SECTION("findFiles") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/file1.txt", "content");
        fsService.createFile(*admin, "/file2.doc", "content");
        fsService.createDirectory(*admin, "/searchdir");
        fsService.createFile(*admin, "/searchdir/file3.txt", "content");
        fsService.createFile(*admin, "/searchdir/file4.txt", "content");

        auto txtFiles = fsService.findFiles(*admin, "*.txt");
        REQUIRE(txtFiles.size() == 3);

        auto docFiles = fsService.findFiles(*admin, "*.doc");
        REQUIRE(docFiles.size() == 1);

        auto dirFiles = fsService.findFiles(*admin, "*.txt", "/searchdir");
        REQUIRE(dirFiles.size() == 2);

        auto noneFiles = fsService.findFiles(*admin, "*.pdf");
        REQUIRE(noneFiles.empty());
    }

    SECTION("exists") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/testfile.txt", "content");
        fsService.createDirectory(*admin, "/testdir");
        fsService.createFile(*admin, "/testdir/nested.txt", "nested");

        REQUIRE(fsService.exists("/testfile.txt"));
        REQUIRE(fsService.exists("/testdir"));
        REQUIRE(fsService.exists("/testdir/nested.txt"));
        REQUIRE(fsService.exists("/"));
        REQUIRE_FALSE(fsService.exists("/nonexistent"));
        REQUIRE_FALSE(fsService.exists("/testdir/nonexistent.txt"));
    }

    SECTION("isFile") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/file.txt", "content");
        fsService.createDirectory(*admin, "/dir");

        REQUIRE(fsService.isFile("/file.txt"));
        REQUIRE_FALSE(fsService.isFile("/dir"));
        REQUIRE_FALSE(fsService.isFile("/nonexistent"));
    }

    SECTION("isDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        fsService.createFile(*admin, "/file.txt", "content");
        fsService.createDirectory(*admin, "/dir");
        fsService.createDirectory(*admin, "/dir/subdir");
        fsService.createFile(*admin, "/dir/file.txt", "content");

        REQUIRE(fsService.isDirectory("/dir"));
        REQUIRE(fsService.isDirectory("/dir/subdir"));
        REQUIRE(fsService.isDirectory("/"));
        REQUIRE_FALSE(fsService.isDirectory("/file.txt"));
        REQUIRE_FALSE(fsService.isDirectory("/nonexistent"));
    }

    SECTION("changeDirectory") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        auto dir = fsService.createDirectory(*admin, "/testDir");
        REQUIRE(dir != nullptr);

        std::map<PermissionType, PermissionEffect> perms;
        perms[PermissionType::Execute] = PermissionEffect::Allow;
        fsService.changePermissions(testUser->getId(), SubjectType::User, "/testDir", perms);

        auto currentDir = fsRepo->getRootDirectory();
        auto result = fsService.changeDirectory(*testUser, "/testDir", currentDir);
        REQUIRE(result != nullptr);
        REQUIRE(result == dir);

        fsService.createDirectory(*admin, "/parent");
        fsService.createDirectory(*admin, "/parent/child");

        fsService.changePermissions(testUser->getId(), SubjectType::User, "/parent", perms);
        fsService.changePermissions(testUser->getId(), SubjectType::User, "/parent/child", perms);

        auto childDir = fsRepo->getDirectoryByPath("/parent/child");
        REQUIRE(childDir != nullptr);

        auto parentResult = fsService.changeDirectory(*testUser, "..", childDir);
        REQUIRE(parentResult != nullptr);

        auto parentDir = fsRepo->getDirectoryByPath("/parent");
        REQUIRE(parentResult == parentDir);

        auto currentResult = fsService.changeDirectory(*admin, ".", currentDir);
        REQUIRE(currentResult != nullptr);
        REQUIRE(currentResult == currentDir);

        fsService.createDirectory(*admin, "/testDir2");
        auto testDir2 = fsRepo->getDirectoryByPath("/testDir2");

        auto rootResult = fsService.changeDirectory(*admin, "/", testDir2);
        REQUIRE(rootResult != nullptr);
        REQUIRE(rootResult == fsRepo->getRootDirectory());

        auto nonExistent = fsService.changeDirectory(*admin, "/nonexistent", currentDir);
        REQUIRE(nonExistent == nullptr);

        fsService.createDirectory(*admin, "/restricted");
        auto noPermResult = fsService.changeDirectory(*testUser, "/restricted", currentDir);
        REQUIRE(noPermResult == nullptr);

        fsService.createDirectory(*admin, "/dir1");
        fsService.createDirectory(*admin, "/dir1/dir2");

        fsService.changePermissions(testUser->getId(), SubjectType::User, "/dir1", perms);
        fsService.changePermissions(testUser->getId(), SubjectType::User, "/dir1/dir2", perms);

        auto dir1 = fsService.changeDirectory(*testUser, "dir1", currentDir);
        REQUIRE(dir1 != nullptr);

        auto dir2 = fsService.changeDirectory(*testUser, "dir2", dir1);
        REQUIRE(dir2 != nullptr);

        fsService.createFile(*admin, "/testfile.txt", "content");
        auto fileResult = fsService.changeDirectory(*admin, "/testfile.txt", currentDir);
        REQUIRE(fileResult == nullptr);
    }

    SECTION("createFile_save_failure") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();

        auto mockFsRepo = std::make_unique<MockFileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *mockFsRepo);
        FileSystemService fsService(*mockFsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *mockFsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(mockFsRepo->getRootDirectory());

        mockFsRepo->setShouldFailSave(true);
        auto parentDir = mockFsRepo->getRootDirectory();
        auto file = fsService.createFile(*admin, "/testfile.txt", "content");
        REQUIRE(file == nullptr);
        auto child = parentDir->getChild("testfile.txt");
        REQUIRE(child == nullptr);

        mockFsRepo->setShouldFailSave(false);
        auto successFile = fsService.createFile(*admin, "/testfile.txt", "content");
        REQUIRE(successFile != nullptr);
        REQUIRE(mockFsRepo->pathExists("/testfile.txt"));
    }

    SECTION("changeOwner") {
        auto userRepo = std::make_unique<UserRepository>();
        auto groupRepo = std::make_unique<GroupRepository>();
        auto fsRepo = std::make_unique<FileSystemRepository>();
        auto securityService = std::make_unique<SecurityService>(*userRepo, *groupRepo);
        auto sessionService = std::make_unique<SessionService>(*securityService, *fsRepo);
        FileSystemService fsService(*fsRepo, *securityService, *sessionService);

        User* admin = nullptr;
        User* testUser = nullptr;
        initializeTestEnvironment(*userRepo, *groupRepo, *fsRepo, *securityService, admin, testUser);

        sessionService->setCurrentUser(admin);
        sessionService->setCurrentDirectory(fsRepo->getRootDirectory());

        bool nonExistent = fsService.changeOwner(*admin, "/nonexistent.txt", "newOwner");
        REQUIRE_FALSE(nonExistent);

        fsService.createFile(*admin, "/ownerfile.txt", "content");
        sessionService->setCurrentUser(testUser);
        bool noPerm = fsService.changeOwner(*testUser, "/ownerfile.txt", "newOwner");
        REQUIRE_FALSE(noPerm);

        sessionService->setCurrentUser(admin);
        std::map<PermissionType, PermissionEffect> perms;
        perms[PermissionType::ChangePermissions] = PermissionEffect::Allow;
        fsService.changePermissions(testUser->getId(), SubjectType::User, "/ownerfile.txt", perms);
        sessionService->setCurrentUser(testUser);
        bool nonExistentUser = fsService.changeOwner(*testUser, "/ownerfile.txt", "nonExistentUser");
        REQUIRE_FALSE(nonExistentUser);

        auto otherUser = std::make_unique<User>(3, "otherUser");
        userRepo->saveUser(std::move(otherUser));
        sessionService->setCurrentUser(admin);
        fsService.changePermissions(testUser->getId(), SubjectType::User, "/ownerfile.txt", perms);
        sessionService->setCurrentUser(testUser);
        bool unimplemented = fsService.changeOwner(*testUser, "/ownerfile.txt", "otherUser");
        REQUIRE_FALSE(unimplemented);

        fsService.createDirectory(*admin, "/ownerDir");
        bool dirResult = fsService.changeOwner(*admin, "/ownerDir", "testUser");
        REQUIRE_FALSE(dirResult);
    }
}