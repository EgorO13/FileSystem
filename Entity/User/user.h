#ifndef LAB3_USER_H
#define LAB3_USER_H

#include <string>
#include <set>
#include <vector>

/**
 * @brief Класс пользователя системы.
 *
 * Представляет пользователя с идентификатором, именем и списком групп, в которые он входит.
 */
class User {
private:
    unsigned int id;            ///< Идентификатор пользователя
    std::string name;           ///< Имя пользователя
    std::set<unsigned int> groups; ///< Множество идентификаторов групп

public:
    /**
     * @brief Конструктор по умолчанию
     */
    User() : id(0), name("guest") {}

    /**
     * @brief Конструктор с параметрами
     * @param id Идентификатор пользователя
     * @param name Имя пользователя
     */
    User(unsigned int id, const std::string& name) : id(id), name(name) {}

    User(const User& other) = default;
    User(User&& other) noexcept = default;
    User& operator=(const User& other) = default;
    User& operator=(User&& other) noexcept = default;

    /**
     * @brief Получить идентификатор пользователя
     * @return Идентификатор пользователя
     */
    unsigned int getId() const { return id; }

    /**
     * @brief Получить имя пользователя
     * @return Константная ссылка на имя пользователя
     */
    const std::string& getName() const { return name; }

    /**
     * @brief Установить новый идентификатор пользователя
     * @param newId Новый идентификатор
     */
    void setId(unsigned int newId) { id = newId; }

    /**
     * @brief Установить новое имя пользователя
     * @param newName Новое имя пользователя
     */
    void setName(const std::string& newName) { name = newName; }

    /**
     * @brief Добавить пользователя в группу
     * @param groupId Идентификатор группы
     */
    void addToGroup(unsigned int groupId) { groups.insert(groupId); }

    /**
     * @brief Удалить пользователя из группы
     * @param groupId Идентификатор группы
     */
    void removeFromGroup(unsigned int groupId) { groups.erase(groupId); }

    /**
     * @brief Проверить наличие пользователя в группе
     * @param groupId Идентификатор группы
     * @return true если пользователь в группе, иначе false
     */
    bool isInGroup(unsigned int groupId) const { return groups.find(groupId) != groups.end(); }

    /**
     * @brief Получить список групп пользователя
     * @return Вектор идентификаторов групп
     */
    std::vector<unsigned int> getGroups() const {
        return std::vector<unsigned int>(groups.begin(), groups.end());
    }

    /**
     * @brief Оператор сравнения
     * @return true если равны, иначе false
     */
    bool operator==(const User& other) const {
        return id == other.id && name == other.name;
    }
};

#endif