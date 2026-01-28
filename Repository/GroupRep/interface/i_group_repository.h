#ifndef LAB3_I_GROUP_REPOSITORY_H
#define LAB3_I_GROUP_REPOSITORY_H

#include "Entity/Group/group.h"
#include <memory>
#include <vector>
#include <string>
#include <set>

/**
 * @brief Интерфейс репозитория групп.
 *
 * Для управления группами пользователей:
 * создание, удаление групп, управление иерархией групп,
 * членством пользователей и предотвращение циклов.
 */
class IGroupRepository {
public:
    virtual ~IGroupRepository() = default;

    /**
     * @brief Получить группу по ID
     * @param id ID группы
     * @return Указатель на группу или nullptr если не найдена
     */
    virtual Group* getGroupById(unsigned int id) = 0;

    /**
     * @brief Получить группу по имени
     * @param name Имя группы
     * @return Указатель на группу или nullptr если не найдена
     */
    virtual Group* getGroupByName(const std::string& name) = 0;

    /**
     * @brief Получить все группы
     * @return Вектор указателей на все группы
     */
    virtual std::vector<Group*> getAllGroups() = 0;

    /**
     * @brief Сохранить группу в репозитории
     * @param group Уникальный указатель на группу для сохранения
     * @return true если сохранение успешно, иначе false
     */
    virtual bool saveGroup(std::unique_ptr<Group> group) = 0;

    /**
     * @brief Удалить группу по ID
     * @param id ID удаляемой группы
     * @return true если удаление успешно, иначе false
     */
    virtual bool deleteGroup(unsigned int id) = 0;

    /**
     * @brief Проверить существование группы по ID
     * @param id ID группы
     * @return true если группа существует, иначе false
     */
    virtual bool groupExists(unsigned int id) const = 0;

    /**
     * @brief Проверить существование группы по имени
     * @param name Имя группы
     * @return true если группа существует, иначе false
     */
    virtual bool groupExists(const std::string& name) const = 0;

    /**
     * @brief Добавить пользователя в группу
     * @param userId ID пользователя
     * @param groupId ID группы
     * @return true если операция успешна, иначе false
     */
    virtual bool addUserToGroup(unsigned int userId, unsigned int groupId) = 0;

    /**
     * @brief Удалить пользователя из группы
     * @param userId ID пользователя
     * @param groupId ID группы
     * @return true если операция успешна, иначе false
     */
    virtual bool removeUserFromGroup(unsigned int userId, unsigned int groupId) = 0;

    /**
     * @brief Добавить дочернюю группу к родительской
     * @param parentGroupId ID родительской группы
     * @param childGroupId ID дочерней группы
     * @return true если операция успешна, иначе false
     */
    virtual bool addSubgroup(unsigned int parentGroupId, unsigned int childGroupId) = 0;

    /**
     * @brief Удалить дочернюю группу из родительской
     * @param parentGroupId ID родительской группы
     * @param childGroupId ID дочерней группы
     * @return true если операция успешна, иначе false
     */
    virtual bool removeSubgroup(unsigned int parentGroupId, unsigned int childGroupId) = 0;

    /**
     * @brief Получить следующий доступный ID
     * @return Следующий ID
     */
    virtual unsigned int getNextId() = 0;

    /**
     * @brief Установить следующий ID
     * @param id Новое значение следующего ID
     * @return true если установка успешна, иначе false
     */
    virtual bool setNextId(unsigned int id) = 0;

    /**
     * @brief Получить прямые дочерние группы
     * @param groupId ID родительской группы
     * @return Вектор ID прямых дочерних групп
     */
    virtual std::vector<unsigned int> getDirectSubgroups(unsigned int groupId) = 0;

    /**
     * @brief Получить прямые родительские группы
     * @param groupId ID дочерней группы
     * @return Вектор ID прямых родительских групп
     */
    virtual std::vector<unsigned int> getDirectParentGroups(unsigned int groupId) = 0;

    /**
     * @brief Получить все родительские группы (включая косвенные)
     * @param groupId ID группы
     * @return Вектор ID всех родительских групп
     */
    virtual std::vector<unsigned int> getAllParentGroups(unsigned int groupId) = 0;

    /**
     * @brief Получить все дочерние группы (включая косвенные)
     * @param groupId ID группы
     * @return Вектор ID всех дочерних групп
     */
    virtual std::vector<unsigned int> getAllSubgroups(unsigned int groupId) = 0;

    /**
     * @brief Получить все группы пользователя
     * @param userId ID пользователя
     * @return Вектор ID всех групп пользователя
     */
    virtual std::vector<unsigned int> getAllGroupsOfUser(unsigned int userId) = 0;

    /**
     * @brief Проверить принадлежность пользователя к группе (рекурсивно)
     * @param userId ID пользователя
     * @param groupId ID группы
     * @return true если пользователь находится в группе (напрямую или косвенно), иначе false
     */
    virtual bool isUserInGroupRecursive(unsigned int userId, unsigned int groupId) = 0;

    /**
     * @brief Проверить является ли группа подгруппой другой группы (рекурсивно)
     * @param parentGroupId ID предполагаемой родительской группы
     * @param childGroupId ID предполагаемой дочерней группы
     * @return true если childGroupId является подгруппой parentGroupId, иначе false
     */
    virtual bool isSubgroupRecursive(unsigned int parentGroupId, unsigned int childGroupId) = 0;

    /**
     * @brief Проверить создаст ли добавление связи цикл в иерархии групп
     * @param parentId ID родительской группы
     * @param childId ID дочерней группы
     * @return true если добавление создаст цикл, иначе false
     */
    virtual bool wouldCreateCycle(unsigned int parentId, unsigned int childId) = 0;

    /**
     * @brief Очистить репозиторий
     */
    virtual void clear() = 0;
};

#endif