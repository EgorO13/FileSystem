#ifndef LAB3_GROUP_REPOSITORY_H
#define LAB3_GROUP_REPOSITORY_H

#include "../interface/i_group_repository.h"
#include <map>
#include <unordered_map>
#include <set>
#include <vector>

/**
 * @brief Репозиторий групп пользователей.
 *
 * Реализует интерфейс IGroupRepository для управления группами,
 * поддержки иерархии групп (вложенности), управления членством
 * пользователей и предотвращения циклических зависимостей.
 */
class GroupRepository : public IGroupRepository {
private:
    std::map<unsigned int, std::unique_ptr<Group>> groupsById;                  ///< Карта групп по ID
    std::unordered_map<std::string, unsigned int> idByName;                    ///< Карта ID по имени группы
    std::multimap<unsigned int, unsigned int> userToGroups;                    ///< Множество связей пользователь->группа
    std::multimap<unsigned int, unsigned int> groupToUsers;                    ///< Множество связей группа->пользователь
    std::multimap<unsigned int, unsigned int> parentToChildGroups;             ///< Множество связей родительская группа->дочерняя группа
    std::multimap<unsigned int, unsigned int> childToParentGroups;             ///< Множество связей дочерняя группа->родительская группа
    unsigned int nextId;                                                       ///< Следующий доступный ID

    /**
     * @brief Инициализировать репозиторий данными по умолчанию
     */
    void initializeDefaultData();

    /**
     * @brief Проверить отсутствие циклов при добавлении связи между группами
     * @param parentId ID родительской группы
     * @param childId ID дочерней группы
     * @param visited Множество посещенных групп
     * @return true если циклов нет, иначе false
     */
    bool validateNoCycle(unsigned int parentId, unsigned int childId,
                        std::set<unsigned int>& visited) const;

public:
    /**
     * @brief Конструктор репозитория
     */
    GroupRepository();

    /**
     * @brief Деструктор репозитория
     */
    ~GroupRepository() = default;

    /**
     * @brief Получить группу по ID
     * @param id ID группы
     * @return Указатель на группу или nullptr если не найдена
     */
    Group* getGroupById(unsigned int id) override;

    /**
     * @brief Получить группу по имени
     * @param name Имя группы
     * @return Указатель на группу или nullptr если не найдена
     */
    Group* getGroupByName(const std::string& name) override;

    /**
     * @brief Получить все группы
     * @return Вектор указателей на все группы
     */
    std::vector<Group*> getAllGroups() override;

    /**
     * @brief Сохранить группу в репозитории
     * @param group Уникальный указатель на группу для сохранения
     * @return true если сохранение успешно, иначе false
     */
    bool saveGroup(std::unique_ptr<Group> group) override;

    /**
     * @brief Удалить группу по ID
     * @param id ID удаляемой группы
     * @return true если удаление успешно, иначе false
     */
    bool deleteGroup(unsigned int id) override;

    /**
     * @brief Проверить существование группы по ID
     * @param id ID группы
     * @return true если группа существует, иначе false
     */
    bool groupExists(unsigned int id) const override;

    /**
     * @brief Проверить существование группы по имени
     * @param name Имя группы
     * @return true если группа существует, иначе false
     */
    bool groupExists(const std::string& name) const override;

    /**
     * @brief Добавить пользователя в группу
     * @param userId ID пользователя
     * @param groupId ID группы
     * @return true если операция успешна, иначе false
     */
    bool addUserToGroup(unsigned int userId, unsigned int groupId) override;

    /**
     * @brief Удалить пользователя из группы
     * @param userId ID пользователя
     * @param groupId ID группы
     * @return true если операция успешна, иначе false
     */
    bool removeUserFromGroup(unsigned int userId, unsigned int groupId) override;

    /**
     * @brief Добавить дочернюю группу к родительской
     * @param parentGroupId ID родительской группы
     * @param childGroupId ID дочерней группы
     * @return true если операция успешна, иначе false
     */
    bool addSubgroup(unsigned int parentGroupId, unsigned int childGroupId) override;

    /**
     * @brief Удалить дочернюю группу из родительской
     * @param parentGroupId ID родительской группы
     * @param childGroupId ID дочерней группы
     * @return true если операция успешна, иначе false
     */
    bool removeSubgroup(unsigned int parentGroupId, unsigned int childGroupId) override;

    /**
     * @brief Получить следующий доступный ID
     * @return Следующий ID
     */
    unsigned int getNextId() override { return nextId++; }

    /**
     * @brief Установить следующий ID
     * @param id Новое значение следующего ID
     * @return true если установка успешна, иначе false
     */
    bool setNextId(unsigned int id) override;

    /**
     * @brief Получить прямые дочерние группы
     * @param groupId ID родительской группы
     * @return Вектор ID прямых дочерних групп
     */
    std::vector<unsigned int> getDirectSubgroups(unsigned int groupId) override;

    /**
     * @brief Получить прямые родительские группы
     * @param groupId ID дочерней группы
     * @return Вектор ID прямых родительских групп
     */
    std::vector<unsigned int> getDirectParentGroups(unsigned int groupId) override;

    /**
     * @brief Получить все родительские группы (включая косвенные)
     * @param groupId ID группы
     * @return Вектор ID всех родительских групп
     */
    std::vector<unsigned int> getAllParentGroups(unsigned int groupId) override;

    /**
     * @brief Получить все дочерние группы (включая косвенные)
     * @param groupId ID группы
     * @return Вектор ID всех дочерних групп
     */
    std::vector<unsigned int> getAllSubgroups(unsigned int groupId) override;

    /**
     * @brief Получить все группы пользователя
     * @param userId ID пользователя
     * @return Вектор ID всех групп пользователя
     */
    std::vector<unsigned int> getAllGroupsOfUser(unsigned int userId) override;

    /**
     * @brief Проверить принадлежность пользователя к группе (рекурсивно)
     * @param userId ID пользователя
     * @param groupId ID группы
     * @return true если пользователь находится в группе (напрямую или косвенно), иначе false
     */
    bool isUserInGroupRecursive(unsigned int userId, unsigned int groupId) override;

    /**
     * @brief Проверить является ли группа подгруппой другой группы (рекурсивно)
     * @param parentGroupId ID предполагаемой родительской группы
     * @param childGroupId ID предполагаемой дочерней группы
     * @return true если childGroupId является подгруппой parentGroupId, иначе false
     */
    bool isSubgroupRecursive(unsigned int parentGroupId, unsigned int childGroupId) override;

    /**
     * @brief Проверить создаст ли добавление связи цикл в иерархии групп
     * @param parentId ID родительской группы
     * @param childId ID дочерней группы
     * @return true если добавление создаст цикл, иначе false
     */
    bool wouldCreateCycle(unsigned int parentId, unsigned int childId) override;

    /**
     * @brief Очистить репозиторий
     */
    void clear() override;
};

#endif