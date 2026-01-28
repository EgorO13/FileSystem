#include "directory_mapper.h"
#include "Entity/Mapper/ConcretMapper/FSMappers/ACLSerializer/acl_serializer.h"

DTO::FileSystemObjectDTO DirectoryMapper::mapTo(const DirectoryDescriptor& dir) const {
    DTO::FileSystemObjectDTO dto;
    dto.type = "DIR";
    dto.address = dir.getAddress();
    dto.name = dir.getName();
    dto.parentAddress = dir.getParentDirectoryAddress();
    dto.ownerName = dir.getOwner().getName();
    dto.ownerId = dir.getOwner().getId();
    dto.creationTime = dir.getCreateTime();
    dto.lastModifyTime = dir.getLastModifyTime();
    auto children = dir.listChild();
    std::string childrenStr;
    for (const auto& child : children) {
        if (!childrenStr.empty()) childrenStr += ",";
        childrenStr += std::to_string(child->getAddress());
    }
    dto.properties["children"] = childrenStr;
    std::vector<ACLEntry> aclEntries = dir.getACL();
    if (!aclEntries.empty()) dto.properties["acl"] = ACLSerializer::serialize(aclEntries);
    return dto;
}

std::unique_ptr<DirectoryDescriptor> DirectoryMapper::mapFrom(const DTO::FileSystemObjectDTO& dto) const {
    User tempOwner(dto.ownerId, dto.ownerName);
    auto dir = std::make_unique<DirectoryDescriptor>(
        dto.name,
        dto.parentAddress,
        tempOwner,
        dto.address
    );
    if (dto.properties.count("acl")) {
        std::vector<ACLEntry> aclEntries = ACLSerializer::deserialize(dto.properties.at("acl"));
        dir->setACL(aclEntries);
    }
    dir->setCreateTime(dto.creationTime);
    dir->setLastModifyTime(dto.lastModifyTime);
    return dir;
}