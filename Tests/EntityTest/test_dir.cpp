#include <catch2/catch_all.hpp>

#include "../../Entity/Directory/realisation/directory_descriptor.h"
#include "../../Entity/File/realisation/file_descriptor.h"
#include "../../Entity/User/user.h"

TEST_CASE("DirectoryDescriptor") {
    User owner(1, "test_user");
    
    SECTION("Создание директории") {
        DirectoryDescriptor dir("test_dir", 0, owner, 200);

        REQUIRE(dir.getName() == "test_dir");
        REQUIRE(dir.getAddress() == 200);
        REQUIRE(dir.getOwner().getId() == 1);
        REQUIRE(dir.getChildCount() == 0);
    }
    
    SECTION("Добавление и получение дочерних элементов") {
        DirectoryDescriptor dir("parent", 0, owner, 200);
        FileDescriptor file("test.txt", dir.getAddress(), owner, 100);
        DirectoryDescriptor childDir("child", dir.getAddress(), owner, 201);

        REQUIRE(dir.addChild(&file));
        REQUIRE(dir.addChild(&childDir));
        REQUIRE(dir.getChildCount() == 2);

        auto children = dir.listChild();
        REQUIRE(children.size() == 2);

        auto foundFile = dir.getChild("test.txt");
        REQUIRE(foundFile != nullptr);
        REQUIRE(foundFile->getName() == "test.txt");

        auto foundDir = dir.getChild("child");
        REQUIRE(foundDir != nullptr);
        REQUIRE(foundDir->getName() == "child");
    }

    SECTION("Дублирование дочернего элемента") {
        DirectoryDescriptor dir("parent", 0, owner, 200);
        FileDescriptor file("test.txt", dir.getAddress(), owner, 100);

        REQUIRE(dir.addChild(&file));
        REQUIRE_FALSE(dir.addChild(&file));

        FileDescriptor file2("test.txt", dir.getAddress(), owner, 101);
        REQUIRE_FALSE(dir.addChild(&file2));
    }

    SECTION("Удаление дочернего элемента") {
        DirectoryDescriptor dir("parent", 0, owner, 200);
        FileDescriptor file("test.txt", dir.getAddress(), owner, 100);

        REQUIRE(dir.addChild(&file));
        REQUIRE(dir.getChildCount() == 1);

        REQUIRE(dir.removeChild("test.txt"));
        REQUIRE(dir.getChildCount() == 0);
        REQUIRE(dir.getChild("test.txt") == nullptr);
    }

    SECTION("Поиск несуществующего элемента") {
        DirectoryDescriptor dir("parent", 0, owner, 200);

        REQUIRE(dir.getChild("nonexistent") == nullptr);
        REQUIRE_FALSE(dir.removeChild("nonexistent"));
    }

    SECTION("Проверка наличия дочернего элемента") {
        DirectoryDescriptor dir("parent", 0, owner, 200);
        FileDescriptor file("test.txt", dir.getAddress(), owner, 100);

        REQUIRE_FALSE(dir.containChild("test.txt"));
        REQUIRE(dir.addChild(&file));
        REQUIRE(dir.containChild("test.txt"));
    }
}