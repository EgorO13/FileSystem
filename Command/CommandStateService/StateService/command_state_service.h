#ifndef LAB3_COMMAND_STATE_SERVICE_H
#define LAB3_COMMAND_STATE_SERVICE_H

#include "Service/StateService/interface/i_state_service.h"
#include "Command/CommandStateService/CommandMapper/composite_command_mapper.h"
#include "Command/CommandRep/interface/i_command_repository.h"
#include <string>

/**
 * @brief Сервис для сохранения и загрузки состояния составных команд
 *
 * Обеспечивает сериализацию и десериализацию составных команд
 * в формате YAML для сохранения между сеансами работы.
 */
class CompositeStateService : public IStateService {
private:
    ICommandRepository& commandRepository_; ///< Ссылка на репозиторий команд
    PolymorphicCommandMapper& mapper_;      ///< Ссылка на маппер команд

public:
    /**
     * @brief Конструктор
     * @param repo Репозиторий команд
     * @param mapper Маппер команд
     */
    explicit CompositeStateService(ICommandRepository& repo, PolymorphicCommandMapper& mapper);

    /**
     * @brief Загружает команды из файла
     * @param path Путь к файлу YAML
     * @throws std::runtime_error Если не удалось загрузить файл
     */
    void load(const std::string& path) override;

    /**
     * @brief Сохраняет команды в файл
     * @param path Путь к файлу YAML
     * @throws std::runtime_error Если не удалось сохранить файл
     */
    void save(const std::string& path) override;
};

#endif