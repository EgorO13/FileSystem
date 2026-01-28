#include <catch2/catch_all.hpp>
#include "Entity/User/user.h"

TEST_CASE("User") {
    SECTION("Конструкторы") {
        User u1;
        REQUIRE(u1.getId() == 0);
        REQUIRE(u1.getName() == "guest");
        
        User u2(1, "admin");
        REQUIRE(u2.getId() == 1);
        REQUIRE(u2.getName() == "admin");
        
        User u3 = u2;
        REQUIRE(u3.getId() == 1);
        REQUIRE(u3.getName() == "admin");
        
        User u4;
        u4 = u2;
        REQUIRE(u4.getId() == 1);
        REQUIRE(u4.getName() == "admin");
    }
    
    SECTION("Методы доступа") {
        User u(1, "test");
        
        REQUIRE(u.getId() == 1);
        REQUIRE(u.getName() == "test");
        
        u.setId(2);
        u.setName("new_name");
        REQUIRE(u.getId() == 2);
        REQUIRE(u.getName() == "new_name");
    }
    
    SECTION("Управление группами") {
        User u(1, "user");
        
        REQUIRE(u.getGroups().empty());
        
        u.addToGroup(100);
        REQUIRE(u.isInGroup(100));
        REQUIRE(u.getGroups().size() == 1);
        REQUIRE(u.getGroups()[0] == 100);
        
        u.addToGroup(100);
        REQUIRE(u.getGroups().size() == 1);
        
        u.addToGroup(200);
        u.addToGroup(300);
        REQUIRE(u.getGroups().size() == 3);
        
        u.removeFromGroup(200);
        REQUIRE_FALSE(u.isInGroup(200));
        REQUIRE(u.getGroups().size() == 2);
        
        u.removeFromGroup(999);
        REQUIRE(u.getGroups().size() == 2);
    }
    
    SECTION("Оператор сравнения") {
        User u1(1, "user1");
        User u2(1, "user1");
        User u3(2, "user2");
        User u4(1, "user2");
        
        REQUIRE(u1 == u2);
        REQUIRE_FALSE(u1 == u3);
        REQUIRE_FALSE(u1 == u4);
        
        REQUIRE(u1 == u1);
    }
    
    SECTION("Интеграция с Group") {
        User u1(1, "user1");
        User u2(2, "user2");
        
        u1.addToGroup(100);
        u1.addToGroup(200);
        
        u2.addToGroup(100);
        u2.addToGroup(300);
        
        REQUIRE(u1.isInGroup(100));
        REQUIRE(u1.isInGroup(200));
        REQUIRE_FALSE(u1.isInGroup(300));
        
        REQUIRE(u2.isInGroup(100));
        REQUIRE_FALSE(u2.isInGroup(200));
        REQUIRE(u2.isInGroup(300));
        
        auto groups = u1.getGroups();
        for (auto g : groups) u1.removeFromGroup(g);
        REQUIRE(u1.getGroups().empty());
    }
}