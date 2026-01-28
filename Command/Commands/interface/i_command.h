#ifndef LAB3_I_COMMAND_H
#define LAB3_I_COMMAND_H

#include "base.h"
#include "FileSystem/interface/i_file_system.h"
#include <string>
#include <vector>

/**
 * @brief Абстрактный интерфейс для всех команд файловой системы.
 *
 * Для команд: имя, описание, использование, выполнение и валидация аргументов.
 */
class ICommand {
public:
    virtual ~ICommand() = default;

    /**
     * @brief Получает имя команды
     * @return Имя команды в виде строки
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Получает описание команды
     * @return Описание команды в виде строки
     */
    virtual std::string getDescription() const = 0;

    /**
     * @brief Получает строку использования команды
     * @return Строка с примером использования команды
     */
    virtual std::string getUsage() const = 0;

    /**
     * @brief Проверяет, требует ли команда прав администратора
     * @return true, если команда доступна только администраторам, иначе false
     */
    virtual bool isOnlyForAdmin() const = 0;

    /**
     * @brief Выполняет команду с указанными аргументами
     * @param args Вектор строк с аргументами команды
     * @param fs Ссылка на файловую систему
     * @return Результат выполнения команды
     */
    virtual CommandResult execute(const std::vector<std::string> &args, IFileSystem& fs) = 0;

    /**
     * @brief Проверяет корректность аргументов команды
     * @param args Вектор строк с аргументами для проверки
     * @return true, если аргументы корректны, иначе false
     */
    virtual bool validateArgs(const std::vector<std::string> &args) const = 0;
};

#endif