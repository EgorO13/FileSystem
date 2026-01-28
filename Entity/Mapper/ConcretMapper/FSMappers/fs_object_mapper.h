#ifndef LAB3_FILESYSTEMOBJECT_MAPPERS_H
#define LAB3_FILESYSTEMOBJECT_MAPPERS_H

#include <memory>
#include "Entity/Mapper/polymorphic_mapper.h"
#include "Entity/Mapper/dto.h"
#include "Entity/FSObject/interface/i_fs_object.h"

using FSObjectMapper = Mapper<IFileSystemObject, DTO::FileSystemObjectDTO, std::unique_ptr<IFileSystemObject>>;

template<typename Subtype>
using ConcreteFSObjectMapper = SubtypeMapper<Subtype, DTO::FileSystemObjectDTO, std::unique_ptr<Subtype>>;

template<typename ConcreteMapper>
using SuperFSObjectMapper = SubtypeMapperAdapter<IFileSystemObject, std::unique_ptr<IFileSystemObject>, ConcreteMapper>;

class PolymorphicFSObjectMapper final : public PolymorphicMapper<IFileSystemObject, DTO::FileSystemObjectDTO, std::unique_ptr<IFileSystemObject>> {
public:
    [[nodiscard]] std::string getKey(const DTO::FileSystemObjectDTO &dto) const override {
        return dto.type;
    }
};

#endif
