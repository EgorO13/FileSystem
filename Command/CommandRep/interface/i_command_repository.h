#ifndef LAB3_I_COMMAND_REPOSITORY_H
#define LAB3_I_COMMAND_REPOSITORY_H

#include "../../Commands/interface/i_command.h"
#include <memory>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Абстрактный интерфейс репозитория команд.
 *
 * Определяет базовые операции для работы с хранилищем команд:
 * сохранение, удаление, получение и проверка команд.
 */
class ICommandRepository {
public:
    virtual ~ICommandRepository() = default;

    /**
     * @brief Сохраняет команду в репозитории
     * @param name Имя команды
     * @param command Указатель на команду
     * @return true, если команда успешно сохранена, false в противном случае
     */
    virtual bool saveCommand(const std::string& name, std::unique_ptr<ICommand> command) = 0;

    /**
     * @brief Удаляет команду из репозитория
     * @param name Имя команды для удаления
     * @return true, если команда успешно удалена, false если команда не найдена
     */
    virtual bool deleteCommand(const std::string& name) = 0;

    /**
     * @brief Получает команду по имени
     * @param name Имя команды
     * @return Указатель на команду или nullptr, если команда не найдена
     */
    virtual ICommand* getCommand(const std::string& name) const = 0;

    /**
     * @brief Проверяет существование команды в репозитории
     * @param name Имя команды
     * @return true, если команда существует, false в противном случае
     */
    virtual bool commandExists(const std::string& name) const = 0;

    /**
     * @brief Получает список имен всех команд в репозитории
     * @return Вектор строк с именами команд
     */
    virtual std::vector<std::string> getCommandNames() const = 0;

    /**
     * @brief Создает новую составную команду и сохраняет ее в репозитории
     * @param name Имя новой составной команды
     * @param description Описание команды
     * @return true, если команда успешно создана, false если команда с таким именем уже существует
     */
    virtual bool createCompositeCommand(const std::string& name, const std::string& description) = 0;

    /**
     * @brief Получает все команды из репозитория
     * @return Карта всех команд (имя -> команда)
     */
    virtual std::map<std::string, ICommand*> getAllCommands() const = 0;
};

#endif