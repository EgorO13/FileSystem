#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include "Entity/ACL/acl_class.h"
#include "base.h"

TEST_CASE("ACL") {
    ACL acl(1);

    SECTION("Установка и проверка одного разрешения") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        
        std::vector<unsigned int> userGroups = {};
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Read));
        REQUIRE_FALSE(acl.checkPermission(2, userGroups, PermissionType::Write));
        REQUIRE_FALSE(acl.checkPermission(3, userGroups, PermissionType::Read));
    }

    SECTION("Установка нескольких разрешений") {
        std::vector<PermissionType> perms = {PermissionType::Read, PermissionType::Write};
        acl.setPermissions(2, SubjectType::User, perms, PermissionEffect::Allow);

        std::vector<unsigned int> userGroups = {};
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Read));
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Write));
        REQUIRE_FALSE(acl.checkPermission(2, userGroups, PermissionType::Execute));
    }

    SECTION("Явные запреты имеют приоритет") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Deny);

        std::vector<unsigned int> userGroups = {};
        REQUIRE_FALSE(acl.checkPermission(2, userGroups, PermissionType::Read));
    }

    SECTION("Владелец с явным запретом через hasExplicitDeny") {
        acl.setPermission(1, SubjectType::User, PermissionType::Write, PermissionEffect::Deny);
        acl.setPermission(1, SubjectType::User, PermissionType::Execute, PermissionEffect::Deny);
        std::vector<unsigned int> userGroups = {};
        REQUIRE_FALSE(acl.checkPermission(1, userGroups, PermissionType::Write));
        REQUIRE_FALSE(acl.checkPermission(1, userGroups, PermissionType::Execute));
        REQUIRE(acl.checkPermission(1, userGroups, PermissionType::Read));
        REQUIRE(acl.checkPermission(1, userGroups, PermissionType::ChangePermissions));
    }

    SECTION("hasExplicitDeny через группы владельца") {
        acl.setPermission(100, SubjectType::Group, PermissionType::Write, PermissionEffect::Deny);
        acl.setPermission(101, SubjectType::Group, PermissionType::Execute, PermissionEffect::Allow);
        std::vector<unsigned int> ownerGroups = {100, 101};
        REQUIRE_FALSE(acl.checkPermission(1, ownerGroups, PermissionType::Write));
        REQUIRE(acl.checkPermission(1, ownerGroups, PermissionType::Execute));
        REQUIRE(acl.checkPermission(1, ownerGroups, PermissionType::Read));
    }

    SECTION("Права для групп") {
        acl.setPermission(100, SubjectType::Group, PermissionType::Read, PermissionEffect::Allow);

        std::vector<unsigned int> userGroups = {100};
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Read));

        std::vector<unsigned int> emptyGroups = {};
        REQUIRE_FALSE(acl.checkPermission(2, emptyGroups, PermissionType::Read));
    }

    SECTION("Владелец имеет все права") {
        std::vector<unsigned int> userGroups = {};
        REQUIRE(acl.checkPermission(1, userGroups, PermissionType::Read));
        REQUIRE(acl.checkPermission(1, userGroups, PermissionType::Write));
        REQUIRE(acl.checkPermission(1, userGroups, PermissionType::Execute));
    }

    SECTION("Удаление разрешений") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(2, SubjectType::User, PermissionType::Write, PermissionEffect::Allow);

        std::vector<unsigned int> userGroups = {};
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Read));
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Write));

        acl.removePermission(2, SubjectType::User, PermissionType::Read);

        REQUIRE_FALSE(acl.checkPermission(2, userGroups, PermissionType::Read));
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Write));
    }

    SECTION("Приоритет запрета над разрешением") {
        acl.setPermission(100, SubjectType::Group, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Deny);
        std::vector<unsigned int> userGroups = {100};
        REQUIRE_FALSE(acl.checkPermission(2, userGroups, PermissionType::Read));
    }

    SECTION("Получение всех эффективных разрешений") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(100, SubjectType::Group, PermissionType::Write, PermissionEffect::Allow);

        std::vector<unsigned int> userGroups = {100};
        auto perms = acl.getEffectivePermissions(2, userGroups);

        REQUIRE(perms[PermissionType::Read] == true);
        REQUIRE(perms[PermissionType::Write] == true);
        REQUIRE(perms[PermissionType::Execute] == false);
        REQUIRE(perms[PermissionType::ChangePermissions] == false);
    }

    SECTION("Получение и установка всех записей") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(3, SubjectType::User, PermissionType::Write, PermissionEffect::Allow);
        acl.setPermission(100, SubjectType::Group, PermissionType::Execute, PermissionEffect::Allow);
        auto entries = acl.getEntries();
        REQUIRE(entries.size() == 3);
        ACL newAcl(1);
        newAcl.setEntries(entries);
        std::vector<unsigned int> userGroups = {100};
        REQUIRE(newAcl.checkPermission(2, userGroups, PermissionType::Read));
        REQUIRE(newAcl.checkPermission(3, userGroups, PermissionType::Write));
        REQUIRE(newAcl.checkPermission(4, userGroups, PermissionType::Execute));
    }

    SECTION("Изменение владельца") {
        REQUIRE(acl.getOwner() == 1);
        acl.setOwnerId(2);
        REQUIRE(acl.getOwner() == 2);
        std::vector<unsigned int> userGroups = {};
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Read));

        REQUIRE_FALSE(acl.checkPermission(1, userGroups, PermissionType::Read));
    }

    SECTION("Сложная иерархия прав") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(100, SubjectType::Group, PermissionType::Write, PermissionEffect::Allow);
        acl.setPermission(101, SubjectType::Group, PermissionType::Execute, PermissionEffect::Deny);
        std::vector<unsigned int> user3Groups = {100};
        REQUIRE_FALSE(acl.checkPermission(3, user3Groups, PermissionType::Read));
        REQUIRE(acl.checkPermission(3, user3Groups, PermissionType::Write));
        REQUIRE_FALSE(acl.checkPermission(3, user3Groups, PermissionType::Execute));
        std::vector<unsigned int> user2Groups = {100, 101};
        REQUIRE(acl.checkPermission(2, user2Groups, PermissionType::Read));
        REQUIRE(acl.checkPermission(2, user2Groups, PermissionType::Write));
        REQUIRE_FALSE(acl.checkPermission(2, user2Groups, PermissionType::Execute));
    }

    SECTION("Очистка прав при установке пустых записей") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(3, SubjectType::User, PermissionType::Write, PermissionEffect::Allow);
        acl.setEntries({});
        std::vector<unsigned int> userGroups = {};
        REQUIRE_FALSE(acl.checkPermission(2, userGroups, PermissionType::Read));
        REQUIRE_FALSE(acl.checkPermission(3, userGroups, PermissionType::Write));
    }

    SECTION("Проверка пустых групп") {
        acl.setPermission(100, SubjectType::Group, PermissionType::Read, PermissionEffect::Allow);

        std::vector<unsigned int> emptyGroups = {};
        REQUIRE_FALSE(acl.checkPermission(999, emptyGroups, PermissionType::Read));
    }

    SECTION("Конфликтующие разрешения для одного пользователя") {
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(2, SubjectType::User, PermissionType::Read, PermissionEffect::Deny);
        acl.setPermission(2, SubjectType::User, PermissionType::Write, PermissionEffect::Allow);

        std::vector<unsigned int> userGroups = {};
        REQUIRE_FALSE(acl.checkPermission(2, userGroups, PermissionType::Read));
        REQUIRE(acl.checkPermission(2, userGroups, PermissionType::Write));
    }

    SECTION("Множественные группы с конфликтующими правами") {
        acl.setPermission(100, SubjectType::Group, PermissionType::Read, PermissionEffect::Allow);
        acl.setPermission(101, SubjectType::Group, PermissionType::Read, PermissionEffect::Deny);
        acl.setPermission(102, SubjectType::Group, PermissionType::Write, PermissionEffect::Allow);

        std::vector<unsigned int> userGroups = {100, 101, 102};
        REQUIRE_FALSE(acl.checkPermission(5, userGroups, PermissionType::Read));
        REQUIRE(acl.checkPermission(5, userGroups, PermissionType::Write));
    }

    SECTION("getPermissionEffect возвращает Deny для несуществующего разрешения") {
        ACLEntry entry;
        entry.subjectId = 1;
        entry.subjectType = SubjectType::User;

        REQUIRE(entry.getPermissionEffect(PermissionType::Read) == PermissionEffect::Deny);
        REQUIRE(entry.getPermissionEffect(PermissionType::Write) == PermissionEffect::Deny);
        REQUIRE(entry.getPermissionEffect(PermissionType::Execute) == PermissionEffect::Deny);
        REQUIRE(entry.getPermissionEffect(PermissionType::ModifyMetadata) == PermissionEffect::Deny);
        REQUIRE(entry.getPermissionEffect(PermissionType::ChangePermissions) == PermissionEffect::Deny);

        entry.setPermission(PermissionType::Read, PermissionEffect::Allow);
        REQUIRE(entry.getPermissionEffect(PermissionType::Read) == PermissionEffect::Allow);
        REQUIRE(entry.getPermissionEffect(PermissionType::Write) == PermissionEffect::Deny);
    }

    SECTION("Конструктор с нулевым владельцем") {
        ACL acl(0);
        REQUIRE(acl.getOwner() == 0);
        std::vector<unsigned int> groups = {};
        REQUIRE(acl.checkPermission(0, groups, PermissionType::Read));
    }

    SECTION("Обработка несуществующих разрешений") {
        ACL acl(1);
        acl.removePermission(999, SubjectType::User, PermissionType::Read);
        REQUIRE(acl.getEntries().empty());
    }

    SECTION("Дублирование записей при setEntries") {
        ACL acl(1);
        std::vector<ACLEntry> entries;
        ACLEntry entry1;
        entry1.subjectId = 2;
        entry1.subjectType = SubjectType::User;
        entry1.setPermission(PermissionType::Read, PermissionEffect::Allow);
        entries.push_back(entry1);
        entries.push_back(entry1);
        acl.setEntries(entries);
        REQUIRE(acl.getEntries().size() == 1);
    }
}