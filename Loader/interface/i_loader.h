#ifndef LAB3_I_LOADER_H
#define LAB3_I_LOADER_H

#include "Entity/Mapper/dto.h"
#include "Entity/Mapper/ConcretMapper/FSMappers/fs_object_mapper.h"
#include "Entity/Mapper/ConcretMapper/FSMappers/FileMapper/file_mapper.h"
#include "Entity/Mapper/ConcretMapper/FSMappers/DirectoryMapper/directory_mapper.h"
#include "Command/CommandStateService/CommandMapper/command_mapper.h"
#include "../../Command/CommandRep/realisation/command_repository.h"
#include "Command/CommandStateService/CommandMapper/composite_command_mapper.h"
#include "../../Repository/FSRep/realisation/fs_repository.h"
#include "../../Repository/UserRep/realisation/user_repository.h"
#include "../../Repository/GroupRep/realisation/group_repository.h"
#include "../../Command/CommandRep/interface/i_command_repository.h"
#include "../../Command/CommandService/interface/i_command_service.h"
#include "Service/StateService/realisation/FSObjectsState/fs_state_service.h"
#include "Service/StateService/realisation/UsersState/user_state_service.h"
#include "Service/StateService/realisation/GroupsState/group_state_service.h"
#include "Command/CommandStateService/StateService/command_state_service.h"
#include "../../View/interface/i_view.h"
#include "../../Service/SecurityService/realisation/security_service.h"
#include "../../Service/FSService/realisation/fs_service.h"
#include "../../Service/UserManagementService/realisation/user_management_service.h"
#include "Service/SessionService/realisation/session_service.h"

/**
 * @brief Интерфейс загрузчика зависимостей.
 *
 * Предоставляет доступ ко всем основным компонентам системы,
 * обеспечивая инверсию зависимостей и централизованное управление объектами.
 */
class ILoader {
public:
    virtual ~ILoader() = default;

    /**
     * @brief Получить представление (View)
     * @return Ссылка на объект представления
     */
    virtual IView& getView() = 0;

    /**
     * @brief Получить маппер объектов файловой системы
     * @return Ссылка на полиморфный маппер объектов файловой системы
     */
    virtual PolymorphicFSObjectMapper& getFsObjectMapper() = 0;

    /**
     * @brief Получить маппер команд
     * @return Ссылка на полиморфный маппер команд
     */
    virtual PolymorphicCommandMapper& getCommandMapper() = 0;

    /**
     * @brief Получить репозиторий файловой системы
     * @return Ссылка на репозиторий файловой системы
     */
    virtual IFileSystemRepository& getFsRepository() = 0;

    /**
     * @brief Получить репозиторий пользователей
     * @return Ссылка на репозиторий пользователей
     */
    virtual IUserRepository& getUserRepository() = 0;

    /**
     * @brief Получить репозиторий групп
     * @return Ссылка на репозиторий групп
     */
    virtual IGroupRepository& getGroupRepository() = 0;

    /**
     * @brief Получить репозиторий команд
     * @return Ссылка на репозиторий команд
     */
    virtual ICommandRepository& getCommandRepository() = 0;

    /**
     * @brief Получить сервис команд
     * @return Ссылка на сервис команд
     */
    virtual ICommandService& getCommandService() = 0;

    /**
     * @brief Получить сервис состояния файловой системы
     * @return Ссылка на сервис состояния файловой системы
     */
    virtual IStateService& getFsStateService() = 0;

    /**
     * @brief Получить сервис состояния пользователей
     * @return Ссылка на сервис состояния пользователей
     */
    virtual IStateService& getUserStateService() = 0;

    /**
     * @brief Получить сервис состояния групп
     * @return Ссылка на сервис состояния групп
     */
    virtual IStateService& getGroupStateService() = 0;

    /**
     * @brief Получить сервис состояния команд
     * @return Ссылка на сервис состояния команд
     */
    virtual IStateService& getCommandStateService() = 0;

    /**
     * @brief Получить сервис безопасности
     * @return Ссылка на сервис безопасности
     */
    virtual ISecurityService& getSecurityService() = 0;

    /**
     * @brief Получить сервис файловой системы
     * @return Ссылка на сервис файловой системы
     */
    virtual IFileSystemService& getFsService() = 0;

    /**
     * @brief Получить сервис управления пользователями
     * @return Ссылка на сервис управления пользователями
     */
    virtual IUserManagementService& getUserManagementService() = 0;

    /**
     * @brief Получить сервис сессий
     * @return Ссылка на сервис сессий
     */
    virtual ISessionService& getSessionService() = 0;
};

#endif