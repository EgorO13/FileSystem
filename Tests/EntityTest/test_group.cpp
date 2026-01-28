#include <catch2/catch_all.hpp>
#include "Entity/Group/group.h"

TEST_CASE("Group") {
    SECTION("Конструкторы") {
        Group g1;
        REQUIRE(g1.getId() == 0);
        REQUIRE(g1.getName() == "");
        
        Group g2(1, "admin");
        REQUIRE(g2.getId() == 1);
        REQUIRE(g2.getName() == "admin");
        
        Group g3 = g2;
        REQUIRE(g3.getId() == 1);
        REQUIRE(g3.getName() == "admin");
        
        Group g4;
        g4 = g2;
        REQUIRE(g4.getId() == 1);
        REQUIRE(g4.getName() == "admin");
    }
    
    SECTION("Методы доступа") {
        Group g(1, "test");
        
        REQUIRE(g.getId() == 1);
        REQUIRE(g.getName() == "test");
        
        g.setId(2);
        g.setName("new_name");
        REQUIRE(g.getId() == 2);
        REQUIRE(g.getName() == "new_name");
    }
    
    SECTION("Управление пользователями") {
        Group g(1, "group");
        
        REQUIRE(g.getUsers().empty());
        
        g.addUser(100);
        REQUIRE(g.containsUser(100));
        REQUIRE(g.getUsers().size() == 1);
        REQUIRE(g.getUsers()[0] == 100);
        
        g.addUser(100);
        REQUIRE(g.getUsers().size() == 1);
        
        g.addUser(200);
        g.addUser(300);
        REQUIRE(g.getUsers().size() == 3);
        
        g.removeUser(200);
        REQUIRE_FALSE(g.containsUser(200));
        REQUIRE(g.getUsers().size() == 2);
        
        g.removeUser(999);
        REQUIRE(g.getUsers().size() == 2);
    }
    
    SECTION("Управление подгруппами") {
        Group g(1, "parent");
        
        REQUIRE(g.getSubgroups().empty());
        
        REQUIRE(g.addSubgroup(10));
        REQUIRE(g.containsSubgroup(10));
        REQUIRE(g.getSubgroups().size() == 1);
        REQUIRE_FALSE(g.addSubgroup(1));
        REQUIRE_FALSE(g.addSubgroup(10));
        REQUIRE(g.addSubgroup(20));
        REQUIRE(g.addSubgroup(30));
        REQUIRE(g.getSubgroups().size() == 3);
        REQUIRE(g.removeSubgroup(20));
        REQUIRE_FALSE(g.containsSubgroup(20));
        REQUIRE(g.getSubgroups().size() == 2);
        REQUIRE_FALSE(g.removeSubgroup(999));
    }
    
    SECTION("Комбинированные операции") {
        Group parent(1, "parent");
        Group child(2, "child");
        
        parent.addUser(100);
        parent.addUser(200);
        parent.addSubgroup(2);
        
        child.addUser(300);
        child.addUser(400);
        
        REQUIRE(parent.containsUser(100));
        REQUIRE(parent.containsSubgroup(2));
        REQUIRE(child.containsUser(300));
        
        REQUIRE(parent.getUsers().size() == 2);
        REQUIRE(parent.getSubgroups().size() == 1);
        REQUIRE(child.getUsers().size() == 2);
    }
}