#ifndef LAB3_COMMAND_SERVICE_H
#define LAB3_COMMAND_SERVICE_H

#include "../interface/i_command_service.h"
#include "Command/Commands/realisation/Composite/composite_command.h"
#include "Command/CommandRep/interface/i_command_repository.h"
#include "Service/SecurityService/interface/i_security_service.h"
#include <memory>
#include <set>

/**
 * @brief Сервис для выполнения и управления командами файловой системы.
 *
 * Обеспечивает выполнение команд, включая составные, с проверкой прав доступа
 * и предотвращением рекурсии. Также предоставляет методы для регистрации,
 * удаления и управления составными командами.
 */
class CommandService : public ICommandService {
private:
    ICommandRepository& commandRepository; ///< Ссылка на репозиторий команд
    ISecurityService& securityService;     ///< Ссылка на сервис безопасности

    /**
     * @brief Выполняет составную команду
     * @param composite Составная команда для выполнения
     * @param args Аргументы для команды
     * @param fs Файловая система
     * @return Результат выполнения команды
     */
    CommandResult executeCompositeCommand(ICompositable* composite, const std::vector<std::string>& args, IFileSystem& fs);

    /**
     * @brief Проверяет наличие рекурсии в составной команде
     * @param commandName Имя команды для проверки
     * @param composite Составная команда
     * @param visited Множество уже посещенных команд
     * @return true, если обнаружена рекурсия, false в противном случае
     */
    bool checkRecursion(const std::string& commandName, ICompositable* composite, std::set<std::string>& visited) const;

    /**
     * @brief Внутренняя рекурсивная проверка на наличие циклов
     * @param targetName Имя целевой команды
     * @param composite Текущая составная команда
     * @param visited Множество уже посещенных команд
     * @return true, если обнаружена рекурсия, false в противном случае
     */
    bool checkRecursionInternal(const std::string& commandName, ICompositable* composite, std::set<std::string>& visited) const;

public:
    /**
     * @brief Конструктор
     * @param cmdRepo Репозиторий команд
     * @param secService Сервис безопасности
     */
    CommandService(ICommandRepository& cmdRepo, ISecurityService& secService);

    /**
     * @brief Выполняет команду с указанными аргументами
     * @param commandName Имя команды
     * @param args Аргументы команды
     * @param fs Файловая система
     * @return Результат выполнения команды
     */
    CommandResult executeCommand(const std::string& commandName, const std::vector<std::string>& args, IFileSystem& fs) override;

    /**
     * @brief Регистрирует новую команду в сервисе
     * @param command Команда для регистрации
     * @return true, если команда успешно зарегистрирована, false в противном случае
     */
    bool registerCommand(std::unique_ptr<ICommand> command) override;

    /**
     * @brief Удаляет команду из сервиса
     * @param commandName Имя команды для удаления
     * @return true, если команда успешно удалена, false в противном случае
     */
    bool unregisterCommand(const std::string& commandName) override;

    /**
     * @brief Создает новую составную команду
     * @param name Имя составной команды
     * @param description Описание команды
     * @return true, если команда успешно создана, false в противном случае
     */
    bool createCompositeCommand(const std::string& name, const std::string& description) override;

    /**
     * @brief Удаляет составную команду
     * @param name Имя составной команды
     * @return true, если команда успешно удалена, false в противном случае
     */
    bool deleteCompositeCommand(const std::string& name) override;

    /**
     * @brief Добавляет подкоманду в составную команду
     * @param compositeName Имя составной команды
     * @param subCommandName Имя подкоманды
     * @param fixedArgs Фиксированные аргументы для подкоманды
     * @param dynamicIndices Индексы динамических аргументов
     * @return true, если подкоманда успешно добавлена, false в противном случае
     */
    bool addToComposite(const std::string& compositeName, const std::string& subCommandName, const std::vector<std::string>& fixedArgs = {}, const std::vector<int>& dynamicIndices = {}) override;

    /**
     * @brief Удаляет подкоманду из составной команды по индексу
     * @param compositeName Имя составной команды
     * @param index Индекс подкоманды для удаления
     * @return true, если подкоманда успешно удалена, false в противном случае
     */
    bool removeFromComposite(const std::string& compositeName, size_t index) override;

    /**
     * @brief Получает составную команду по имени
     * @param name Имя составной команды
     * @return Указатель на составную команду или nullptr, если команда не найдена
     */
    ICommand* getCompositeCommand(const std::string& name) const override;

    /**
     * @brief Получает список доступных команд
     * @return Вектор имен доступных команд
     */
    std::vector<std::string> getAvailableCommands() const override;

    /**
     * @brief Получает команду по имени
     * @param name Имя команды
     * @return Указатель на команду или nullptr, если команда не найдена
     */
    ICommand* getCommand(const std::string& name) const override;

    /**
     * @brief Проверяет существование команды
     * @param name Имя команды
     * @return true, если команда существует, false в противном случае
     */
    bool commandExists(const std::string& name) const override;

    /**
     * @brief Получает список составных команд
     * @return Вектор имен составных команд
     */
    std::vector<std::string> getCompositeCommands() const override;

    /**
     * @brief Получает информацию о составной команде в виде строки
     * @param name Имя составной команды
     * @return Строка с информацией о команде
     */
    std::string getCompositeInfo(const std::string& name) const override;
};

#endif