#ifndef LAB3_I_FS_SERVICE_H
#define LAB3_I_FS_SERVICE_H

#include "../../../Entity/Directory/interface/i_directory.h"
#include "../../../Entity/File/interface/i_file.h"
#include "Entity/User/user.h"
#include "base.h"
#include <string>
#include <vector>
#include <map>

/**
 * @brief Интерфейс сервиса файловой системы.
 *
 * Определяет базовые операции для работы с файлами и директориями.
 */
class IFileSystemService {
public:
    virtual ~IFileSystemService() = default;

    /**
     * @brief Изменить текущую директорию пользователя
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к новой директории
     * @param currentDir Текущая директория
     * @return Указатель на новую директорию или nullptr в случае ошибки
     */
    virtual IDirectory* changeDirectory(const User& user, const std::string& path, IDirectory* currentDir) = 0;

    /**
     * @brief Получить текущую директорию
     * @return Указатель на текущую директорию
     */
    virtual IDirectory* getCurrentDirectory() const = 0;

    /**
     * @brief Получить корневую директорию
     * @return Указатель на корневую директорию
     */
    virtual IDirectory* getRootDirectory() const = 0;

    /**
     * @brief Получить список содержимого директории
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к директории (пустая строка - текущая директория)
     * @return Вектор информации о файлах и директориях
     */
    virtual std::vector<FileInfo> listDirectory(const User& user, const std::string& path = "") = 0;

    /**
     * @brief Найти файлы по шаблону
     * @param user Пользователь, выполняющий операцию
     * @param pattern Шаблон поиска
     * @param startPath Начальный путь для поиска
     * @return Вектор путей к найденным файлам
     */
    virtual std::vector<std::string> findFiles(const User& user, const std::string& pattern, const std::string& startPath = "") = 0;

    /**
     * @brief Создать файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к создаваемому файлу
     * @param content Содержимое файла
     * @return Указатель на созданный файл или nullptr в случае ошибки
     */
    virtual IFile* createFile(const User& user, const std::string& path, const std::string& content = "") = 0;

    /**
     * @brief Прочитать содержимое файла
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @return Содержимое файла или пустая строка в случае ошибки
     */
    virtual std::string readFile(const User& user, const std::string& path) = 0;

    /**
     * @brief Записать содержимое в файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @param content Записываемое содержимое
     * @param append Флаг добавления в конец файла
     * @return true если запись успешна, иначе false
     */
    virtual bool writeFile(const User& user, const std::string& path, const std::string& content, bool append = false) = 0;

    /**
     * @brief Удалить файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @return true если удаление успешно, иначе false
     */
    virtual bool deleteFile(const User& user, const std::string& path) = 0;

    /**
     * @brief Копировать файл
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходному файлу
     * @param destination Путь к целевому файлу
     * @return true если копирование успешно, иначе false
     */
    virtual bool copyFile(const User& user, const std::string& source, const std::string& destination) = 0;

    /**
     * @brief Переместить файл
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходному файлу
     * @param destination Путь к целевому файлу
     * @return true если перемещение успешно, иначе false
     */
    virtual bool moveFile(const User& user, const std::string& source, const std::string& destination) = 0;

    /**
     * @brief Заблокировать файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @param lockType Тип блокировки
     * @return true если блокировка успешна, иначе false
     */
    virtual bool lockFile(const User& user, const std::string& path, Lock lockType) = 0;

    /**
     * @brief Разблокировать файл
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к файлу
     * @return true если разблокировка успешна, иначе false
     */
    virtual bool unlockFile(const User& user, const std::string& path) = 0;

    /**
     * @brief Создать директорию
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к создаваемой директории
     * @return Указатель на созданную директорию или nullptr в случае ошибки
     */
    virtual IDirectory* createDirectory(const User& user, const std::string& path) = 0;

    /**
     * @brief Удалить директорию
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к директории
     * @param recursive Флаг рекурсивного удаления
     * @return true если удаление успешно, иначе false
     */
    virtual bool deleteDirectory(const User& user, const std::string& path, bool recursive = false) = 0;

    /**
     * @brief Копировать директорию
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходной директории
     * @param destination Путь к целевой директории
     * @return true если копирование успешно, иначе false
     */
    virtual bool copyDirectory(const User& user, const std::string& source, const std::string& destination) = 0;

    /**
     * @brief Переместить директорию
     * @param user Пользователь, выполняющий операцию
     * @param source Путь к исходной директории
     * @param destination Путь к целевой директории
     * @return true если перемещение успешно, иначе false
     */
    virtual bool moveDirectory(const User& user, const std::string& source, const std::string& destination) = 0;

    /**
     * @brief Изменить разрешения для объекта
     * @param id Идентификатор субъекта
     * @param s_type Тип субъекта
     * @param path Путь к объекту
     * @param permissions Карта разрешений для изменения
     * @return true если изменение успешно, иначе false
     */
    virtual bool changePermissions(unsigned int id, SubjectType s_type, const std::string& path, const std::map<PermissionType, PermissionEffect>& permissions) = 0;

    /**
     * @brief Изменить владельца объекта
     * @param user Пользователь, выполняющий операцию
     * @param path Путь к объекту
     * @param newOwnerUsername Имя нового владельца
     * @return true если изменение успешно, иначе false
     */
    virtual bool changeOwner(const User& user, const std::string& path, const std::string& newOwnerUsername) = 0;

    /**
     * @brief Проверить существование объекта по пути
     * @param path Путь к объекту
     * @return true если объект существует, иначе false
     */
    virtual bool exists(const std::string& path) = 0;

    /**
     * @brief Проверить, является ли объект файлом
     * @param path Путь к объекту
     * @return true если объект является файлом, иначе false
     */
    virtual bool isFile(const std::string& path) = 0;

    /**
     * @brief Проверить, является ли объект директорией
     * @param path Путь к объекту
     * @return true если объект является директорией, иначе false
     */
    virtual bool isDirectory(const std::string& path) = 0;
};

#endif