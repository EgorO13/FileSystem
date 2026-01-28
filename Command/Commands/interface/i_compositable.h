#ifndef LAB3_I_COMPOSITABLE_H
#define LAB3_I_COMPOSITABLE_H

#include "base.h"
#include <vector>
#include <string>

/**
 * @brief Интерфейс для объектов, которые могут содержать другие команды (композитный паттерн).
 *
 * Определяет операции для работы с составными командами: добавление, удаление, получение списка подкоманд.
 */
class ICompositable {
public:
    virtual ~ICompositable() = default;

    /**
     * @brief Добавляет подкоманду в композитный объект
     * @param commandName Имя добавляемой команды
     * @param fixedArgs Фиксированные аргументы для команды
     * @param dynamicIndices Индексы динамических аргументов из входных параметров
     */
    virtual void addCommand(const std::string& commandName, const std::vector<std::string>& fixedArgs = {}, const std::vector<int>& dynamicIndices = {}) = 0;

    /**
     * @brief Удаляет подкоманду по индексу
     * @param index Индекс удаляемой подкоманды
     * @return true, если подкоманда успешно удалена, false в противном случае
     */
    virtual bool removeCommand(size_t index) = 0;

    /**
     * @brief Очищает все подкоманды
     */
    virtual void clearCommands() = 0;

    /**
     * @brief Получает список всех подкоманд
     * @return Константная ссылка на вектор подкоманд
     */
    virtual const std::vector<CommandEntry>& getSubCommands() const = 0;

    /**
     * @brief Получает количество подкоманд
     * @return Количество подкоманд в композитном объекте
     */
    virtual size_t getCommandCount() const = 0;
};

#endif