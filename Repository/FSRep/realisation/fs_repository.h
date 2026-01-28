#ifndef LAB3_FS_REPOSITORY_H
#define LAB3_FS_REPOSITORY_H

#include "../interface/i_fs_repository.h"
#include "../../../Entity/Directory/realisation/directory_descriptor.h"
#include "../../../Entity/File/realisation/file_descriptor.h"
#include <map>
#include <memory>
#include <string>

/**
 * @brief Репозиторий файловой системы.
 *
 * Реализует интерфейс IFileSystemRepository для управления объектами
 * файловой системы с использованием адресов и путей.
 */
class FileSystemRepository : public IFileSystemRepository {
private:
    std::map<unsigned int, std::unique_ptr<IFileSystemObject>> objectsByAddress;  ///< Карта объектов по адресам
    IDirectory* rootDirectory;                                                    ///< Указатель на корневую директорию
    unsigned int nextAddress;                                                     ///< Следующий доступный адрес

    /**
     * @brief Инициализировать репозиторий данными по умолчанию
     */
    void initializeDefaultData();

    /**
     * @brief Рекурсивный поиск объектов в директории по шаблону
     * @param pattern Шаблон для поиска
     * @param directory Указатель на директорию для поиска
     * @param results Вектор для сохранения указателей на результаты
     */
    void findObjectsInDirectory(const std::string& pattern, IDirectory* directory, std::vector<IFileSystemObject*>& results) const;

    /**
     * @brief Рекурсивное построение пути к объекту
     * @param object Указатель на объект файловой системы
     * @param parts Вектор для накопления частей пути
     */
    void buildPathRecursive(IFileSystemObject* object, std::vector<std::string>& parts) const;

public:
    /**
     * @brief Конструктор репозитория
     */
    explicit FileSystemRepository();

    /**
     * @brief Деструктор репозитория
     */
    ~FileSystemRepository();

    /**
     * @brief Установить корневую директорию
     * @param rootDirectory Уникальный указатель на корневую директорию
     */
    void setRootDirectory(IDirectory* rootDirectory) override;

    /**
     * @brief Получить все объекты файловой системы
     * @return Вектор указателей на все объекты
     */
    std::vector<IFileSystemObject*> getAllObjects() const override;

    /**
     * @brief Получить корневую директорию
     * @return Указатель на корневую директорию
     */
    IDirectory* getRootDirectory() const override;

    /**
     * @brief Получить объект по адресу
     * @param address Адрес объекта
     * @return Указатель на объект или nullptr если не найден
     */
    IFileSystemObject* getObjectByAddress(unsigned int address) const override;

    /**
     * @brief Получить объект по пути
     * @param path Путь к объекту
     * @return Указатель на объект или nullptr если не найден
     */
    IFileSystemObject* getObjectByPath(const std::string& path) const override;

    /**
     * @brief Получить директорию по пути
     * @param path Путь к директории
     * @return Указатель на директорию или nullptr если не найдена или не является директорией
     */
    IDirectory* getDirectoryByPath(const std::string& path) const override;

    /**
     * @brief Получить файл по пути
     * @param path Путь к файлу
     * @return Указатель на файл или nullptr если не найден или не является файлом
     */
    IFile* getFileByPath(const std::string& path) const override;

    /**
     * @brief Сохранить объект в репозитории
     * @param object Уникальный указатель на объект для сохранения
     * @return true если сохранение успешно, иначе false
     */
    bool saveObject(std::unique_ptr<IFileSystemObject> object) override;

    /**
     * @brief Удалить объект из репозитория по адресу
     * @param address Адрес удаляемого объекта
     * @return true если удаление успешно, иначе false
     */
    bool deleteObject(unsigned int address) override;

    /**
     * @brief Проверить существование объекта по адресу
     * @param address Адрес объекта
     * @return true если объект существует, иначе false
     */
    bool objectExists(unsigned int address) const override;

    /**
     * @brief Проверить существование пути
     * @param path Путь для проверки
     * @return true если путь существует, иначе false
     */
    bool pathExists(const std::string& path) const override;

    /**
     * @brief Найти объекты по шаблону
     * @param pattern Шаблон поиска (поддерживает * и ?)
     * @param startPath Начальный путь для поиска (пустая строка означает корень)
     * @return Вектор указателей на найденные объекты
     */
    std::vector<IFileSystemObject*> findObjects(const std::string& pattern, const std::string& startPath = "") const override;

    /**
     * @brief Получить новый уникальный адрес
     * @return Новый адрес
     */
    unsigned int getAddress() override;

    /**
     * @brief Получить путь к объекту
     * @param object Указатель на объект файловой системы
     * @return Полный путь к объекту
     */
    std::string getPath(IFileSystemObject* object) const override;

    /**
     * @brief Очистить репозиторий
     */
    void clear() override;
};

#endif