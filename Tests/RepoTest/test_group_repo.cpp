#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <algorithm>
#include "Repository/GroupRep/realisation/group_repository.h"
#include "Entity/User/user.h"
#include "Entity/Group/group.h"

TEST_CASE("GroupRepository") {
    GroupRepository repo;
    User user1(1, "user1");
    User user2(2, "user2");
    User user3(3, "user3");

    SECTION("getGroupById") {
        auto group = std::make_unique<Group>(repo.getNextId(), "test");
        unsigned int groupId = group->getId();
        REQUIRE(repo.saveGroup(std::move(group)));
        auto* found = repo.getGroupById(groupId);
        REQUIRE(found != nullptr);
        REQUIRE(found->getName() == "test");
        REQUIRE(repo.getGroupById(9999) == nullptr);
        auto* allGroup = repo.getGroupById(0);
        REQUIRE(allGroup != nullptr);
        REQUIRE(allGroup->getName() == "All");
    }

    SECTION("getGroupByName") {
        auto group = std::make_unique<Group>(repo.getNextId(), "test");
        unsigned int groupId = group->getId();
        REQUIRE(repo.saveGroup(std::move(group)));
        auto* found = repo.getGroupByName("test");
        REQUIRE(found != nullptr);
        REQUIRE(found->getId() == groupId);
        REQUIRE(repo.getGroupByName("nonexistent") == nullptr);
        REQUIRE(repo.getGroupByName("") == nullptr);
    }

    SECTION("getAllGroups") {
        auto initialGroups = repo.getAllGroups();
        REQUIRE(initialGroups.size() >= 1);

        auto group1 = std::make_unique<Group>(repo.getNextId(), "group1");
        auto group2 = std::make_unique<Group>(repo.getNextId(), "group2");
        REQUIRE(repo.saveGroup(std::move(group1)));
        REQUIRE(repo.saveGroup(std::move(group2)));

        auto allGroups = repo.getAllGroups();
        REQUIRE(allGroups.size() >= 3);
    }

    SECTION("saveGroup") {
        auto group1 = std::make_unique<Group>(repo.getNextId(), "group1");
        REQUIRE(repo.saveGroup(std::move(group1)));
        REQUIRE_FALSE(repo.saveGroup(nullptr));
        auto group2 = std::make_unique<Group>(1, "group2");
        REQUIRE_FALSE(repo.saveGroup(std::move(group2)));
        auto group3 = std::make_unique<Group>(repo.getNextId(), "group1");
        REQUIRE_FALSE(repo.saveGroup(std::move(group3)));
        auto group4 = std::make_unique<Group>(100, "group4");
        REQUIRE(repo.saveGroup(std::move(group4)));
    }

    SECTION("deleteGroup") {
        REQUIRE_FALSE(repo.deleteGroup(9999));

        REQUIRE_FALSE(repo.deleteGroup(0));

        auto parentGroup = std::make_unique<Group>(repo.getNextId(), "parent");
        auto childGroup = std::make_unique<Group>(repo.getNextId(), "child");
        auto grandchildGroup = std::make_unique<Group>(repo.getNextId(), "grandchild");

        unsigned int parentId = parentGroup->getId();
        unsigned int childId = childGroup->getId();
        unsigned int grandchildId = grandchildGroup->getId();

        REQUIRE(repo.saveGroup(std::move(parentGroup)));
        REQUIRE(repo.saveGroup(std::move(childGroup)));
        REQUIRE(repo.saveGroup(std::move(grandchildGroup)));

        REQUIRE(repo.addSubgroup(parentId, childId));
        REQUIRE(repo.addSubgroup(childId, grandchildId));

        REQUIRE(repo.addUserToGroup(user1.getId(), parentId));
        REQUIRE(repo.addUserToGroup(user2.getId(), childId));
        REQUIRE(repo.addUserToGroup(user3.getId(), grandchildId));

        auto otherGroup = std::make_unique<Group>(repo.getNextId(), "other");
        unsigned int otherId = otherGroup->getId();
        REQUIRE(repo.saveGroup(std::move(otherGroup)));
        REQUIRE(repo.addSubgroup(otherId, parentId));

        REQUIRE(repo.groupExists(parentId));
        REQUIRE(repo.groupExists(childId));
        REQUIRE(repo.groupExists(grandchildId));
        REQUIRE(repo.isUserInGroupRecursive(user1.getId(), parentId));
        REQUIRE(repo.isSubgroupRecursive(parentId, childId));
        REQUIRE(repo.isSubgroupRecursive(parentId, grandchildId));

        REQUIRE(repo.deleteGroup(parentId));
        REQUIRE_FALSE(repo.groupExists(parentId));

        REQUIRE(repo.groupExists(childId));
        REQUIRE(repo.groupExists(grandchildId));

        REQUIRE_FALSE(repo.isUserInGroupRecursive(user1.getId(), parentId));
        REQUIRE_FALSE(repo.isSubgroupRecursive(parentId, childId));

        auto subgroups = repo.getDirectSubgroups(otherId);
        REQUIRE(std::find(subgroups.begin(), subgroups.end(), parentId) == subgroups.end());

        auto singleGroup = std::make_unique<Group>(repo.getNextId(), "single");
        unsigned int singleId = singleGroup->getId();
        REQUIRE(repo.saveGroup(std::move(singleGroup)));
        REQUIRE(repo.deleteGroup(singleId));
        REQUIRE_FALSE(repo.groupExists(singleId));
    }

    SECTION("removeUserFromGroup") {
        auto group = std::make_unique<Group>(repo.getNextId(), "test");
        unsigned int groupId = group->getId();
        REQUIRE(repo.saveGroup(std::move(group)));
        REQUIRE_FALSE(repo.removeUserFromGroup(user1.getId(), 9999));
        REQUIRE_FALSE(repo.removeUserFromGroup(user1.getId(), groupId));
        REQUIRE(repo.addUserToGroup(user1.getId(), groupId));
        REQUIRE(repo.removeUserFromGroup(user1.getId(), groupId));
        REQUIRE_FALSE(repo.isUserInGroupRecursive(user1.getId(), groupId));
        REQUIRE_FALSE(repo.removeUserFromGroup(user1.getId(), groupId));

        REQUIRE(repo.addUserToGroup(user1.getId(), groupId));
        REQUIRE(repo.addUserToGroup(user2.getId(), groupId));
        REQUIRE(repo.addUserToGroup(user3.getId(), groupId));

        REQUIRE(repo.removeUserFromGroup(user2.getId(), groupId));
        REQUIRE(repo.isUserInGroupRecursive(user1.getId(), groupId));
        REQUIRE_FALSE(repo.isUserInGroupRecursive(user2.getId(), groupId));
        REQUIRE(repo.isUserInGroupRecursive(user3.getId(), groupId));
    }

    SECTION("clear") {
        REQUIRE(repo.groupExists(0));
        REQUIRE(repo.getGroupByName("All") != nullptr);
        auto group1 = std::make_unique<Group>(repo.getNextId(), "group1");
        auto group2 = std::make_unique<Group>(repo.getNextId(), "group2");
        unsigned int id1 = group1->getId();
        unsigned int id2 = group2->getId();

        REQUIRE(repo.saveGroup(std::move(group1)));
        REQUIRE(repo.saveGroup(std::move(group2)));
        REQUIRE(repo.addUserToGroup(user1.getId(), id1));
        REQUIRE(repo.addSubgroup(id1, id2));
        REQUIRE(repo.groupExists(id1));
        REQUIRE(repo.groupExists(id2));
        REQUIRE(repo.isUserInGroupRecursive(user1.getId(), id1));
        REQUIRE(repo.isSubgroupRecursive(id1, id2));

        repo.clear();

        REQUIRE(repo.groupExists(0));
        REQUIRE_FALSE(repo.groupExists(id1));
        REQUIRE_FALSE(repo.groupExists(id2));
        REQUIRE_FALSE(repo.isUserInGroupRecursive(user1.getId(), id1));
        REQUIRE(repo.getAllGroups().size() == 2);
        REQUIRE(repo.getNextId() == 2);
    }

    SECTION("setNextId") {
        REQUIRE(repo.setNextId(100));
        REQUIRE(repo.getNextId() == 100);
        REQUIRE_FALSE(repo.setNextId(50));
        unsigned int currentId = repo.getNextId();
        REQUIRE_FALSE(repo.setNextId(currentId));
        REQUIRE_FALSE(repo.setNextId(0));
        REQUIRE(repo.getNextId() != 0);

        auto group = std::make_unique<Group>(repo.getNextId(), "test");
        REQUIRE(repo.saveGroup(std::move(group)));
    }

    SECTION("groupExists (по ID и имени)") {
        auto group = std::make_unique<Group>(repo.getNextId(), "test");
        unsigned int groupId = group->getId();
        REQUIRE(repo.saveGroup(std::move(group)));
        REQUIRE(repo.groupExists(groupId));
        REQUIRE_FALSE(repo.groupExists(9999));
        REQUIRE(repo.groupExists("test"));
        REQUIRE_FALSE(repo.groupExists("nonexistent"));
        REQUIRE_FALSE(repo.groupExists(""));
    }

    SECTION("addUserToGroup и getAllGroupsOfUser") {
        auto group1 = std::make_unique<Group>(repo.getNextId(), "group1");
        auto group2 = std::make_unique<Group>(repo.getNextId(), "group2");
        unsigned int id1 = group1->getId();
        unsigned int id2 = group2->getId();

        REQUIRE(repo.saveGroup(std::move(group1)));
        REQUIRE(repo.saveGroup(std::move(group2)));
        REQUIRE_FALSE(repo.addUserToGroup(user1.getId(), 9999));
        REQUIRE(repo.addUserToGroup(user1.getId(), id1));
        REQUIRE(repo.addUserToGroup(user1.getId(), id2));
        REQUIRE_FALSE(repo.addUserToGroup(user1.getId(), id1));

        auto userGroups = repo.getAllGroupsOfUser(user1.getId());
        REQUIRE(userGroups.size() == 2);
        REQUIRE(std::find(userGroups.begin(), userGroups.end(), id1) != userGroups.end());
        REQUIRE(std::find(userGroups.begin(), userGroups.end(), id2) != userGroups.end());
        REQUIRE(repo.getAllGroupsOfUser(9999).empty());
    }

    SECTION("addSubgroup и removeSubgroup") {
        auto parent = std::make_unique<Group>(repo.getNextId(), "parent");
        auto child = std::make_unique<Group>(repo.getNextId(), "child");
        unsigned int parentId = parent->getId();
        unsigned int childId = child->getId();
        REQUIRE(repo.saveGroup(std::move(parent)));
        REQUIRE(repo.saveGroup(std::move(child)));
        REQUIRE_FALSE(repo.addSubgroup(9999, childId));
        REQUIRE_FALSE(repo.addSubgroup(parentId, 9999));
        REQUIRE_FALSE(repo.addSubgroup(parentId, parentId));
        REQUIRE(repo.addSubgroup(parentId, childId));
        REQUIRE_FALSE(repo.addSubgroup(parentId, childId));

        auto grandchild = std::make_unique<Group>(repo.getNextId(), "grandchild");
        unsigned int grandchildId = grandchild->getId();
        REQUIRE(repo.saveGroup(std::move(grandchild)));
        REQUIRE(repo.addSubgroup(childId, grandchildId));
        REQUIRE(repo.wouldCreateCycle(grandchildId, parentId));
        REQUIRE_FALSE(repo.addSubgroup(grandchildId, parentId));

        REQUIRE(repo.removeSubgroup(parentId, childId));
        REQUIRE_FALSE(repo.isSubgroupRecursive(parentId, childId));

        REQUIRE_FALSE(repo.removeSubgroup(parentId, childId));
        REQUIRE_FALSE(repo.removeSubgroup(9999, childId));
        REQUIRE_FALSE(repo.removeSubgroup(parentId, 9999));
    }

    SECTION("getDirectSubgroups и getDirectParentGroups") {
        auto parent = std::make_unique<Group>(repo.getNextId(), "parent");
        auto child1 = std::make_unique<Group>(repo.getNextId(), "child1");
        auto child2 = std::make_unique<Group>(repo.getNextId(), "child2");
        unsigned int parentId = parent->getId();
        unsigned int child1Id = child1->getId();
        unsigned int child2Id = child2->getId();

        REQUIRE(repo.saveGroup(std::move(parent)));
        REQUIRE(repo.saveGroup(std::move(child1)));
        REQUIRE(repo.saveGroup(std::move(child2)));

        REQUIRE(repo.addSubgroup(parentId, child1Id));
        REQUIRE(repo.addSubgroup(parentId, child2Id));

        auto subgroups = repo.getDirectSubgroups(parentId);
        REQUIRE(subgroups.size() == 2);

        auto parents = repo.getDirectParentGroups(child1Id);
        REQUIRE(parents.size() == 1);
        REQUIRE(parents[0] == parentId);

        REQUIRE(repo.getDirectSubgroups(9999).empty());
        REQUIRE(repo.getDirectParentGroups(9999).empty());
    }

    SECTION("getAllParentGroups и getAllSubgroups") {
        auto a = std::make_unique<Group>(repo.getNextId(), "A");
        auto b = std::make_unique<Group>(repo.getNextId(), "B");
        auto c = std::make_unique<Group>(repo.getNextId(), "C");
        unsigned int idA = a->getId();
        unsigned int idB = b->getId();
        unsigned int idC = c->getId();

        REQUIRE(repo.saveGroup(std::move(a)));
        REQUIRE(repo.saveGroup(std::move(b)));
        REQUIRE(repo.saveGroup(std::move(c)));

        REQUIRE(repo.addSubgroup(idA, idB));
        REQUIRE(repo.addSubgroup(idB, idC));

        auto parentsOfC = repo.getAllParentGroups(idC);
        REQUIRE(parentsOfC.size() == 2);

        auto subsOfA = repo.getAllSubgroups(idA);
        REQUIRE(subsOfA.size() == 2);

        REQUIRE(repo.getAllParentGroups(9999).empty());
        REQUIRE(repo.getAllSubgroups(9999).empty());
    }

    SECTION("isUserInGroupRecursive и isSubgroupRecursive") {
        auto parent = std::make_unique<Group>(repo.getNextId(), "parent");
        auto child = std::make_unique<Group>(repo.getNextId(), "child");
        unsigned int parentId = parent->getId();
        unsigned int childId = child->getId();

        REQUIRE(repo.saveGroup(std::move(parent)));
        REQUIRE(repo.saveGroup(std::move(child)));
        REQUIRE(repo.addSubgroup(parentId, childId));
        REQUIRE(repo.addUserToGroup(user1.getId(), childId));

        REQUIRE(repo.isUserInGroupRecursive(user1.getId(), childId));
        REQUIRE(repo.isUserInGroupRecursive(user1.getId(), parentId));
        REQUIRE_FALSE(repo.isUserInGroupRecursive(user2.getId(), parentId));

        REQUIRE(repo.isSubgroupRecursive(parentId, childId));
        REQUIRE_FALSE(repo.isSubgroupRecursive(childId, parentId));
        REQUIRE_FALSE(repo.isSubgroupRecursive(parentId, parentId));

        REQUIRE_FALSE(repo.isUserInGroupRecursive(user1.getId(), 9999));
        REQUIRE_FALSE(repo.isSubgroupRecursive(9999, childId));
        REQUIRE_FALSE(repo.isSubgroupRecursive(parentId, 9999));
    }

    SECTION("wouldCreateCycle и validateNoCycle") {
        auto a = std::make_unique<Group>(repo.getNextId(), "A");
        auto b = std::make_unique<Group>(repo.getNextId(), "B");
        auto c = std::make_unique<Group>(repo.getNextId(), "C");
        unsigned int idA = a->getId();
        unsigned int idB = b->getId();
        unsigned int idC = c->getId();

        REQUIRE(repo.saveGroup(std::move(a)));
        REQUIRE(repo.saveGroup(std::move(b)));
        REQUIRE(repo.saveGroup(std::move(c)));

        REQUIRE(repo.addSubgroup(idA, idB));

        REQUIRE_FALSE(repo.wouldCreateCycle(idA, idB));
        REQUIRE(repo.wouldCreateCycle(idB, idA));
        REQUIRE_FALSE(repo.addSubgroup(idB, idA));
        REQUIRE_FALSE(repo.wouldCreateCycle(idB, idC));
        REQUIRE(repo.addSubgroup(idB, idC));
        REQUIRE(repo.wouldCreateCycle(idC, idA));
        REQUIRE_FALSE(repo.addSubgroup(idC, idA));
        REQUIRE(repo.wouldCreateCycle(idA, idA));
        REQUIRE_FALSE(repo.addSubgroup(idA, idA));
    }
}