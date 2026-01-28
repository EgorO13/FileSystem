#ifndef LAB3_GROUP_STATE_SERVICE_H
#define LAB3_GROUP_STATE_SERVICE_H

#include "Service/StateService/interface/i_state_service.h"
#include "Repository/GroupRep/interface/i_group_repository.h"
#include <string>

/**
 * @brief Сервис для сохранения и загрузки состояния групп.
 *
 * Реализует интерфейс IStateService для работы с группами пользователей.
 * Сохраняет и загружает данные о группах, их членстве и иерархии в YAML-формате.
 */
class GroupStateService final : public IStateService {
private:
    IGroupRepository& groupRepo_;  ///< Репозиторий групп

public:
    /**
     * @brief Конструктор сервиса состояния групп
     * @param groupRepo Репозиторий групп
     */
    explicit GroupStateService(IGroupRepository& groupRepo);

    /**
     * @brief Загрузить состояние групп из файла
     * @param path Путь к файлу с данными
     */
    void load(const std::string& path) override;

    /**
     * @brief Сохранить состояние групп в файл
     * @param path Путь к файлу для сохранения
     */
    void save(const std::string& path) override;
};

#endif