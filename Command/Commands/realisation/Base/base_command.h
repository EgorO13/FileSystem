#ifndef LAB3_BASE_COMMAND_H
#define LAB3_BASE_COMMAND_H

#include "Command/Commands/interface/i_command.h"
#include "FileSystem/interface/i_file_system.h"
#include "base.h"
#include <string>
#include <vector>

/**
 * @brief Базовый класс для всех команд файловой системы.
 *
 * Реализует общую логику для команд: хранение имени, описания, использования и информации о требуемых правах доступа.
 */
class BaseCommand : public ICommand {
protected:
    std::string name;        ///< Имя команды
    std::string description; ///< Описание команды
    std::string usage;       ///< Строка использования команды
    bool onlyForAdmin;       ///< Флаг, указывающий на требование прав администратора

public:
    BaseCommand() = delete;

    /**
     * @brief Конструктор базовой команды
     * @param name Имя команды
     * @param description Описание команды
     * @param usage Строка использования команды
     * @param adminOnly Флаг, указывающий на требование прав администратора
     * @throws std::invalid_argument Если имя команды пустое или содержит недопустимые символы
     */
    BaseCommand(const std::string &name, const std::string &description, const std::string &usage, bool adminOnly = false);

    /**
     * @brief Получает имя команды
     * @return Имя команды
     */
    std::string getName() const override { return name; }

    /**
     * @brief Получает описание команды
     * @return Описание команды
     */
    std::string getDescription() const override { return description; }

    /**
     * @brief Получает строку использования команды
     * @return Строка использования команды
     */
    std::string getUsage() const override { return usage; }

    /**
     * @brief Проверяет, требует ли команда прав администратора
     * @return true, если команда доступна только администраторам, иначе false
     */
    bool isOnlyForAdmin() const override { return onlyForAdmin; }

    /**
     * @brief Проверяет корректность аргументов команды
     * @param args Аргументы для проверки
     * @return true, если аргументы корректны, иначе false
     */
    virtual bool validateArgs(const std::vector<std::string> &args) const = 0;

    /**
     * @brief Выполняет команду с указанными аргументами
     * @param args Аргументы команды
     * @param fs Ссылка на файловую систему
     * @return Результат выполнения команды
     */
    virtual CommandResult execute(const std::vector<std::string> &args, IFileSystem& fs) = 0;
};

#endif