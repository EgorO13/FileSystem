#ifndef LAB3_GROUP_H
#define LAB3_GROUP_H

#include <string>
#include <set>
#include <vector>
#include <memory>

/**
 * @brief Класс группы пользователей.
 *
 * Представляет группу в системе с возможностью вложенности
 * (подгруппы) и управления списком пользователей.
 */
class Group {
private:
    unsigned int id;                ///< Идентификатор группы
    std::string name;               ///< Имя группы
    std::set<unsigned int> users;   ///< Множество идентификаторов пользователей
    std::set<unsigned int> subgroups; ///< Множество идентификаторов подгрупп

public:
    /**
     * @brief Конструктор по умолчанию
     */
    Group() : id(0), name("") {}

    /**
     * @brief Конструктор с параметрами
     * @param id Идентификатор группы
     * @param name Имя группы
     */
    Group(unsigned int id, const std::string& name) : id(id), name(name) {}

    Group(const Group& other) = default;
    Group(Group&& other) noexcept = default;
    Group& operator=(const Group& other) = default;
    Group& operator=(Group&& other) noexcept = default;

    /**
     * @brief Получить идентификатор группы
     * @return Идентификатор группы
     */
    unsigned int getId() const { return id; }

    /**
     * @brief Получить имя группы
     * @return Константная ссылка на имя группы
     */
    const std::string& getName() const { return name; }

    /**
     * @brief Получить список пользователей группы
     * @return Вектор идентификаторов пользователей
     */
    std::vector<unsigned int> getUsers() const {
        return std::vector<unsigned int>(users.begin(), users.end());
    }

    /**
     * @brief Получить список подгрупп
     * @return Вектор идентификаторов подгрупп
     */
    std::vector<unsigned int> getSubgroups() const {
        return std::vector<unsigned int>(subgroups.begin(), subgroups.end());
    }

    /**
     * @brief Установить новый идентификатор группы
     * @param newId Новый идентификатор
     */
    void setId(unsigned int newId) { id = newId; }

    /**
     * @brief Установить новое имя группы
     * @param newName Новое имя группы
     */
    void setName(const std::string& newName) { name = newName; }

    /**
     * @brief Добавить пользователя в группу
     * @param userId Идентификатор пользователя
     */
    void addUser(unsigned int userId) { users.insert(userId); }

    /**
     * @brief Удалить пользователя из группы
     * @param userId Идентификатор пользователя
     */
    void removeUser(unsigned int userId) { users.erase(userId); }

    /**
     * @brief Проверить наличие пользователя в группе
     * @param userId Идентификатор пользователя
     * @return true если пользователь в группе, иначе false
     */
    bool containsUser(unsigned int userId) const { return users.find(userId) != users.end(); }

    /**
     * @brief Добавить подгруппу
     * @param subgroupId Идентификатор подгруппы
     * @return true если подгруппа добавлена, иначе false
     */
    bool addSubgroup(unsigned int subgroupId) {
        if (subgroupId == id) return false;
        if (subgroups.find(subgroupId) != subgroups.end()) return false;
        subgroups.insert(subgroupId);
        return true;
    }

    /**
     * @brief Удалить подгруппу
     * @param subgroupId Идентификатор подгруппы
     * @return true если подгруппа удалена, иначе false
     */
    bool removeSubgroup(unsigned int subgroupId) { return subgroups.erase(subgroupId) > 0; }

    /**
     * @brief Проверить наличие подгруппы
     * @param subgroupId Идентификатор подгруппы
     * @return true если подгруппа существует, иначе false
     */
    bool containsSubgroup(unsigned int subgroupId) const { return subgroups.find(subgroupId) != subgroups.end(); }
};

#endif