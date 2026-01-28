#include "session_service.h"

SessionService::SessionService(ISecurityService& secService, IFileSystemRepository& fsRepo)
    : securityService(secService), fsRepository(fsRepo), currentUser(nullptr), currentDirectory(nullptr) {
}

bool SessionService::login(const std::string& username) {
    User* user = securityService.authenticate(username);
    if (!user) return false;

    currentUser = user;
    currentDirectory = fsRepository.getRootDirectory();
    return true;
}

void SessionService::logout() {
    currentUser = nullptr;
    currentDirectory = fsRepository.getRootDirectory();
}