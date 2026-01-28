#include <catch2/catch_test_macros.hpp>
#include "Service/SecurityService/realisation/security_service.h"
#include "Repository/UserRep/realisation/user_repository.h"
#include "Repository/GroupRep/realisation/group_repository.h"
#include "Entity/User/user.h"
#include "Entity/FSObject/realisation/fs_object.h"
#include "Entity/Group/group.h"

class MockFileSystemObject : public FileSystemObject {
public:
    MockFileSystemObject(const std::string& name, unsigned int address, unsigned int parentAddress, const User& owner)
        : FileSystemObject(name, parentAddress, owner, address) {}

    void testSetPermissions(unsigned int id, SubjectType s_type, const std::vector<PermissionType>& permissions, PermissionEffect effect) {
        setPermissions(id, s_type, permissions, effect);
    }
};

TEST_CASE("SecurityService") {
    UserRepository userRepo;
    GroupRepository groupRepo;
    SecurityService securityService(userRepo, groupRepo);

    auto user1 = std::make_unique<User>(1, "user1");
    auto user2 = std::make_unique<User>(2, "user2");
    userRepo.saveUser(std::move(user1));
    userRepo.saveUser(std::move(user2));

    User* user1Ptr = userRepo.getUserByName("user1");
    User* user2Ptr = userRepo.getUserByName("user2");

    SECTION("authenticate") {
        REQUIRE(securityService.authenticate("user1") != nullptr);
        REQUIRE(securityService.authenticate("nonexistent") == nullptr);
    }

    SECTION("isOwner") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.isOwner(*user1Ptr, obj) == true);
        REQUIRE(securityService.isOwner(*user2Ptr, obj) == false);
    }

    SECTION("isAdministrator") {
        auto adminGroup = std::make_unique<Group>(1, "Administrators");
        groupRepo.saveGroup(std::move(adminGroup));
        groupRepo.addUserToGroup(user1Ptr->getId(), 1);
        REQUIRE(securityService.isAdministrator(*user1Ptr) == true);
        REQUIRE(securityService.isAdministrator(*user2Ptr) == false);
    }

    SECTION("canRead") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.canRead(*user1Ptr, obj) == true);
        REQUIRE(securityService.canRead(*user2Ptr, obj) == false);
    }

    SECTION("canWrite") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.canWrite(*user1Ptr, obj) == true);
        REQUIRE(securityService.canWrite(*user2Ptr, obj) == false);
    }

    SECTION("canExecute") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.canExecute(*user1Ptr, obj) == true);
        REQUIRE(securityService.canExecute(*user2Ptr, obj) == false);
    }

    SECTION("canModify") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.canModify(*user1Ptr, obj) == true);
        REQUIRE(securityService.canModify(*user2Ptr, obj) == false);
    }

    SECTION("canModifyMetadata") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.canModifyMetadata(*user1Ptr, obj) == true);
        REQUIRE(securityService.canModifyMetadata(*user2Ptr, obj) == false);
    }

    SECTION("canChangePermissions") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.canChangePermissions(*user1Ptr, obj) == true);
        REQUIRE(securityService.canChangePermissions(*user2Ptr, obj) == false);
    }

    SECTION("checkPermission") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        REQUIRE(securityService.checkPermission(*user1Ptr, obj, PermissionType::Read) == true);
        REQUIRE(securityService.checkPermission(*user2Ptr, obj, PermissionType::Read) == false);
    }

    SECTION("getEffectivePermissions") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        auto perms = securityService.getEffectivePermissions(*user1Ptr, obj);
        REQUIRE(perms[PermissionType::Read] == true);
        REQUIRE(perms[PermissionType::Write] == true);
        REQUIRE(perms[PermissionType::Execute] == true);
        REQUIRE(perms[PermissionType::Modify] == true);
        REQUIRE(perms[PermissionType::ModifyMetadata] == true);
        REQUIRE(perms[PermissionType::ChangePermissions] == true);
    }

    SECTION("explicitPermissions") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        obj.testSetPermissions(user2Ptr->getId(), SubjectType::User, {PermissionType::Read}, PermissionEffect::Allow);
        REQUIRE(securityService.canRead(*user2Ptr, obj) == true);
        REQUIRE(securityService.canWrite(*user2Ptr, obj) == false);
    }

    SECTION("groupPermissions") {
        auto group = std::make_unique<Group>(10, "test_group");
        unsigned int groupId = group->getId();
        group->addUser(user2Ptr->getId());
        user2Ptr->addToGroup(groupId);
        groupRepo.saveGroup(std::move(group));
        groupRepo.addUserToGroup(user2Ptr->getId(), groupId);

        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        obj.testSetPermissions(groupId, SubjectType::Group, {PermissionType::Read}, PermissionEffect::Allow);
        REQUIRE(securityService.canRead(*user2Ptr, obj) == true);
    }

    SECTION("denyOverridesAllow") {
        auto group = std::make_unique<Group>(10, "test_group");
        unsigned int groupId = group->getId();
        groupRepo.saveGroup(std::move(group));
        groupRepo.addUserToGroup(user2Ptr->getId(), groupId);

        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        obj.testSetPermissions(groupId, SubjectType::Group, {PermissionType::Read}, PermissionEffect::Allow);
        obj.testSetPermissions(user2Ptr->getId(), SubjectType::User, {PermissionType::Read}, PermissionEffect::Deny);
        REQUIRE(securityService.canRead(*user2Ptr, obj) == false);
    }

    SECTION("modifyRequiresWriteAndMetadata") {
        MockFileSystemObject obj("test_txt", 1, 0, *user1Ptr);
        obj.testSetPermissions(user2Ptr->getId(), SubjectType::User,{PermissionType::Write, PermissionType::Modify}, PermissionEffect::Allow);
        REQUIRE(securityService.canModify(*user2Ptr, obj) == true);

        MockFileSystemObject obj2("test2_txt", 2, 0, *user1Ptr);
        obj2.testSetPermissions(user2Ptr->getId(), SubjectType::User,{PermissionType::Write}, PermissionEffect::Allow);
        REQUIRE(securityService.canModify(*user2Ptr, obj2) == false);
    }
}