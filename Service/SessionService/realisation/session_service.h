#ifndef LAB3_SESSION_SERVICE_H
#define LAB3_SESSION_SERVICE_H

#include "../interface/i_session_service.h"
#include "Service/SecurityService/interface/i_security_service.h"
#include "Repository/FSRep/realisation/fs_repository.h"

/**
 * @brief Реализация сервиса управления сессиями пользователей.
 *
 * Класс управляет текущей сессией пользователя, включая аутентификацию,
 * хранение текущего пользователя и рабочей директории.
 * Интегрируется с сервисом безопасности и репозиторием файловой системы.
 */
class SessionService : public ISessionService {
private:
    User* currentUser;                  ///< Указатель на текущего пользователя
    IDirectory* currentDirectory;       ///< Указатель на текущую директорию
    ISecurityService& securityService;  ///< Ссылка на сервис безопасности
    IFileSystemRepository& fsRepository;///< Ссылка на репозиторий файловой системы

public:
    /**
     * @brief Конструктор сервиса сессий
     * @param secService Сервис безопасности
     * @param fsRepo Репозиторий файловой системы
     */
    SessionService(ISecurityService& secService, IFileSystemRepository& fsRepo);

    /**
     * @brief Деструктор сервиса сессий
     */
    ~SessionService() = default;

    /**
     * @brief Получить текущего пользователя
     * @return Указатель на текущего пользователя или nullptr если пользователь не аутентифицирован
     */
    User* getCurrentUser() const override { return currentUser; }

    /**
     * @brief Получить текущую директорию
     * @return Указатель на текущую директорию
     */
    IDirectory* getCurrentDirectory() const override { return currentDirectory; }

    /**
     * @brief Установить текущего пользователя
     * @param user Указатель на пользователя
     */
    void setCurrentUser(User* user) override { currentUser = user; }

    /**
     * @brief Установить текущую директорию
     * @param dir Указатель на директорию
     */
    void setCurrentDirectory(IDirectory* dir) override { currentDirectory = dir; }

    /**
     * @brief Аутентифицировать пользователя по имени
     * @param username Имя пользователя
     * @return true если аутентификация успешна, иначе false
     */
    bool login(const std::string& username) override;

    /**
     * @brief Завершить текущую сессию пользователя
     */
    void logout() override;

    /**
     * @brief Проверить, аутентифицирован ли пользователь
     * @return true если пользователь аутентифицирован, иначе false
     */
    bool isLoggedIn() const override { return currentUser != nullptr; }
};

#endif