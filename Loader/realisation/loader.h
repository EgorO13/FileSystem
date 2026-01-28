#ifndef LAB3_LOADER_H
#define LAB3_LOADER_H

#include "../interface/i_loader.h"
#include <memory>
#include <vector>

/**
 * @brief Реализация загрузчика зависимостей для файловой системы.
 *
 * Создает и хранит все основные объекты системы, обеспечивая их
 * правильную инициализацию и время жизни.
 */
class FSLoader final : public ILoader {
private:
    std::unique_ptr<PolymorphicFSObjectMapper> fsObjectMapper_;                                     ///< Маппер объектов файловой системы
    std::vector<std::unique_ptr<SubtypeMapper<IFileSystemObject, DTO::FileSystemObjectDTO, std::unique_ptr<IFileSystemObject>, DTO::FileSystemObjectDTO, std::string>>> fsSubtypeMappers_; ///< Вектор подтипов мапперов объектов ФС

    std::unique_ptr<PolymorphicCommandMapper> commandMapper_;                                        ///< Маппер команд
    std::vector<std::unique_ptr<SubtypeMapper<ICommand, DTO::CompositeCommandDTO, std::unique_ptr<ICommand>>>> commandSubtypeMappers_; ///< Вектор подтипов мапперов команд

    std::unique_ptr<IFileSystemRepository> fsRepository_;                                            ///< Репозиторий файловой системы
    std::unique_ptr<IUserRepository> userRepository_;                                                ///< Репозиторий пользователей
    std::unique_ptr<IGroupRepository> groupRepository_;                                              ///< Репозиторий групп
    std::unique_ptr<ICommandRepository> commandRepository_;                                          ///< Репозиторий команд

    std::unique_ptr<IStateService> fsStateService_;                                                  ///< Сервис состояния файловой системы
    std::unique_ptr<IStateService> userStateService_;                                                ///< Сервис состояния пользователей
    std::unique_ptr<IStateService> groupStateService_;                                               ///< Сервис состояния групп
    std::unique_ptr<IStateService> commandStateService_;                                             ///< Сервис состояния команд

    std::unique_ptr<ICommandService> commandService_;                                                ///< Сервис команд
    std::unique_ptr<ISecurityService> securityService_;                                              ///< Сервис безопасности
    std::unique_ptr<IFileSystemService> fsService_;                                                  ///< Сервис файловой системы
    std::unique_ptr<IUserManagementService> userManagementService_;                                  ///< Сервис управления пользователями
    std::unique_ptr<ISessionService> sessionService_;                                                ///< Сервис сессий

    std::unique_ptr<IView> view_;                                                                    ///< Представление

public:
    /**
     * @brief Получить представление (View)
     * @return Ссылка на объект представления
     */
    IView& getView() override;

    /**
     * @brief Получить маппер объектов файловой системы
     * @return Ссылка на полиморфный маппер объектов файловой системы
     */
    PolymorphicFSObjectMapper& getFsObjectMapper() override;

    /**
     * @brief Получить маппер команд
     * @return Ссылка на полиморфный маппер команд
     */
    PolymorphicCommandMapper& getCommandMapper() override;

    /**
     * @brief Получить репозиторий файловой системы
     * @return Ссылка на репозиторий файловой системы
     */
    IFileSystemRepository& getFsRepository() override;

    /**
     * @brief Получить репозиторий пользователей
     * @return Ссылка на репозиторий пользователей
     */
    IUserRepository& getUserRepository() override;

    /**
     * @brief Получить репозиторий групп
     * @return Ссылка на репозиторий групп
     */
    IGroupRepository& getGroupRepository() override;

    /**
     * @brief Получить репозиторий команд
     * @return Ссылка на репозиторий команд
     */
    ICommandRepository& getCommandRepository() override;

    /**
     * @brief Получить сервис команд
     * @return Ссылка на сервис команд
     */
    ICommandService& getCommandService() override;

    /**
     * @brief Получить сервис состояния файловой системы
     * @return Ссылка на сервис состояния файловой системы
     */
    IStateService& getFsStateService() override;

    /**
     * @brief Получить сервис состояния пользователей
     * @return Ссылка на сервис состояния пользователей
     */
    IStateService& getUserStateService() override;

    /**
     * @brief Получить сервис состояния групп
     * @return Ссылка на сервис состояния групп
     */
    IStateService& getGroupStateService() override;

    /**
     * @brief Получить сервис состояния команд
     * @return Ссылка на сервис состояния команд
     */
    IStateService& getCommandStateService() override;

    /**
     * @brief Получить сервис безопасности
     * @return Ссылка на сервис безопасности
     */
    ISecurityService& getSecurityService() override;

    /**
     * @brief Получить сервис файловой системы
     * @return Ссылка на сервис файловой системы
     */
    IFileSystemService& getFsService() override;

    /**
     * @brief Получить сервис управления пользователями
     * @return Ссылка на сервис управления пользователями
     */
    IUserManagementService& getUserManagementService() override;

    /**
     * @brief Получить сервис сессий
     * @return Ссылка на сервис сессий
     */
    ISessionService& getSessionService() override;
};

#endif