#ifndef LAB3_FILE_MAPPER_H
#define LAB3_FILE_MAPPER_H

#include "Entity/Mapper/ConcretMapper/FSMappers/fs_object_mapper.h"
#include "../../../../File/interface/i_file.h"
#include "../../../../File/interface/i_lockable.h"
#include "../../../../File/realisation/file_descriptor.h"
#include <memory>

/**
 * @brief Конкретный маппер для FileDescriptor.
 *
 * Преобразует объекты в DTO и обратно, включая
 * содержимое файлов, информацию о блокировках и списки контроля доступа.
 */
class FileMapper final : public ConcreteFSObjectMapper<FileDescriptor> {
public:
    /**
     * @brief Получает ключ типа для этого маппера
     * @return Строка "FILE"
     */
    [[nodiscard]] std::string getKey() const override {
        return "FILE";
    }

    /**
     * @brief Получает typeid для типа файла
     * @return typeid FileDescriptor
     */
    [[nodiscard]] std::type_index getType() const override {
        return typeid(FileDescriptor);
    }

    /**
     * @brief Преобразует объект FileDescriptor в DTO
     * @param file Файл для преобразования
     * @return DTO файловой системы
     */
    [[nodiscard]] DTO::FileSystemObjectDTO mapTo(const FileDescriptor& file) const override;

    /**
     * @brief Преобразует DTO в объект FileDescriptor
     * @param dto DTO файловой системы
     * @return Уникальный указатель на созданный объект FileDescriptor
     */
    [[nodiscard]] std::unique_ptr<FileDescriptor> mapFrom(const DTO::FileSystemObjectDTO& dto) const override;
};

#endif