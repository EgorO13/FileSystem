#ifndef LAB3_I_FS_REPOSITORY_H
#define LAB3_I_FS_REPOSITORY_H

#include "../../../Entity/FSObject/interface/i_fs_object.h"
#include "../../../Entity/Directory/interface/i_directory.h"
#include "../../../Entity/File/interface/i_file.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @brief Интерфейс репозитория файловой системы.
 *
 * Для работы с объектами файловой системы:
 * сохранение, поиск, удаление объектов, получение по путям и адресам.
 */
class IFileSystemRepository {
public:
    virtual ~IFileSystemRepository() = default;

    /**
     * @brief Установить корневую директорию
     * @param rootDirectory Указатель на корневую директорию
     */
    virtual void setRootDirectory(IDirectory* rootDirectory) = 0;

    /**
     * @brief Получить все объекты файловой системы
     * @return Вектор указателей на все объекты
     */
    virtual std::vector<IFileSystemObject*> getAllObjects() const = 0;

    /**
     * @brief Получить корневую директорию
     * @return Указатель на корневую директорию
     */
    virtual IDirectory* getRootDirectory() const = 0;

    /**
     * @brief Получить объект по адресу
     * @param address Адрес объекта
     * @return Указатель на объект или nullptr если не найден
     */
    virtual IFileSystemObject* getObjectByAddress(unsigned int address) const = 0;

    /**
     * @brief Получить объект по пути
     * @param path Путь к объекту
     * @return Указатель на объект или nullptr если не найден
     */
    virtual IFileSystemObject* getObjectByPath(const std::string& path) const = 0;

    /**
     * @brief Получить директорию по пути
     * @param path Путь к директории
     * @return Указатель на директорию или nullptr если не найдена или не является директорией
     */
    virtual IDirectory* getDirectoryByPath(const std::string& path) const = 0;

    /**
     * @brief Получить файл по пути
     * @param path Путь к файлу
     * @return Указатель на файл или nullptr если не найден или не является файлом
     */
    virtual IFile* getFileByPath(const std::string& path) const = 0;

    /**
     * @brief Сохранить объект в репозитории
     * @param object Уникальный указатель на объект для сохранения
     * @return true если сохранение успешно, иначе false
     */
    virtual bool saveObject(std::unique_ptr<IFileSystemObject> object) = 0;

    /**
     * @brief Удалить объект из репозитория по адресу
     * @param address Адрес удаляемого объекта
     * @return true если удаление успешно, иначе false
     */
    virtual bool deleteObject(unsigned int address) = 0;

    /**
     * @brief Проверить существование объекта по адресу
     * @param address Адрес объекта
     * @return true если объект существует, иначе false
     */
    virtual bool objectExists(unsigned int address) const = 0;

    /**
     * @brief Проверить существование пути
     * @param path Путь для проверки
     * @return true если путь существует, иначе false
     */
    virtual bool pathExists(const std::string& path) const = 0;

    /**
     * @brief Найти объекты по шаблону
     * @param pattern Шаблон поиска (поддерживает * и ?)
     * @param startPath Начальный путь для поиска (пустая строка означает корень)
     * @return Вектор указателей на найденные объекты
     */
    virtual std::vector<IFileSystemObject*> findObjects(const std::string& pattern, const std::string& startPath = "") const = 0;

    /**
     * @brief Получить новый уникальный адрес
     * @return Новый адрес
     */
    virtual unsigned int getAddress() = 0;

    /**
     * @brief Получить путь к объекту
     * @param object Указатель на объект файловой системы
     * @return Полный путь к объекту
     */
    virtual std::string getPath(IFileSystemObject* object) const = 0;

    /**
     * @brief Очистить репозиторий
     */
    virtual void clear() = 0;
};

#endif