#ifndef LAB3_BASIC_COMMANDS_H
#define LAB3_BASIC_COMMANDS_H

#include "Command/Commands/realisation/Base/base_command.h"
#include <string>
#include <vector>

/**
 * @namespace BasicCommands
 * @brief Пространство имен, содержащее все базовые команды файловой системы.
 *
 * Включает команды для навигации, управления файлами и директориями,
 * работы с пользователями и группами.
 */
namespace BasicCommands {

    /**
     * @brief Команда для изменения текущей рабочей директории
     */
    class ChangeDirectoryCommand : public BaseCommand {
    public:
        ChangeDirectoryCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для просмотра содержимого директории
     */
    class ListDirectoryCommand : public BaseCommand {
    public:
        ListDirectoryCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для создания пустого файла
     */
    class CreateFileCommand : public BaseCommand {
    public:
        CreateFileCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для создания директории
     */
    class MakeDirectoryCommand : public BaseCommand {
    public:
        MakeDirectoryCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для удаления директории
     */
    class DeleteDirectoryCommand : public BaseCommand {
    public:
        DeleteDirectoryCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для чтения содержимого файла
     */
    class ReadFileCommand : public BaseCommand {
    public:
        ReadFileCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для удаления файла
     */
    class DeleteFileCommand : public BaseCommand {
    public:
        DeleteFileCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для копирования файлов и директорий
     */
    class CopyCommand : public BaseCommand {
    public:
        CopyCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для перемещения файлов и директорий
     */
    class MoveCommand : public BaseCommand {
    public:
        MoveCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для изменения прав доступа к файлу
     */
    class ChangePermissionsCommand : public BaseCommand {
    public:
        ChangePermissionsCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для изменения владельца файла
     */
    class ChangeOwnerCommand : public BaseCommand {
    public:
        ChangeOwnerCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для отображения статистики файловой системы
     */
    class StatisticsCommand : public BaseCommand {
    public:
        StatisticsCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для поиска файлов по шаблону
     */
    class FindCommand : public BaseCommand {
    public:
        FindCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для создания нового пользователя
     * @note Требует прав администратора
     */
    class CreateUserCommand : public BaseCommand {
    public:
        CreateUserCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для создания новой группы
     * @note Требует прав администратора
     */
    class CreateGroupCommand : public BaseCommand {
    public:
        CreateGroupCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для добавления пользователя в группу
     * @note Требует прав администратора
     */
    class AddUserToGroupCommand : public BaseCommand {
    public:
        AddUserToGroupCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для удаления группы
     * @note Требует прав администратора
     */
    class DeleteGroupCommand : public BaseCommand {
    public:
        DeleteGroupCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для удаления пользователя
     * @note Требует прав администратора
     */
    class DeleteUserCommand : public BaseCommand {
    public:
        DeleteUserCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для редактирования содержимого файла
     */
    class EditFileCommand : public BaseCommand {
    public:
        EditFileCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для создания случайных элементов в файловой системе
     * @note Требует прав администратора
     */
    class CreateRandomElementsCommand : public BaseCommand {
    public:
        CreateRandomElementsCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для сохранения всей файловой системы в файл
     * @note Требует прав администратора
     */
    class SaveProjectCommand : public BaseCommand {
    public:
        SaveProjectCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };

    /**
     * @brief Команда для загрузки файловой системы из файла
     * @note Требует прав администратора
     */
    class LoadProjectCommand : public BaseCommand {
    public:
        LoadProjectCommand();
        CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;
        bool validateArgs(const std::vector<std::string>& args) const override;
    };
}

#endif