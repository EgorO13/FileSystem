#ifndef LAB3_I_COMMAND_SERVICE_H
#define LAB3_I_COMMAND_SERVICE_H

#include "base.h"
#include <memory>
#include <string>
#include <vector>

class ICommand;
class IFileSystem;

/**
 * @brief Абстрактный интерфейс сервиса для управления и выполнения команд.
 *
 * Определяет операции для работы с командами: выполнение, регистрация,
 * управление составными командами и получение информации о командах.
 */
class ICommandService {
public:
    virtual ~ICommandService() = default;

    /**
     * @brief Выполняет команду с указанными аргументами
     * @param commandName Имя команды
     * @param args Аргументы команды
     * @param fs Файловая система
     * @return Результат выполнения команды
     */
    virtual CommandResult executeCommand(const std::string& commandName, const std::vector<std::string>& args, IFileSystem& fs) = 0;

    /**
     * @brief Регистрирует новую команду в сервисе
     * @param command Команда для регистрации
     * @return true, если команда успешно зарегистрирована, false в противном случае
     */
    virtual bool registerCommand(std::unique_ptr<ICommand> command) = 0;

    /**
     * @brief Удаляет команду из сервиса
     * @param commandName Имя команды для удаления
     * @return true, если команда успешно удалена, false в противном случае
     */
    virtual bool unregisterCommand(const std::string& commandName) = 0;

    /**
     * @brief Создает новую составную команду
     * @param name Имя составной команды
     * @param description Описание команды
     * @return true, если команда успешно создана, false в противном случае
     */
    virtual bool createCompositeCommand(const std::string& name, const std::string& description) = 0;

    /**
     * @brief Удаляет составную команду
     * @param name Имя составной команды
     * @return true, если команда успешно удалена, false в противном случае
     */
    virtual bool deleteCompositeCommand(const std::string& name) = 0;

    /**
     * @brief Добавляет подкоманду в составную команду
     * @param compositeName Имя составной команды
     * @param subCommandName Имя подкоманды
     * @param fixedArgs Фиксированные аргументы для подкоманды
     * @param dynamicIndices Индексы динамических аргументов
     * @return true, если подкоманда успешно добавлена, false в противном случае
     */
    virtual bool addToComposite(const std::string& compositeName, const std::string& subCommandName, const std::vector<std::string>& fixedArgs = {}, const std::vector<int>& dynamicIndices = {}) = 0;

    /**
     * @brief Удаляет подкоманду из составной команды по индексу
     * @param compositeName Имя составной команды
     * @param index Индекс подкоманды для удаления
     * @return true, если подкоманда успешно удалена, false в противном случае
     */
    virtual bool removeFromComposite(const std::string& compositeName, size_t index) = 0;

    /**
     * @brief Получает составную команду по имени
     * @param name Имя составной команды
     * @return Указатель на составную команду или nullptr, если команда не найдена
     */
    virtual ICommand* getCompositeCommand(const std::string& name) const = 0;

    /**
     * @brief Получает список доступных команд
     * @return Вектор имен доступных команд
     */
    virtual std::vector<std::string> getAvailableCommands() const = 0;

    /**
     * @brief Получает команду по имени
     * @param name Имя команды
     * @return Указатель на команду или nullptr, если команда не найдена
     */
    virtual ICommand* getCommand(const std::string& name) const = 0;

    /**
     * @brief Проверяет существование команды
     * @param name Имя команды
     * @return true, если команда существует, false в противном случае
     */
    virtual bool commandExists(const std::string& name) const = 0;

    /**
     * @brief Получает список составных команд
     * @return Вектор имен составных команд
     */
    virtual std::vector<std::string> getCompositeCommands() const = 0;

    /**
     * @brief Получает информацию о составной команде в виде строки
     * @param name Имя составной команды
     * @return Строка с информацией о команде
     */
    virtual std::string getCompositeInfo(const std::string& name) const = 0;
};

#endif