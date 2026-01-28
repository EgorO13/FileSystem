#include "loader.h"

#include "../../View/realisation/view.h"
#include "../../Command/CommandService/realisation/command_service.h"

PolymorphicFSObjectMapper& FSLoader::getFsObjectMapper() {
    if (!fsObjectMapper_) {
        fsSubtypeMappers_.emplace_back(std::make_unique<SuperFSObjectMapper<FileMapper>>());
        fsSubtypeMappers_.emplace_back(std::make_unique<SuperFSObjectMapper<DirectoryMapper>>());
        auto fsObjectMapper = std::make_unique<PolymorphicFSObjectMapper>();
        for (auto& fsSubtypeMapper : fsSubtypeMappers_) {
            fsObjectMapper->addSubMapper(*fsSubtypeMapper);
        }
        fsObjectMapper_ = std::move(fsObjectMapper);
    }
    return *fsObjectMapper_;
}

PolymorphicCommandMapper& FSLoader::getCommandMapper() {
    if (!commandMapper_) {
        commandSubtypeMappers_.emplace_back(std::make_unique<SuperCommandMapper<CompositeCommandMapper>>());
        auto commandMapper = std::make_unique<PolymorphicCommandMapper>();
        for (auto& commandSubtypeMapper : commandSubtypeMappers_) {
            commandMapper->addSubMapper(*commandSubtypeMapper);
        }
        commandMapper_ = std::move(commandMapper);
    }
    return *commandMapper_;
}

IView& FSLoader::getView() {
    if (!view_) view_ = std::make_unique<ConsoleView>();
    return *view_;
}


IFileSystemRepository& FSLoader::getFsRepository() {
    if (!fsRepository_) fsRepository_ = std::make_unique<FileSystemRepository>();
    return *fsRepository_;
}

IUserRepository& FSLoader::getUserRepository() {
    if (!userRepository_) userRepository_ = std::make_unique<UserRepository>();
    return *userRepository_;
}

IGroupRepository& FSLoader::getGroupRepository() {
    if (!groupRepository_) groupRepository_ = std::make_unique<GroupRepository>();
    return *groupRepository_;
}

ICommandRepository& FSLoader::getCommandRepository() {
    if (!commandRepository_) commandRepository_ = std::make_unique<CommandRepository>();
    return *commandRepository_;
}

ICommandService& FSLoader::getCommandService() {
    if (!commandService_) commandService_ = std::make_unique<CommandService>(getCommandRepository(), getSecurityService());
    return *commandService_;
}

IStateService& FSLoader::getFsStateService() {
    if (!fsStateService_) fsStateService_ = std::make_unique<FSStateService>(getFsRepository(), getUserRepository(), getFsObjectMapper());
    return *fsStateService_;
}

IStateService& FSLoader::getUserStateService() {
    if (!userStateService_) userStateService_ = std::make_unique<UserStateService>(getUserRepository());//getUserMapper()
    return *userStateService_;
}

IStateService& FSLoader::getGroupStateService() {
    if (!groupStateService_) groupStateService_ = std::make_unique<GroupStateService>(getGroupRepository());//getGroupMapper()
    return *groupStateService_;
}

IStateService& FSLoader::getCommandStateService() {
    if (!commandStateService_) commandStateService_ = std::make_unique<CompositeStateService>(getCommandRepository(), getCommandMapper());
    return *commandStateService_;
}

ISecurityService& FSLoader::getSecurityService() {
    if (!securityService_) securityService_ = std::make_unique<SecurityService>(getUserRepository(),getGroupRepository());
    return *securityService_;
}

IFileSystemService& FSLoader::getFsService() {
    if (!fsService_) fsService_ = std::make_unique<FileSystemService>(getFsRepository(),getSecurityService(),getSessionService());
    return *fsService_;
}

IUserManagementService& FSLoader::getUserManagementService() {
    if (!userManagementService_) userManagementService_ = std::make_unique<UserManagementService>(getUserRepository(),getGroupRepository(),getSecurityService());
    return *userManagementService_;
}

ISessionService& FSLoader::getSessionService() {
    if (!sessionService_) sessionService_ = std::make_unique<SessionService>(getSecurityService(),getFsRepository());
    return *sessionService_;
}