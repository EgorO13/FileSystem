#ifndef LAB3_COMMAND_REPOSITORY_H
#define LAB3_COMMAND_REPOSITORY_H

#include "../interface/i_command_repository.h"
#include <map>
#include <memory>

/**
 * @brief Реализация репозитория команд, хранящего команды для файловой системы.
 *
 * Класс предоставляет методы для сохранения, удаления, получения и проверки существования команд.
 * Также позволяет создавать и управлять составными командами.
 */
class CommandRepository : public ICommandRepository {
private:
    std::map<std::string, std::unique_ptr<ICommand>> commandStorage; ///< Хранилище команд (имя -> команда)
    void initializeDefaultCommands(); ///< Инициализация стандартных команд при создании репозитория

public:
    /**
     * @brief Конструктор по умолчанию
     * Создает репозиторий и инициализирует его стандартными командами.
     */
    CommandRepository();

    /**
     * @brief Сохраняет команду в репозитории
     * @param name Имя команды
     * @param command Указатель на команду
     * @return true, если команда успешно сохранена, false в противном случае
     */
    bool saveCommand(const std::string& name, std::unique_ptr<ICommand> command) override;

    /**
     * @brief Удаляет команду из репозитория
     * @param name Имя команды для удаления
     * @return true, если команда успешно удалена, false если команда не найдена
     */
    bool deleteCommand(const std::string& name) override;

    /**
     * @brief Получает команду по имени
     * @param name Имя команды
     * @return Указатель на команду или nullptr, если команда не найдена
     * @throw std::runtime_error Команда не найдена
     */
    ICommand* getCommand(const std::string& name) const override;

    /**
     * @brief Проверяет существование команды в репозитории
     * @param name Имя команды
     * @return true, если команда существует, false в противном случае
     */
    bool commandExists(const std::string& name) const override;

    /**
     * @brief Получает список имен всех команд в репозитории
     * @return Вектор строк с именами команд
     */
    std::vector<std::string> getCommandNames() const override;

    /**
     * @brief Создает новую составную команду и сохраняет ее в репозитории
     * @param name Имя новой составной команды
     * @param description Описание команды
     * @return true, если команда успешно создана, false если команда с таким именем уже существует
     */
    bool createCompositeCommand(const std::string& name, const std::string& description) override;

    /**
     * @brief Получает все команды из репозитория
     * @return Карта всех команд (имя -> команда)
     */
    std::map<std::string, ICommand*> getAllCommands() const override;
};

#endif