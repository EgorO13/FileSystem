#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include "Repository/UserRep/realisation/user_repository.h"
#include "Entity/User/user.h"

TEST_CASE("UserRepository") {
    UserRepository repo;

    SECTION("Конструктор и инициализация") {
        REQUIRE(repo.getAllUsers().empty());
        REQUIRE(repo.getNextId() == 1);
    }

    SECTION("Save, Exists") {
        auto user1 = std::make_unique<User>(repo.getNextId(), "user1");
        unsigned int id1 = user1->getId();
        REQUIRE(repo.saveUser(std::move(user1)));
        REQUIRE(repo.userExists(id1));
        REQUIRE(repo.userExists("user1"));
        auto user2 = std::make_unique<User>(repo.getNextId(), "user2");
        unsigned int id2 = user2->getId();
        REQUIRE(repo.saveUser(std::move(user2)));
        REQUIRE(repo.userExists(id2));
        REQUIRE(repo.userExists("user2"));
    }

    SECTION("Save nullptr") {
        REQUIRE_FALSE(repo.saveUser(nullptr));
    }

    SECTION("Save "" ") {
        auto user = std::make_unique<User>(repo.getNextId(), "");
        REQUIRE_FALSE(repo.saveUser(std::move(user)));
    }

    SECTION("Save с существующим ID") {
        auto user1 = std::make_unique<User>(1, "user1");
        REQUIRE(repo.saveUser(std::move(user1)));

        auto user2 = std::make_unique<User>(1, "user2");
        REQUIRE_FALSE(repo.saveUser(std::move(user2)));
    }

    SECTION("Save с существующим именем") {
        auto user1 = std::make_unique<User>(repo.getNextId(), "duplicate");
        REQUIRE(repo.saveUser(std::move(user1)));

        auto user2 = std::make_unique<User>(repo.getNextId(), "duplicate");
        REQUIRE_FALSE(repo.saveUser(std::move(user2)));
    }

    SECTION("getUserById") {
        auto user = std::make_unique<User>(repo.getNextId(), "test_user");
        unsigned int userId = user->getId();
        REQUIRE(repo.saveUser(std::move(user)));

        auto* retrieved = repo.getUserById(userId);
        REQUIRE(retrieved != nullptr);
        REQUIRE(retrieved->getId() == userId);
        REQUIRE(retrieved->getName() == "test_user");
        REQUIRE(repo.getUserById(9999) == nullptr);
        REQUIRE(repo.getUserById(0) == nullptr);
    }

    SECTION("getUserByName") {
        auto user = std::make_unique<User>(repo.getNextId(), "test_user");
        unsigned int userId = user->getId();
        REQUIRE(repo.saveUser(std::move(user)));

        auto* retrieved = repo.getUserByName("test_user");
        REQUIRE(retrieved != nullptr);
        REQUIRE(retrieved->getId() == userId);
        REQUIRE(retrieved->getName() == "test_user");
        REQUIRE(repo.getUserByName("nonexistent") == nullptr);
        REQUIRE(repo.getUserByName("") == nullptr);
    }

    SECTION("getAllUsers") {
        REQUIRE(repo.getAllUsers().empty());
        auto user1 = std::make_unique<User>(repo.getNextId(), "user1");
        auto user2 = std::make_unique<User>(repo.getNextId(), "user2");
        auto user3 = std::make_unique<User>(repo.getNextId(), "user3");
        unsigned int id1 = user1->getId();
        unsigned int id2 = user2->getId();
        unsigned int id3 = user3->getId();
        REQUIRE(repo.saveUser(std::move(user1)));
        REQUIRE(repo.saveUser(std::move(user2)));
        REQUIRE(repo.saveUser(std::move(user3)));
        auto allUsers = repo.getAllUsers();
        REQUIRE(allUsers.size() == 3);
        std::set<unsigned int> foundIds;
        std::set<std::string> foundNames;
        for (const auto* user : allUsers) {
            REQUIRE(user != nullptr);
            foundIds.insert(user->getId());
            foundNames.insert(user->getName());
        }
        REQUIRE(foundIds.count(id1) == 1);
        REQUIRE(foundIds.count(id2) == 1);
        REQUIRE(foundIds.count(id3) == 1);
        REQUIRE(foundNames.count("user1") == 1);
        REQUIRE(foundNames.count("user2") == 1);
        REQUIRE(foundNames.count("user3") == 1);
    }

    SECTION("deleteUser") {
        auto user = std::make_unique<User>(repo.getNextId(), "to_delete");
        unsigned int userId = user->getId();
        REQUIRE(repo.saveUser(std::move(user)));
        REQUIRE(repo.userExists(userId));
        REQUIRE(repo.userExists("to_delete"));
        REQUIRE(repo.deleteUser(userId));
        REQUIRE_FALSE(repo.userExists(userId));
        REQUIRE_FALSE(repo.userExists("to_delete"));
        REQUIRE(repo.getUserById(userId) == nullptr);
        REQUIRE(repo.getUserByName("to_delete") == nullptr);
        REQUIRE_FALSE(repo.deleteUser(9999));
        REQUIRE_FALSE(repo.deleteUser(0));
        REQUIRE_FALSE(repo.deleteUser(userId));
    }

    SECTION("userExists") {
        auto user = std::make_unique<User>(repo.getNextId(), "existing");
        unsigned int userId = user->getId();
        REQUIRE(repo.saveUser(std::move(user)));
        REQUIRE(repo.userExists(userId));
        REQUIRE_FALSE(repo.userExists(9999));
        REQUIRE_FALSE(repo.userExists(0));
        REQUIRE(repo.userExists("existing"));
        REQUIRE_FALSE(repo.userExists("nonexistent"));
        REQUIRE_FALSE(repo.userExists(""));
    }

    SECTION("authenticate") {
        auto user = std::make_unique<User>(repo.getNextId(), "testuser");
        unsigned int userId = user->getId();
        REQUIRE(repo.saveUser(std::move(user)));
        auto* authenticated = repo.authenticate("testuser");
        REQUIRE(authenticated != nullptr);
        REQUIRE(authenticated->getId() == userId);
        REQUIRE(authenticated->getName() == "testuser");
        REQUIRE(repo.authenticate("nonexistent") == nullptr);
        REQUIRE(repo.authenticate("") == nullptr);
        REQUIRE(repo.authenticate("TESTUSER") == nullptr);
        REQUIRE(repo.authenticate("TestUser") == nullptr);
    }

    SECTION("getNextId") {
        unsigned int firstId = repo.getNextId();
        REQUIRE(firstId == 1);
        auto user1 = std::make_unique<User>(firstId, "user1");
        REQUIRE(repo.saveUser(std::move(user1)));
        unsigned int secondId = repo.getNextId();
        REQUIRE(secondId == 2);
        auto user2 = std::make_unique<User>(secondId, "user2");
        REQUIRE(repo.saveUser(std::move(user2)));
        unsigned int thirdId = repo.getNextId();
        REQUIRE(thirdId == 3);
    }

    SECTION("clear") {
        auto user1 = std::make_unique<User>(repo.getNextId(), "user1");
        auto user2 = std::make_unique<User>(repo.getNextId(), "user2");
        unsigned int id1 = user1->getId();
        unsigned int id2 = user2->getId();
        REQUIRE(repo.saveUser(std::move(user1)));
        REQUIRE(repo.saveUser(std::move(user2)));
        REQUIRE(repo.getAllUsers().size() == 2);
        REQUIRE(repo.userExists(id1));
        REQUIRE(repo.userExists(id2));
        REQUIRE(repo.userExists("user1"));
        REQUIRE(repo.userExists("user2"));
        repo.clear();
        REQUIRE(repo.getAllUsers().empty());
        REQUIRE_FALSE(repo.userExists(id1));
        REQUIRE_FALSE(repo.userExists(id2));
        REQUIRE_FALSE(repo.userExists("user1"));
        REQUIRE_FALSE(repo.userExists("user2"));
        REQUIRE(repo.authenticate("user1") == nullptr);
        REQUIRE(repo.authenticate("user2") == nullptr);
        unsigned int nextId = repo.getNextId();
        REQUIRE(nextId == 1);
    }

    SECTION("Пользователи с различными именами") {
        std::vector<std::string> testNames = {
            "user1",
            "user_2",
            "user-name",
            "user.name",
            "user123",
            "UserWithCaps",
            "user with spaces",
            "user\twith\ttabs",
            "user\nwith\nnewlines",
        };

        for (const auto& name : testNames) {
            auto user = std::make_unique<User>(repo.getNextId(), name);
            bool saved = repo.saveUser(std::move(user));
            REQUIRE_NOTHROW(repo.saveUser(std::make_unique<User>(repo.getNextId(), name)));
        }
    }

    SECTION("Работа с большим количеством пользователей") {
        const int NUM_USERS = 100;
        for (int i = 0; i < NUM_USERS; ++i) {
            std::string username = "user" + std::to_string(i);
            auto user = std::make_unique<User>(repo.getNextId(), username);
            REQUIRE(repo.saveUser(std::move(user)));
        }
        auto allUsers = repo.getAllUsers();
        REQUIRE(allUsers.size() == NUM_USERS);
        unsigned int nextId = repo.getNextId();
        REQUIRE(nextId == NUM_USERS + 1);
    }

    SECTION("Порядок пользователей при получении всех") {
        auto user3 = std::make_unique<User>(3, "user3");
        auto user1 = std::make_unique<User>(1, "user1");
        auto user2 = std::make_unique<User>(2, "user2");
        REQUIRE(repo.saveUser(std::move(user3)));
        REQUIRE(repo.saveUser(std::move(user1)));
        REQUIRE(repo.saveUser(std::move(user2)));
        auto allUsers = repo.getAllUsers();
        REQUIRE(allUsers.size() == 3);
        REQUIRE(allUsers[0]->getId() == 1);
        REQUIRE(allUsers[0]->getName() == "user1");
        REQUIRE(allUsers[1]->getId() == 2);
        REQUIRE(allUsers[1]->getName() == "user2");
        REQUIRE(allUsers[2]->getId() == 3);
        REQUIRE(allUsers[2]->getName() == "user3");
    }

    SECTION("Обновление nextId при сохранении пользователя с большим ID") {
        auto user = std::make_unique<User>(100, "user_with_high_id");
        REQUIRE(repo.saveUser(std::move(user)));
        unsigned int nextId = repo.getNextId();
        REQUIRE(nextId == 101);
    }

    SECTION("Повторное использование удаленного ID") {
        auto user1 = std::make_unique<User>(repo.getNextId(), "user1");
        unsigned int id1 = user1->getId();
        REQUIRE(repo.saveUser(std::move(user1)));
        REQUIRE(repo.deleteUser(id1));
        auto user2 = std::make_unique<User>(id1, "user2");
        REQUIRE(repo.saveUser(std::move(user2)));
        REQUIRE(repo.userExists(id1));
        REQUIRE(repo.getUserById(id1)->getName() == "user2");
    }
}