#include "file_mapper.h"
#include "Entity/Mapper/ConcretMapper/FSMappers/ACLSerializer/acl_serializer.h"

DTO::FileSystemObjectDTO FileMapper::mapTo(const FileDescriptor& file) const {
    DTO::FileSystemObjectDTO dto;
    dto.type = "FILE";
    dto.address = file.getAddress();
    dto.name = file.getName();
    dto.parentAddress = file.getParentDirectoryAddress();
    dto.ownerName = file.getOwner().getName();
    dto.ownerId = file.getOwner().getId();
    dto.creationTime = file.getCreateTime();
    dto.lastModifyTime = file.getLastModifyTime();
    dto.properties["content"] = file.readContent();
    dto.properties["size"] = std::to_string(file.getSize());
    Lock mode = file.isReadable() && file.isWritable() ? Lock::NotLock :
               file.isReadable() ? Lock::WriteLock :
               file.isWritable() ? Lock::ReadLock : Lock::AllLock;
    dto.properties["mode"] = std::to_string(static_cast<int>(mode));
    std::vector<ACLEntry> aclEntries = file.getACL();
    if (!aclEntries.empty()) dto.properties["acl"] = ACLSerializer::serialize(aclEntries);
    return dto;
}

std::unique_ptr<FileDescriptor> FileMapper::mapFrom(const DTO::FileSystemObjectDTO& dto) const {
    User tempOwner(dto.ownerId, dto.ownerName);
    auto file = std::make_unique<FileDescriptor>(
        dto.name,
        dto.parentAddress,
        tempOwner,
        dto.address
    );
    if (dto.properties.count("content")) file->writeContent(dto.properties.at("content"));
    if (dto.properties.count("mode")) file->setMode(static_cast<Lock>(std::stoi(dto.properties.at("mode"))));
    if (dto.properties.count("acl")) {
        std::vector<ACLEntry> aclEntries = ACLSerializer::deserialize(dto.properties.at("acl"));
        file->setACL(aclEntries);
    }
    file->setCreateTime(dto.creationTime);
    file->setLastModifyTime(dto.lastModifyTime);
    return file;
}