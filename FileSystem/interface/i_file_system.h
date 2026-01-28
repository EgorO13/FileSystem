#ifndef LAB3_I_FILE_SYSTEM_H
#define LAB3_I_FILE_SYSTEM_H

#include <string>
#include <map>
#include "base.h"
#include "../../Service/SecurityService/interface/i_security_service.h"
#include "Repository/FSRep/interface/i_fs_repository.h"
#include "Entity/Group/group.h"

/**
 * @brief Интерфейс файловой системы для работы со всей системой.
 *
 * Определяет базовые операции для управления файловой системой.
 */
class IFileSystem {
private:
    /**
     * @brief Создать данные по умолчанию при инициализации системы
     *
     * Создает администратора, группу администраторов и настраивает базовые права.
     * Используется при инициализации системы.
     */
    virtual void createDefaultData() = 0;

public:
    virtual ~IFileSystem() = default;

    /**
     * @brief Войти в систему под указанным пользователем
     * @param username Имя пользователя
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult login(const std::string& username) = 0;

    /**
     * @brief Выйти из системы
     */
    virtual void logout() = 0;

    /**
     * @brief Проверить, выполнен ли вход в систему
     * @return true если пользователь аутентифицирован, иначе false
     */
    virtual bool isLoggedIn() const = 0;

    /**
     * @brief Получить текущего пользователя
     * @return Указатель на текущего пользователя или nullptr если не аутентифицирован
     */
    virtual User* getCurrentUser() const = 0;

    /**
     * @brief Получить сервис безопасности
     * @return Ссылка на сервис безопасности
     */
    virtual ISecurityService& getSecurityService() const = 0;

    /**
     * @brief Создать файл
     * @param path Путь к создаваемому файлу
     * @param content Содержимое файла
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult createFile(const std::string& path, const std::string& content = "") = 0;

    /**
     * @brief Прочитать содержимое файла
     * @param path Путь к файлу
     * @return Результат операции с содержимым файла или сообщением об ошибке
     */
    virtual FileSystemResult readFile(const std::string& path) = 0;

    /**
     * @brief Записать содержимое в файл
     * @param path Путь к файлу
     * @param content Содержимое для записи
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult writeFile(const std::string& path, const std::string& content) = 0;

    /**
     * @brief Удалить файл
     * @param path Путь к файлу
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult deleteFile(const std::string& path) = 0;

    /**
     * @brief Скопировать файл
     * @param source Путь к исходному файлу
     * @param dest Путь к целевому файлу
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult copyFile(const std::string& source, const std::string& dest) = 0;

    /**
     * @brief Переместить файл
     * @param source Путь к исходному файлу
     * @param dest Путь к целевому файлу
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult moveFile(const std::string& source, const std::string& dest) = 0;

    /**
     * @brief Создать директорию
     * @param path Путь к создаваемой директории
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult createDirectory(const std::string& path) = 0;

    /**
     * @brief Удалить директорию
     * @param path Путь к директории
     * @param recursive Флаг рекурсивного удаления
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult deleteDirectory(const std::string& path, bool recursive = false) = 0;

    /**
     * @brief Получить список содержимого директории
     * @param path Путь к директории (пустая строка - текущая директория)
     * @return Результат операции со списком файлов или сообщением об ошибке
     */
    virtual FileSystemResult listDirectory(const std::string& path = "") = 0;

    /**
     * @brief Изменить текущую рабочую директорию
     * @param path Путь к новой директории
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult changeDirectory(const std::string& path) = 0;

    /**
     * @brief Получить текущий путь
     * @return Текущий абсолютный путь
     */
    virtual std::string getCurrentPath() const = 0;

    /**
     * @brief Изменить права доступа для объекта
     * @param path Путь к объекту
     * @param perms Карта новых прав доступа
     * @param forAll Применить для всех пользователей
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult changePermissions(const std::string& path, const std::map<PermissionType, PermissionEffect>& perms, bool forAll) = 0;

    /**
     * @brief Изменить владельца объекта
     * @param path Путь к объекту
     * @param newOwner Имя нового владельца
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult changeOwner(const std::string& path, const std::string& newOwner) = 0;

    /**
     * @brief Получить пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    virtual User* getUser(const std::string& username) = 0;

    /**
     * @brief Получить группу по имени
     * @param groupname Имя группы
     * @return Указатель на группу или nullptr если не найдена
     */
    virtual Group* getGroup(const std::string& groupname) = 0;

    /**
     * @brief Создать пользователя
     * @param username Имя пользователя
     * @param isAdmin Флаг администратора
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult createUser(const std::string& username, bool isAdmin = false) = 0;

    /**
     * @brief Удалить пользователя
     * @param username Имя пользователя
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult deleteUser(const std::string& username) = 0;

    /**
     * @brief Создать группу
     * @param groupName Имя группы
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult createGroup(const std::string& groupName) = 0;

    /**
     * @brief Удалить группу
     * @param groupName Имя группы
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult deleteGroup(const std::string& groupName) = 0;

    /**
     * @brief Добавить пользователя в группу
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult addUserToGroup(const std::string& username, const std::string& groupName) = 0;

    /**
     * @brief Получить информацию о файле
     * @param path Путь к файлу
     * @return Результат операции с информацией о файле или сообщением об ошибке
     */
    virtual FileSystemResult getFileInfo(const std::string& path) = 0;

    /**
     * @brief Найти файлы по шаблону
     * @param pattern Шаблон поиска
     * @param startPath Начальный путь для поиска
     * @return Результат операции со списком найденных файлов или сообщением об ошибке
     */
    virtual FileSystemResult find(const std::string& pattern, const std::string& startPath = "") = 0;

    /**
     * @brief Статистика файловой системы
     * @param path Путь от какой директории собирать статистику
     * @param threadCount Максимальное число потоков
     * @return Результат сбора статистики
     */
    virtual FileSystemResult getStatistics(int threadCount, bool ignorePermissions) = 0;

    /**
     * @brief Создать N случайных элементов в файловой системе
     * @param count Количество элементов для создания
     * @return Результат операции
     */
    virtual CommandResult createRandomElements(int count) = 0;

    /**
     * @brief Получить IFileSystemRepository&
     * @return Репозиторий
     */
    virtual IFileSystemRepository& getRepository() const = 0;

    /**
     * @brief Сохранить проект в файл
     * @param filename Имя файла для сохранения
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult saveProject(const std::string& filename) = 0;

    /**
     * @brief Загрузить проект из файла
     * @param filename Имя файла для загрузки
     * @return Результат операции с сообщением об ошибке или успехе
     */
    virtual FileSystemResult loadProject(const std::string& filename) = 0;
};

#endif