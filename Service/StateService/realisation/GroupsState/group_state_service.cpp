#include "group_state_service.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>

GroupStateService::GroupStateService(IGroupRepository& groupRepo)
    : groupRepo_(groupRepo) {}

void GroupStateService::save(const std::string& path) {
    std::vector<Group*> allGroups = groupRepo_.getAllGroups();
    std::ofstream out(path);
    YAML::Emitter emitter(out);

    emitter << YAML::BeginMap << YAML::Key << "groups" << YAML::Value << YAML::BeginSeq;

    for (Group* group : allGroups) {
        if (!group) continue;

        unsigned int groupId = group->getId();
        emitter << YAML::BeginMap
                << YAML::Key << "id" << YAML::Value << groupId
                << YAML::Key << "name" << YAML::Value << group->getName();

        std::vector<unsigned int> userIds = group->getUsers();
        if (!userIds.empty()) {
            emitter << YAML::Key << "users" << YAML::Value << YAML::BeginSeq;
            for (unsigned int userId : userIds) {
                emitter << userId;
            }
            emitter << YAML::EndSeq;
        }

        std::vector<unsigned int> subgroupIds = groupRepo_.getDirectSubgroups(groupId);
        if (!subgroupIds.empty()) {
            emitter << YAML::Key << "subgroups" << YAML::Value << YAML::BeginSeq;
            for (unsigned int subgroupId : subgroupIds) {
                emitter << subgroupId;
            }
            emitter << YAML::EndSeq;
        }

        emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq << YAML::EndMap;
}

void GroupStateService::load(const std::string& path) {
    YAML::Node config = YAML::LoadFile(path);
    YAML::Node groupNodes = config["groups"];
    groupRepo_.clear();
    struct GroupData {
        unsigned int id;
        std::string name;
        std::vector<unsigned int> userIds;
        std::vector<unsigned int> subgroupIds;
    };
    std::vector<GroupData> groupsData;
    for (const auto& node : groupNodes) {
        GroupData data;
        data.id = node["id"].as<unsigned int>();
        data.name = node["name"].as<std::string>();
        if (node["users"]) {
            YAML::Node users = node["users"];
            for (const auto& userNode : users) {
                unsigned int userId = userNode.as<unsigned int>();
                data.userIds.push_back(userId);
            }
        }
        if (node["subgroups"]) {
            YAML::Node subgroups = node["subgroups"];
            for (const auto& subgroupNode : subgroups) {
                unsigned int subgroupId = subgroupNode.as<unsigned int>();
                data.subgroupIds.push_back(subgroupId);
            }
        }
        groupsData.push_back(std::move(data));
    }
    for (const auto& data : groupsData) {
        auto group = std::make_unique<Group>(data.id, data.name);
        groupRepo_.saveGroup(std::move(group));
    }
    for (const auto& data : groupsData) {
        for (unsigned int userId : data.userIds) {
            groupRepo_.addUserToGroup(userId, data.id);
        }
    }
    for (const auto& data : groupsData) {
        for (unsigned int subgroupId : data.subgroupIds) {
            groupRepo_.addSubgroup(data.id, subgroupId);
        }
    }
    unsigned int maxId = 0;
    for (const auto& data : groupsData) {
        if (data.id > maxId) {
            maxId = data.id;
        }
    }
    groupRepo_.setNextId(maxId + 1);
}