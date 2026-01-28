#include <catch2/catch_test_macros.hpp>
#include "Service/SessionService/realisation/session_service.h"
#include "Service/SecurityService/realisation/security_service.h"
#include "Repository/UserRep/realisation/user_repository.h"
#include "Repository/GroupRep/realisation/group_repository.h"
#include "Repository/FSRep/realisation/fs_repository.h"
#include "Entity/Directory/realisation/directory_descriptor.h"

TEST_CASE("SessionService") {
    UserRepository userRepo;
    GroupRepository groupRepo;
    FileSystemRepository fsRepo;

    auto admin = std::make_unique<User>(1, "admin");
    auto regularUser = std::make_unique<User>(2, "user");

    userRepo.saveUser(std::move(admin));
    userRepo.saveUser(std::move(regularUser));

    SecurityService securityService(userRepo, groupRepo);
    SessionService sessionService(securityService, fsRepo);

    SECTION("Defoult") {
        REQUIRE(sessionService.getCurrentUser() == nullptr);
        REQUIRE(sessionService.getCurrentDirectory() == nullptr);
        REQUIRE(sessionService.isLoggedIn() == false);
    }

    SECTION("isLoggedIn") {
        REQUIRE(sessionService.login("admin") == true);
        REQUIRE(sessionService.isLoggedIn() == true);

        auto* currentUser = sessionService.getCurrentUser();
        REQUIRE(currentUser != nullptr);
        REQUIRE(currentUser->getName() == "admin");
        REQUIRE(currentUser->getId() == 1);

        REQUIRE(sessionService.getCurrentDirectory() != nullptr);
    }

    SECTION("isLoggedIn false") {
        REQUIRE(sessionService.login("nonexistent") == false);
        REQUIRE(sessionService.isLoggedIn() == false);
        REQUIRE(sessionService.getCurrentUser() == nullptr);
        REQUIRE(sessionService.login("") == false);
    }

    SECTION("logout") {
        REQUIRE(sessionService.login("admin") == true);
        REQUIRE(sessionService.isLoggedIn() == true);
        sessionService.logout();
        REQUIRE(sessionService.isLoggedIn() == false);
        REQUIRE(sessionService.getCurrentUser() == nullptr);
        REQUIRE(sessionService.getCurrentDirectory() != nullptr);
    }

    SECTION("login and logout") {
        REQUIRE(sessionService.login("admin") == true);
        REQUIRE(sessionService.getCurrentUser()->getName() == "admin");

        sessionService.logout();
        REQUIRE(sessionService.getCurrentUser() == nullptr);

        REQUIRE(sessionService.login("user") == true);
        REQUIRE(sessionService.getCurrentUser()->getName() == "user");

        sessionService.logout();
        REQUIRE(sessionService.getCurrentUser() == nullptr);

        REQUIRE(sessionService.login("admin") == true);
        REQUIRE(sessionService.getCurrentUser()->getName() == "admin");
    }

    SECTION("setCurrentUser") {
        auto* user = userRepo.getUserByName("admin");
        REQUIRE(user != nullptr);

        sessionService.setCurrentUser(user);
        REQUIRE(sessionService.isLoggedIn() == true);
        REQUIRE(sessionService.getCurrentUser() == user);
        REQUIRE(sessionService.getCurrentUser()->getName() == "admin");
    }

    SECTION("setCurrentDirectory") {
        auto* rootDir = fsRepo.getRootDirectory();
        REQUIRE(rootDir != nullptr);

        User admin(1, "admin");
        auto testDir = std::make_unique<DirectoryDescriptor>("test_dir", 0, admin, fsRepo.getAddress());
        auto* testDirPtr = testDir.get();
        fsRepo.saveObject(std::move(testDir));

        sessionService.setCurrentDirectory(testDirPtr);
        REQUIRE(sessionService.getCurrentDirectory() == testDirPtr);
        REQUIRE(dynamic_cast<IFileSystemObject*>(sessionService.getCurrentDirectory())->getName() == "test_dir");

        sessionService.setCurrentDirectory(rootDir);
        REQUIRE(sessionService.getCurrentDirectory() == rootDir);
        REQUIRE(dynamic_cast<IFileSystemObject*>(sessionService.getCurrentDirectory())->getName() == "/");
    }

    SECTION("setCurrentUser") {
        auto* user = userRepo.getUserByName("admin");
        auto* dir = fsRepo.getRootDirectory();

        sessionService.setCurrentUser(user);
        sessionService.setCurrentDirectory(dir);

        REQUIRE(sessionService.getCurrentUser() == user);
        REQUIRE(sessionService.getCurrentDirectory() == dir);

        sessionService.setCurrentUser(nullptr);
        sessionService.setCurrentDirectory(nullptr);

        REQUIRE(sessionService.getCurrentUser() == nullptr);
        REQUIRE(sessionService.getCurrentDirectory() == nullptr);
        REQUIRE(sessionService.isLoggedIn() == false);
    }

    SECTION("Session with Security") {
        REQUIRE(sessionService.login("admin") == true);

        auto* currentUser = sessionService.getCurrentUser();
        auto* securityUser = securityService.authenticate("admin");

        REQUIRE(currentUser == securityUser);
        REQUIRE(currentUser->getId() == securityUser->getId());
        REQUIRE(currentUser->getName() == securityUser->getName());
    }

    SECTION("Constructor") {
        SessionService newSessionService(securityService, fsRepo);

        REQUIRE(newSessionService.getCurrentUser() == nullptr);
        REQUIRE(newSessionService.getCurrentDirectory() == nullptr);
        REQUIRE(newSessionService.isLoggedIn() == false);
    }
}