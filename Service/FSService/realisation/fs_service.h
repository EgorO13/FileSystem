#ifndef LAB3_FILE_SYSTEM_SERVICE_H
#define LAB3_FILE_SYSTEM_SERVICE_H

#include "../interface/i_fs_service.h"
#include "Service/SecurityService/interface/i_security_service.h"
#include "Service/SessionService/interface/i_session_service.h"
#include "Repository/FSRep/interface/i_fs_repository.h"
#include <map>

/**
 * @brief Сервис для управления файловой системой.
 *
 * Класс реализует операции работы с файлами и директориями,
 * включая создание, чтение, запись, удаление, копирование и перемещение.
 * Обеспечивает интеграцию с системами безопасности и сессиями пользователей.
 */
class FileSystemService : public IFileSystemService {
private:
    IFileSystemRepository& fsRepository; ///< Ссылка на репозиторий файловой системы
    ISecurityService& securityService;   ///< Ссылка на сервис безопасности
    ISessionService& sessionService;     ///< Ссылка на сервис сессий

    /**
     * @brief Разрешить путь относительно текущей директории пользователя
     * @param path Относительный или абсолютный путь
     * @return Абсолютный путь или пустая строка в случае ошибки
     */
    std::string resolveUserPath(const std::string& path) const;

    /**
     * @brief Получить объект файловой системы по пути
     * @param path Путь к объекту
     * @return Указатель на объект или nullptr если не найден
     */
    IFileSystemObject* getObject(const std::string& path) const;

    /**
     * @brief Проверить валидность пути для операции
     * @param path Проверяемый путь
     * @return true если путь валиден, иначе false
     */
    bool validateOperationPath(const std::string& path) const;

public:
    /**
     * @brief Конструктор сервиса файловой системы
     * @param fsRepo Репозиторий файловой системы
     * @param secService Сервис безопасности
     * @param sessionServ Сервис сессий
     */
    FileSystemService(IFileSystemRepository& fsRepo, ISecurityService& secService, ISessionService& sessionServ);

    /**
     * @brief Изменить текущую директорию пользователя
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к новой директории
     * @param currentDir Текущая директория
     * @return Указатель на новую директорию или nullptr в случае ошибки
     */
    IDirectory* changeDirectory(const User& user, const std::string& path, IDirectory* currentDir) override;

    /**
     * @brief Получить текущую директорию
     * @return Указатель на текущую директорию
     */
    IDirectory* getCurrentDirectory() const override { return sessionService.getCurrentDirectory(); }

    /**
     * @brief Получить корневую директорию
     * @return Указатель на корневую директорию
     */
    IDirectory* getRootDirectory() const override { return fsRepository.getRootDirectory(); }

    /**
     * @brief Получить список содержимого директории
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к директории (пустая строка - текущая директория)
     * @return Вектор информации о файлах и директориях
     */
    std::vector<FileInfo> listDirectory(const User& user, const std::string& path = "") override;

    /**
     * @brief Найти файлы по шаблону
     * @param user Пользователь, выполняющий операцию
     * @param pattern Шаблон поиска
     * @param startPath Начальный путь для поиска
     * @return Вектор путей к найденным файлам
     */
    std::vector<std::string> findFiles(const User& user, const std::string& pattern, const std::string& startPath = "") override;

    /**
     * @brief Создать файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к создаваемому файлу
     * @param content Содержимое файла
     * @return Указатель на созданный файл или nullptr в случае ошибки
     */
    IFile* createFile(const User& user, const std::string& path, const std::string& content = "") override;

    /**
     * @brief Прочитать содержимое файла
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @return Содержимое файла или пустая строка в случае ошибки
     */
    std::string readFile(const User& user, const std::string& path) override;

    /**
     * @brief Записать содержимое в файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @param content Записываемое содержимое
     * @param append Флаг добавления в конец файла
     * @return true если запись успешна, иначе false
     */
    bool writeFile(const User& user, const std::string& path, const std::string& content, bool append = false) override;

    /**
     * @brief Удалить файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @return true если удаление успешно, иначе false
     */
    bool deleteFile(const User& user, const std::string& path) override;

    /**
     * @brief Копировать файл
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходному файлу
     * @param destination Путь к целевому файлу
     * @return true если копирование успешно, иначе false
     */
    bool copyFile(const User& user, const std::string& source, const std::string& destination) override;

    /**
     * @brief Переместить файл
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходному файлу
     * @param destination Путь к целевому файлу
     * @return true если перемещение успешно, иначе false
     */
    bool moveFile(const User& user, const std::string& source, const std::string& destination) override;

    /**
     * @brief Создать директорию
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к создаваемой директории
     * @return Указатель на созданную директорию или nullptr в случае ошибки
     */
    IDirectory* createDirectory(const User& user, const std::string& path) override;

    /**
     * @brief Удалить директорию
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к директории
     * @param recursive Флаг рекурсивного удаления
     * @return true если удаление успешно, иначе false
     */
    bool deleteDirectory(const User& user, const std::string& path, bool recursive = false) override;

    /**
     * @brief Копировать директорию
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходной директории
     * @param destination Путь к целевой директории
     * @return true если копирование успешно, иначе false
     */
    bool copyDirectory(const User& user, const std::string& source, const std::string& destination) override;

    /**
     * @brief Переместить директорию
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходной директории
     * @param destination Путь к целевой директории
     * @return true если перемещение успешно, иначе false
     */
    bool moveDirectory(const User& user, const std::string& source, const std::string& destination) override;

    /**
     * @brief Изменить разрешения для объекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param path Путь к объекту
     * @param permissions Карта разрешений для изменения
     * @return true если изменение успешно, иначе false
     */
    bool changePermissions(unsigned int id, SubjectType s_type, const std::string& path, const std::map<PermissionType, PermissionEffect>& permissions) override;

    /**
     * @brief Изменить владельца объекта
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к объекту
     * @param newOwnerUsername Имя нового владельца
     * @return true если изменение успешно, иначе false
     */
    bool changeOwner(const User& user, const std::string& path, const std::string& newOwnerUsername) override;

    /**
     * @brief Заблокировать файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @param lockType Тип блокировки
     * @return true если блокировка успешна, иначе false
     */
    bool lockFile(const User& user, const std::string& path, Lock lockType) override;

    /**
     * @brief Разблокировать файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @return true если разблокировка успешна, иначе false
     */
    bool unlockFile(const User& user, const std::string& path) override;

    /**
     * @brief Проверить существование объекта по пути
     * @param path Путь к объекту
     * @return true если объект существует, иначе false
     */
    bool exists(const std::string& path) override;

    /**
     * @brief Проверить, является ли объект файлом
     * @param path Путь к объекту
     * @return true если объект является файлом, иначе false
     */
    bool isFile(const std::string& path) override;

    /**
     * @brief Проверить, является ли объект директорией
     * @param path Путь к объекту
     * @return true если объект является директорией, иначе false
     */
    bool isDirectory(const std::string& path) override;
};

#endif