#include <catch2/catch_all.hpp>
#include "../Entity/File/realisation/file_descriptor.h"
#include "../Entity/User/user.h"
#include "../base.h"

TEST_CASE("FileDescriptor") {
    User owner(1, "test_user");

    SECTION("Создание файла") {
        FileDescriptor file("test", 0, owner, 100);

        REQUIRE(file.getName() == "test");
        REQUIRE(file.getAddress() == 100);
        REQUIRE(file.getOwner().getId() == 1);
        REQUIRE(file.getSize() == 0);
        REQUIRE(file.getParentDirectoryAddress() == 0);
        REQUIRE(file.getCreateTime() <= std::chrono::system_clock::now());
        REQUIRE(file.getLastModifyTime() <= std::chrono::system_clock::now());
    }

    SECTION("Запись и чтение содержимого") {
        FileDescriptor file("test", 0, owner, 100);

        REQUIRE(file.writeContent("Hello, World!"));
        REQUIRE(file.readContent() == "Hello, World!");
        REQUIRE(file.getSize() == 13);

        REQUIRE(file.writeContent("New content"));
        REQUIRE(file.readContent() == "New content");
        REQUIRE(file.getSize() == 11);
    }

    SECTION("Очистка содержимого") {
        FileDescriptor file("test", 0, owner, 100);

        REQUIRE(file.writeContent("Some content"));
        REQUIRE(file.readContent() == "Some content");

        file.clearContent();
        REQUIRE(file.readContent().empty());
        REQUIRE(file.getSize() == 0);
    }

    SECTION("Смена владельца") {
        User newOwner(2, "new_owner");
        FileDescriptor file("test", 0, owner, 100);

        file.setOwner(newOwner);
        REQUIRE(file.getOwner().getId() == 2);
        REQUIRE(file.getOwner().getName() == "new_owner");
    }

    SECTION("Смена адреса родительской директории") {
        FileDescriptor fs("test", 0, owner, 100);
        REQUIRE(fs.getParentDirectoryAddress() == 0);
        auto old_modify_time = fs.getLastModifyTime();
        fs.setParentDirectoryAddress(500);
        REQUIRE(fs.getParentDirectoryAddress() == 500);
        REQUIRE(fs.getLastModifyTime() >= old_modify_time);
    }

    SECTION("Смена адреса объекта") {
        FileDescriptor fs("test", 0, owner, 100);

        REQUIRE(fs.getAddress() == 100);

        fs.setAddress(200);
        REQUIRE(fs.getAddress() == 200);
    }

    SECTION("Установка и проверка разрешений") {
        FileDescriptor file("test", 0, owner, 100);

        std::vector<PermissionType> perms = {PermissionType::Read, PermissionType::Write};
        file.setPermissions(owner.getId(), SubjectType::User, perms, PermissionEffect::Allow);

        std::vector<unsigned int> groupIds = {};
        REQUIRE(file.checkPermission(owner.getId(), groupIds, PermissionType::Read));
        REQUIRE(file.checkPermission(owner.getId(), groupIds, PermissionType::Write));
        REQUIRE(file.checkPermission(owner.getId(), groupIds, PermissionType::Execute));
    }

    SECTION("Усечение содержимого") {
        FileDescriptor file("test", 0, owner, 100);
        REQUIRE(file.writeContent("Hello, World!"));
        REQUIRE(file.truncateContent(5));
        REQUIRE(file.readContent() == "Hello");
        REQUIRE(file.getSize() == 5);
        REQUIRE(file.truncateContent(0));
        REQUIRE(file.readContent().empty());
        REQUIRE(file.getSize() == 0);

        REQUIRE_FALSE(file.truncateContent(-1));
        REQUIRE_FALSE(file.truncateContent(999));
    }

    SECTION("Запись и чтение без проверки разрешений") {
        FileDescriptor file("test", 0, owner, 100);

        REQUIRE(file.writeContentAlways("Always writable"));
        REQUIRE(file.readContentAlways() == "Always writable");
        REQUIRE(file.getSize() == 15);
    }

    SECTION("Блокировка файла") {
        FileDescriptor file("test", 0, owner, 100);
        file.setMode(Lock::ReadLock);
        REQUIRE_FALSE(file.isReadable());
        file.setMode(Lock::NotLock);
        REQUIRE(file.isReadable());
        REQUIRE(file.isWritable());
        file.setMode(Lock::WriteLock);
        REQUIRE_FALSE(file.isWritable());
    }

    SECTION("Смена имени файла") {
        FileDescriptor file("test", 0, owner, 100);

        REQUIRE(file.setName("new_name"));
        REQUIRE(file.getName() == "new_name");

        REQUIRE_FALSE(file.setName(""));
        REQUIRE(file.getName() == "new_name");
    }

    SECTION("Проверка временных меток") {
        auto start_time = std::chrono::system_clock::now();
        FileDescriptor file("test", 0, owner, 100);

        REQUIRE(file.getCreateTime() >= start_time);
        REQUIRE(file.getLastModifyTime() >= start_time);

        auto before_write = std::chrono::system_clock::now();
        file.writeContent("test");
        REQUIRE(file.getLastModifyTime() >= before_write);

        auto custom_time = std::chrono::system_clock::now() - std::chrono::hours(1);
        file.setCreateTime(custom_time);
        file.setLastModifyTime(custom_time);
        REQUIRE(file.getCreateTime() == custom_time);
        REQUIRE(file.getLastModifyTime() == custom_time);
    }

    SECTION("Проверка ACL") {
        FileDescriptor file("test", 0, owner, 100);

        std::vector<PermissionType> perms = {PermissionType::Read};
        file.setPermissions(2, SubjectType::User, perms, PermissionEffect::Allow);
        file.setPermission(2, SubjectType::User, PermissionType::Write, PermissionEffect::Allow);

        auto acl = file.getACL();
        REQUIRE(acl.size() == 1);
        REQUIRE(acl[0].subjectId == 2);
        REQUIRE(acl[0].subjectType == SubjectType::User);

        std::vector<ACLEntry> newAcl;
        ACLEntry entry;
        entry.subjectId = 3;
        entry.subjectType = SubjectType::Group;
        entry.setPermission(PermissionType::Execute, PermissionEffect::Allow);
        newAcl.push_back(entry);

        file.setACL(newAcl);
        auto updatedAcl = file.getACL();
        REQUIRE(updatedAcl.size() == 1);
        REQUIRE(updatedAcl[0].subjectId == 3);
    }

    SECTION("Обработка исключений при отсутствии прав") {
        User other(2, "other");
        FileDescriptor file("test", 0, owner, 100);
        file.setMode(Lock::WriteLock);
        REQUIRE_THROWS_AS(file.writeContent("test"), std::runtime_error);
        file.writeContentAlways("initial");
        REQUIRE(file.readContent() == "initial");
        REQUIRE_THROWS_AS(file.clearContent(), std::runtime_error);
    }
}

TEST_CASE("FileDescriptor Edge Cases") {
    User owner(1, "owner");

    SECTION("Большой контент") {
        FileDescriptor file("large", 0, owner, 100);
        std::string large_content(10000, 'A');

        REQUIRE(file.writeContent(large_content));
        REQUIRE(file.getSize() == 10000);
        REQUIRE(file.readContent() == large_content);
    }

    SECTION("Пустой контент") {
        FileDescriptor file("empty", 0, owner, 100);

        REQUIRE(file.writeContent(""));
        REQUIRE(file.getSize() == 0);
        REQUIRE(file.readContent().empty());
    }

    SECTION("Специальные символы в контенте") {
        FileDescriptor file("special", 0, owner, 100);
        std::string special = "Line1\nLine2\tTab\r\nUnicode: тест";

        REQUIRE(file.writeContent(special));
        REQUIRE(file.readContent() == special);
    }

    SECTION("Многократное усечение") {
        FileDescriptor file("truncate", 0, owner, 100);

        REQUIRE(file.writeContent("1234567890"));
        REQUIRE(file.truncateContent(5));
        REQUIRE(file.readContent() == "12345");

        REQUIRE(file.truncateContent(2));
        REQUIRE(file.readContent() == "12");

        REQUIRE(file.truncateContent(0));
        REQUIRE(file.readContent().empty());
    }
}