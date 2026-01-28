#include <catch2/catch_test_macros.hpp>

#include "directory_descriptor.h"
#include "Service/SecurityService/realisation/security_service.h"
#include "Service/UserManagementService/realisation/user_management_service.h"
#include "Repository/UserRep/realisation/user_repository.h"
#include "Repository/GroupRep/realisation/group_repository.h"

TEST_CASE("UserManagementService") {
    UserRepository userRepo;
    GroupRepository groupRepo;
    SecurityService securityService(userRepo, groupRepo);
    UserManagementService service(userRepo, groupRepo, securityService);

    SECTION("createUser") {
        User owner(0, "owner");
        auto dir = std::make_unique<DirectoryDescriptor>("dir", 0, owner, 0);
        REQUIRE(service.createUser("newuser1", nullptr, false) == true);
        REQUIRE(service.userExists("newuser1") == true);
        REQUIRE(service.createUser("newuser2", dir.get(), false) == true);
        REQUIRE(service.userExists("newuser2") == true);
    }

    SECTION("createUserDuplicate") {
        REQUIRE(service.createUser("user1", nullptr, false) == true);
        REQUIRE(service.createUser("user1", nullptr, false) == false);
    }

    SECTION("deleteUser") {
        REQUIRE(service.createUser("todelete", nullptr, false) == true);
        REQUIRE(service.deleteUser("todelete") == true);
        REQUIRE(service.userExists("todelete") == false);
    }

    SECTION("deleteUserNonexistent") {
        REQUIRE(service.deleteUser("nonexistent") == false);
    }

    SECTION("modifyUser") {
        REQUIRE(service.createUser("oldname", nullptr, false) == true);
        REQUIRE(service.modifyUser("oldname", "") == true);
        REQUIRE(service.modifyUser("oldname", "newname") == true);
        REQUIRE(service.userExists("oldname") == false);
        REQUIRE(service.userExists("newname") == true);
    }

    SECTION("getUser") {
        REQUIRE(service.createUser("getuser", nullptr, false) == true);
        REQUIRE(service.getUser("getuser") != nullptr);
        REQUIRE(service.getUser("nonexistent") == nullptr);
    }

    SECTION("getAllUsers") {
        REQUIRE(service.createUser("user1", nullptr, false) == true);
        REQUIRE(service.createUser("user2", nullptr, false) == true);
        auto users = service.getAllUsers();
        REQUIRE(users.size() >= 2);
    }

    SECTION("userExists") {
        REQUIRE(service.userExists("nonexistent") == false);
        REQUIRE(service.createUser("exists", nullptr, false) == true);
        REQUIRE(service.userExists("exists") == true);
    }

    SECTION("createGroup") {
        REQUIRE(service.createGroup("newgroup") == true);
        REQUIRE(service.groupExists("newgroup") == true);
    }

    SECTION("createGroupDuplicate") {
        REQUIRE(service.createGroup("group1") == true);
        REQUIRE(service.createGroup("group1") == false);
    }

    SECTION("deleteGroup") {
        REQUIRE(service.createGroup("delgroup") == true);
        REQUIRE(service.deleteGroup("delgroup") == true);
        REQUIRE(service.groupExists("delgroup") == false);
    }

    SECTION("deleteGroupNonexistent") {
        REQUIRE(service.deleteGroup("nonexistent") == false);
    }

    SECTION("getGroup") {
        REQUIRE(service.createGroup("testgroup") == true);
        REQUIRE(service.getGroup("testgroup") != nullptr);
        REQUIRE(service.getGroup("nonexistent") == nullptr);
    }

    SECTION("getAllGroups") {
        REQUIRE(service.createGroup("group1") == true);
        REQUIRE(service.createGroup("group2") == true);
        auto groups = service.getAllGroups();
        REQUIRE(groups.size() >= 2);
    }

    SECTION("groupExists") {
        REQUIRE(service.groupExists("nonexistent") == false);
        REQUIRE(service.createGroup("exists") == true);
        REQUIRE(service.groupExists("exists") == true);
    }

    SECTION("addUserToGroup") {
        REQUIRE(service.createUser("user3", nullptr, false) == true);
        REQUIRE(service.createGroup("group3") == true);
        REQUIRE(service.addUserToGroup("user3", "group3") == true);
        REQUIRE(service.isUserInGroup("user3", "group3") == true);
    }

    SECTION("removeUserFromGroup") {
        REQUIRE(service.createUser("user4", nullptr, false) == true);
        REQUIRE(service.createGroup("group4") == true);
        REQUIRE(service.addUserToGroup("user4", "group4") == true);
        REQUIRE(service.removeUserFromGroup("user4", "group4") == true);
        REQUIRE(service.isUserInGroup("user4", "group4") == false);
    }

    SECTION("getUserGroups") {
        REQUIRE(service.createUser("user5", nullptr, false) == true);
        REQUIRE(service.createGroup("group5") == true);
        REQUIRE(service.addUserToGroup("user5", "group5") == true);
        auto groups = service.getUserGroups("user5");
        REQUIRE(groups.size() == 2);
    }

    SECTION("createAdminUser") {
        REQUIRE(service.createUser("admin", nullptr, true) == true);
        REQUIRE(service.isUserInGroup("admin", "Administrators") == true);
    }
}