#include "fs_state_service.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

FSStateService::FSStateService(IFileSystemRepository& fsRepo, IUserRepository& userRepo, PolymorphicFSObjectMapper& mapper)
    : fsRepo_(fsRepo), userRepo_(userRepo), mapper_(mapper) {}

void FSStateService::save(const std::string& path) {
    std::vector<IFileSystemObject*> allObjects = fsRepo_.getAllObjects();
    std::ofstream out(path);
    YAML::Emitter emitter(out);
    emitter << YAML::BeginMap << YAML::Key << "filesystem" << YAML::Value << YAML::BeginSeq;
    for (IFileSystemObject* obj : allObjects) {
        if (!obj) continue;
        DTO::FileSystemObjectDTO dto = mapper_.mapTo(*obj);
        emitter << YAML::BeginMap
                << YAML::Key << "type" << YAML::Value << dto.type
                << YAML::Key << "address" << YAML::Value << dto.address
                << YAML::Key << "name" << YAML::Value << dto.name
                << YAML::Key << "parentAddress" << YAML::Value << dto.parentAddress
                << YAML::Key << "ownerName" << YAML::Value << dto.ownerName
                << YAML::Key << "ownerId" << YAML::Value << dto.ownerId
                << YAML::Key << "creationTime" << YAML::Value
                << std::chrono::system_clock::to_time_t(dto.creationTime)
                << YAML::Key << "lastModifyTime" << YAML::Value
                << std::chrono::system_clock::to_time_t(dto.lastModifyTime)
                << YAML::Key << "properties" << YAML::Value << YAML::BeginMap;
        for (const auto& [key, value] : dto.properties) {
            emitter << YAML::Key << key << YAML::Value << value;
        }
        emitter << YAML::EndMap << YAML::EndMap;
    }

    emitter << YAML::EndSeq << YAML::EndMap;
}

void FSStateService::load(const std::string& path) {
    YAML::Node config = YAML::LoadFile(path);
    YAML::Node fsNodes = config["filesystem"];
    std::map<unsigned int, std::unique_ptr<IFileSystemObject>> objects;
    std::map<unsigned int, DTO::FileSystemObjectDTO> dtosMap;
    for (const auto& node : fsNodes) {
        DTO::FileSystemObjectDTO dto;
        dto.type = node["type"].as<std::string>();
        dto.address = node["address"].as<unsigned int>();
        dto.name = node["name"].as<std::string>();
        dto.parentAddress = node["parentAddress"].as<unsigned int>();
        dto.ownerName = node["ownerName"].as<std::string>();
        dto.ownerId = node["ownerId"].as<unsigned int>();
        dto.creationTime = std::chrono::system_clock::from_time_t(node["creationTime"].as<time_t>());
        dto.lastModifyTime = std::chrono::system_clock::from_time_t(node["lastModifyTime"].as<time_t>());

        YAML::Node propertiesNode = node["properties"];
        for (const auto& property : propertiesNode) {
            std::string key = property.first.as<std::string>();
            std::string value = property.second.as<std::string>();
            dto.properties[key] = value;
        }

        std::unique_ptr<IFileSystemObject> obj = mapper_.mapFrom(dto);
        if (obj) {
            objects[dto.address] = std::move(obj);
            dtosMap[dto.address] = std::move(dto);
        }
    }

    for (const auto& [address, dto] : dtosMap) {
        if (dto.type == "DIR") {
            auto it = objects.find(address);
            if (it != objects.end()) {
                IDirectory* dir = dynamic_cast<IDirectory*>(it->second.get());
                if (dir && dto.properties.count("children")) {
                    std::string childrenStr = dto.properties.at("children");
                    std::istringstream iss(childrenStr);
                    std::string token;

                    while (std::getline(iss, token, ',')) {
                        if (!token.empty()) {
                            try {
                                unsigned int childAddr = std::stoul(token);
                                auto childIt = objects.find(childAddr);
                                if (childIt != objects.end()) {
                                    dir->addChild(childIt->second.get());
                                }
                            } catch (...) {}
                        }
                    }
                }
            }
        }
    }

    for (auto& [address, obj] : objects) {
        auto dtoIt = dtosMap.find(address);
        if (dtoIt != dtosMap.end()) {
            const auto& dto = dtoIt->second;
            User* user = userRepo_.getUserByName(dto.ownerName);
            if (user) {
                obj->setOwner(*user);
            }
        }
    }
    fsRepo_.clear();
    auto rootIt = objects.find(0);
    if (rootIt != objects.end()) {
        IDirectory* rootDir = dynamic_cast<IDirectory*>(rootIt->second.get());
        if (rootDir) fsRepo_.setRootDirectory(rootDir);
    }

    for (auto& [address, obj] : objects) {
        fsRepo_.saveObject(std::move(obj));
    }
}