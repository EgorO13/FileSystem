#ifndef LAB3_FS_STATE_SERVICE_H
#define LAB3_FS_STATE_SERVICE_H

#include "Service/StateService/interface/i_state_service.h"
#include "Repository/FSRep/interface/i_fs_repository.h"
#include "Repository/UserRep/interface/i_user_repository.h"
#include "Entity/Mapper/ConcretMapper/FSMappers/fs_object_mapper.h"
#include <string>

/**
 * @brief Сервис для сохранения и загрузки состояния файловой системы.
 *
 * Реализует интерфейс IStateService для работы с объектами файловой системы.
 * Сохраняет и загружает данные о файлах и директориях с использованием маппера.
 */
class FSStateService final : public IStateService {
private:
    IFileSystemRepository& fsRepo_;      ///< Репозиторий файловой системы
    IUserRepository& userRepo_;          ///< Репозиторий пользователей
    PolymorphicFSObjectMapper& mapper_;  ///< Маппер объектов файловой системы

public:
    /**
     * @brief Конструктор сервиса состояния файловой системы
     * @param fsRepo Репозиторий файловой системы
     * @param userRepo Репозиторий пользователей
     * @param mapper Маппер объектов файловой системы
     */
    explicit FSStateService(IFileSystemRepository& fsRepo, IUserRepository& userRepo, PolymorphicFSObjectMapper& mapper);

    /**
     * @brief Загрузить состояние файловой системы из файла
     * @param path Путь к файлу с данными
     */
    void load(const std::string& path) override;

    /**
     * @brief Сохранить состояние файловой системы в файл
     * @param path Путь к файлу для сохранения
     */
    void save(const std::string& path) override;
};

#endif