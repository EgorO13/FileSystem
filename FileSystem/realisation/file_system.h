#ifndef LAB3_FILE_SYSTEM_H
#define LAB3_FILE_SYSTEM_H

#include <memory>
#include <string>
#include <map>
#include "FileSystem/interface/i_file_system.h"
#include "../../Loader/realisation/loader.h"
#include "Threads/Metric/MetricFactory/metric_factory.h"
#include "base.h"

/**
 * @brief Реализация файловой системы для работы со всей системой.
 *
 * Класс предоставляет единый интерфейс для выполнения операций с файловой системой:
 * управление файлами и директориями, работа с пользователями и группами,
 * управление правами доступа, загрузка и сохранение состояния системы.
 * Использует загрузчик (ILoader) для доступа к сервисам и репозиториям.
 */
class FileSystem final : public IFileSystem {
private:
    std::unique_ptr<ILoader> loader_; ///< Загрузчик сервисов и репозиториев

    /**
     * @brief Создать данные по умолчанию при инициализации системы
     *
     * Создает администратора, группу администраторов и настраивает базовые права.
     */
    void createDefaultData();

public:
    /**
     * @brief Конструктор файловой системы
     * @param loader Уникальный указатель на загрузчик сервисов
     * @throws std::runtime_error если loader равен nullptr
     */
    FileSystem(std::unique_ptr<ILoader> loader);

    /**
     * @brief Войти в систему под указанным пользователем
     * @param username Имя пользователя
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult login(const std::string& username) override;

    /**
     * @brief Выйти из системы
     */
    void logout() override;

    /**
     * @brief Проверить, выполнен ли вход в систему
     * @return true если пользователь аутентифицирован, иначе false
     */
    bool isLoggedIn() const override;

    /**
     * @brief Получить текущего пользователя
     * @return Указатель на текущего пользователя или nullptr если не аутентифицирован
     */
    User* getCurrentUser() const override;

    /**
     * @brief Получить сервис безопасности
     * @return Ссылка на сервис безопасности
     */
    ISecurityService& getSecurityService() const override;

    /**
     * @brief Создать файл
     * @param path Путь к создаваемому файлу
     * @param content Содержимое файла
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult createFile(const std::string& path, const std::string& content = "") override;

    /**
     * @brief Прочитать содержимое файла
     * @param path Путь к файлу
     * @return Результат операции с содержимым файла или сообщением об ошибке
     */
    FileSystemResult readFile(const std::string& path) override;

    /**
     * @brief Записать содержимое в файл
     * @param path Путь к файлу
     * @param content Содержимое для записи
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult writeFile(const std::string& path, const std::string& content) override;

    /**
     * @brief Удалить файл
     * @param path Путь к файлу
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult deleteFile(const std::string& path) override;

    /**
     * @brief Скопировать файл
     * @param source Путь к исходному файлу
     * @param dest Путь к целевому файлу
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult copyFile(const std::string& source, const std::string& dest) override;

    /**
     * @brief Переместить файл
     * @param source Путь к исходному файлу
     * @param dest Путь к целевому файлу
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult moveFile(const std::string& source, const std::string& dest) override;

    /**
     * @brief Создать директорию
     * @param path Путь к создаваемой директории
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult createDirectory(const std::string& path) override;

    /**
     * @brief Удалить директорию
     * @param path Путь к директории
     * @param recursive Флаг рекурсивного удаления
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult deleteDirectory(const std::string& path, bool recursive = false) override;

    /**
     * @brief Получить список содержимого директории
     * @param path Путь к директории (пустая строка - текущая директория)
     * @return Результат операции со списком файлов или сообщением об ошибке
     */
    FileSystemResult listDirectory(const std::string& path = "") override;

    /**
     * @brief Изменить текущую рабочую директорию
     * @param path Путь к новой директории
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult changeDirectory(const std::string& path) override;

    /**
     * @brief Получить текущий путь
     * @return Текущий абсолютный путь
     */
    std::string getCurrentPath() const override;

    /**
     * @brief Изменить права доступа для объекта
     * @param path Путь к объекту
     * @param perms Карта новых прав доступа
     * @param forAll Применить для всех пользователей
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult changePermissions(const std::string& path, const std::map<PermissionType, PermissionEffect>& perms, bool forAll) override;

    /**
     * @brief Изменить владельца объекта
     * @param path Путь к объекту
     * @param newOwner Имя нового владельца
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult changeOwner(const std::string& path, const std::string& newOwner) override;

    /**
     * @brief Получить пользователя по имени
     * @param username Имя пользователя
     * @return Указатель на пользователя или nullptr если не найден
     */
    User* getUser(const std::string& username) override;

    /**
     * @brief Получить группу по имени
     * @param groupname Имя группы
     * @return Указатель на группу или nullptr если не найдена
     */
    Group* getGroup(const std::string& groupname) override;

    /**
     * @brief Создать пользователя
     * @param username Имя пользователя
     * @param isAdmin Флаг администратора
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult createUser(const std::string& username, bool isAdmin = false) override;

    /**
     * @brief Удалить пользователя
     * @param username Имя пользователя
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult deleteUser(const std::string& username) override;

    /**
     * @brief Создать группу
     * @param groupName Имя группы
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult createGroup(const std::string& groupName) override;

    /**
     * @brief Удалить группу
     * @param groupName Имя группы
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult deleteGroup(const std::string& groupName) override;

    /**
     * @brief Добавить пользователя в группу
     * @param username Имя пользователя
     * @param groupName Имя группы
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult addUserToGroup(const std::string& username, const std::string& groupName) override;

    /**
     * @brief Получить информацию о файле
     * @param path Путь к файлу
     * @return Результат операции с информацией о файле или сообщением об ошибке
     */
    FileSystemResult getFileInfo(const std::string& path) override;

    /**
     * @brief Найти файлы по шаблону
     * @param pattern Шаблон поиска
     * @param startPath Начальный путь для поиска
     * @return Результат операции со списком найденных файлов или сообщением об ошибке
     */
    FileSystemResult find(const std::string& pattern, const std::string& startPath = "") override;
    /**
     * @brief Статистика файловой системы
     * @param path Путь от какой директории собирать статистику
     * @param threadCount Максимальное число потоков
     * @return Результат сбора статистики
     */
    FileSystemResult getStatistics(int threadCount = 0, bool ignorePermissions = false) override;

    /**
     * @brief Создать N случайных элементов в файловой системе
     * @param count Количество элементов для создания
     * @return Результат операции
     */
    CommandResult createRandomElements(int count) override;

    /**
     * @brief Получить IFileSystemRepository&
     * @return Репозиторий
     */
    IFileSystemRepository& getRepository() const override;

    /**
     * @brief Сохранить проект в файл
     * @param filename Имя файла для сохранения
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult saveProject(const std::string& filename) override;

    /**
     * @brief Загрузить проект из файла
     * @param filename Имя файла для загрузки
     * @return Результат операции с сообщением об ошибке или успехе
     */
    FileSystemResult loadProject(const std::string& filename) override;
};

#endif