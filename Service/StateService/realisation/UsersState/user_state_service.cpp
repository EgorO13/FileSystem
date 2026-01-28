#include "user_state_service.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <vector>
#include <memory>

UserStateService::UserStateService(IUserRepository& userRepo)
    : userRepo_(userRepo) {}

void UserStateService::save(const std::string& path) {
    std::vector<User*> allUsers = userRepo_.getAllUsers();
    std::ofstream out(path);
    YAML::Emitter emitter(out);

    emitter << YAML::BeginMap << YAML::Key << "users" << YAML::Value << YAML::BeginSeq;

    for (User* user : allUsers) {
        if (!user) continue;

        emitter << YAML::BeginMap
                << YAML::Key << "id" << YAML::Value << user->getId()
                << YAML::Key << "name" << YAML::Value << user->getName();

        std::vector<unsigned int> groupIds = user->getGroups();
        if (!groupIds.empty()) {
            emitter << YAML::Key << "groups" << YAML::Value << YAML::BeginSeq;
            for (unsigned int groupId : groupIds) {
                emitter << groupId;
            }
            emitter << YAML::EndSeq;
        }

        emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq << YAML::EndMap;
}

void UserStateService::load(const std::string& path) {
    YAML::Node config = YAML::LoadFile(path);
    YAML::Node userNodes = config["users"];

    std::vector<std::unique_ptr<User>> users;

    for (const auto& node : userNodes) {
        unsigned int id = node["id"].as<unsigned int>();
        std::string name = node["name"].as<std::string>();

        auto user = std::make_unique<User>(id, name);

        if (node["groups"]) {
            YAML::Node groups = node["groups"];
            for (const auto& groupNode : groups) {
                unsigned int groupId = groupNode.as<unsigned int>();
                user->addToGroup(groupId);
            }
        }
        users.push_back(std::move(user));
    }

    userRepo_.clear();
    for (auto& user : users) {
        userRepo_.saveUser(std::move(user));
    }
}

