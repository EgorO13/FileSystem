#ifndef LAB3_DIRECTORY_MAPPER_H
#define LAB3_DIRECTORY_MAPPER_H

#include "Entity/Mapper/ConcretMapper/FSMappers/fs_object_mapper.h"
#include "../../../../Directory/realisation/directory_descriptor.h"
#include "../../../../Directory/interface/i_directory.h"
#include <memory>

/**
 * @brief Конкретный маппер для объектов DirectoryDescriptor.
 *
 * Преобразует объекты директорий в DTO и обратно, включая
 * информацию о детях директории и списках контроля доступа.
 */
class DirectoryMapper final : public ConcreteFSObjectMapper<DirectoryDescriptor> {
public:
    /**
     * @brief Получает ключ типа для этого маппера
     * @return Строка "DIR"
     */
    [[nodiscard]] std::string getKey() const override {
        return "DIR";
    }

    /**
     * @brief Получает typeid для типа директории
     * @return typeid DirectoryDescriptor
     */
    [[nodiscard]] std::type_index getType() const override {
        return typeid(DirectoryDescriptor);
    }

    /**
     * @brief Преобразует объект DirectoryDescriptor в DTO
     * @param dir Директория для преобразования
     * @return DTO файловой системы
     */
    [[nodiscard]] DTO::FileSystemObjectDTO mapTo(const DirectoryDescriptor& dir) const override;

    /**
     * @brief Преобразует DTO в объект DirectoryDescriptor
     * @param dto DTO файловой системы
     * @return Уникальный указатель на созданный объект DirectoryDescriptor
     */
    [[nodiscard]] std::unique_ptr<DirectoryDescriptor> mapFrom(const DTO::FileSystemObjectDTO& dto) const override;
};

#endif