#ifndef LAB3_I_DIRECTORY_H
#define LAB3_I_DIRECTORY_H

#include <vector>
#include <string>

class IFileSystemObject;

/**
 * @brief Интерфейс директории файловой системы.
 *
 * Определяет базовые операции для работы с директориями:
 * добавление, удаление, поиск и перечисление дочерних объектов.
 */
class IDirectory {
public:
    virtual ~IDirectory() = default;

    /**
     * @brief Добавить дочерний объект
     * @param obj Умный указатель на дочерний объект
     * @return true если объект добавлен, иначе false
     */
    virtual bool addChild(IFileSystemObject* obj) = 0;

    /**
     * @brief Удалить дочерний объект по имени
     * @param name Имя удаляемого объекта
     * @return true если объект удален, иначе false
     */
    virtual bool removeChild(const std::string &name) = 0;

    /**
     * @brief Получить дочерний объект по имени
     * @param name Имя искомого объекта
     * @return Умный указатель на объект или nullptr если не найден
     */
    virtual IFileSystemObject* getChild(const std::string &name) const = 0;

    /**
     * @brief Получить количество дочерних объектов
     * @return Количество дочерних объектов
     */
    virtual int getChildCount() const = 0;

    /**
     * @brief Получить список всех дочерних объектов
     * @return Вектор умных указателей на дочерние объекты
     */
    virtual std::vector<IFileSystemObject*> listChild() const = 0;

    /**
     * @brief Проверить наличие дочернего объекта по имени
     * @param name Имя искомого объекта
     * @return true если объект существует, иначе false
     */
    virtual bool containChild(const std::string &name) const = 0;
};

#endif