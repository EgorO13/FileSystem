#ifndef LAB3_DIRECTORY_DESCRIPTOR_H
#define LAB3_DIRECTORY_DESCRIPTOR_H

#include "Entity/FSObject/realisation/fs_object.h"
#include "Entity/Directory/interface/i_directory.h"
#include "Table/table.h"
#include "Entity/User/user.h"
#include <vector>

/**
 * @brief Класс дескриптора директории файловой системы.
 *
 * Реализует интерфейсы файловой системы и директории,
 * предоставляет методы для управления дочерними объектами.
 */
class DirectoryDescriptor : public FileSystemObject, public IDirectory {
private:
    Table<std::string, IFileSystemObject*> children;  ///< Таблица дочерних объектов

public:
    /**
     * @brief Конструктор директории
     * @param name Имя директории
     * @param parentAddress Адрес родительской директории
     * @param owner Владелец директории
     * @param adr Адрес директории в файловой системе
     */
    DirectoryDescriptor(const std::string &name, unsigned int parentAddress, const User &owner, unsigned int adr);

    /**
     * @brief Добавить дочерний объект
     * @param obj Умный указатель на дочерний объект
     * @return true если объект добавлен, иначе false
     */
    bool addChild(IFileSystemObject* obj) override;

    /**
     * @brief Удалить дочерний объект по имени
     * @param name Имя удаляемого объекта
     * @return true если объект удален, иначе false
     */
    bool removeChild(const std::string &name) override;

    /**
     * @brief Получить дочерний объект по имени
     * @param name Имя искомого объекта
     * @return Умный указатель на объект или nullptr если не найден
     */
    IFileSystemObject* getChild(const std::string &name) const override;

    /**
     * @brief Получить количество дочерних объектов
     * @return Количество дочерних объектов
     */
    int getChildCount() const override;

    /**
     * @brief Получить список всех дочерних объектов
     * @return Вектор умных указателей на дочерние объекты
     */
    std::vector<IFileSystemObject*> listChild() const override;

    /**
     * @brief Проверить наличие дочернего объекта по имени
     * @param name Имя искомого объекта
     * @return true если объект существует, иначе false
     */
    bool containChild(const std::string &name) const override;
};

#endif