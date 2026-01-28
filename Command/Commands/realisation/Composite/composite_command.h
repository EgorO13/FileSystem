#ifndef LAB3_COMPOSITE_COMMAND_H
#define LAB3_COMPOSITE_COMMAND_H

#include "Command/Commands/interface/i_compositable.h"
#include "Command/Commands/realisation/Base/base_command.h"
#include "base.h"
#include <vector>
#include <string>
#include <memory>

/**
 * @brief Класс составной команды, объединяющей несколько команд в одну последовательность.
 *
 * Составная команда позволяет группировать несколько базовых или других составных команд в единую.
 */
class CompositeCommand : public BaseCommand, public ICompositable {
private:
    std::vector<CommandEntry> commands; ///< Вектор подкоманд, входящих в составную команду

public:
    /**
     * @brief Конструктор составной команды
     * @param name Имя составной команды
     * @param description Описание составной команды
     */
    CompositeCommand(const std::string& name, const std::string& description);

    /**
     * @brief Выполняет составную команду
     * @param args Аргументы команды
     * @param fs Ссылка на файловую систему
     * @return Результат выполнения (всегда ошибка, так как составные команды выполняются через CommandService)
     */
    CommandResult execute(const std::vector<std::string>& args, IFileSystem& fs) override;

    /**
     * @brief Проверяет корректность аргументов команды
     * @param args Аргументы для проверки
     * @return Всегда true, так как валидация аргументов выполняется для каждой подкоманды отдельно
     */
    bool validateArgs(const std::vector<std::string>& args) const override;

    /**
     * @brief Добавляет подкоманду в составную команду
     * @param commandName Имя добавляемой команды
     * @param fixedArgs Фиксированные аргументы для команды
     * @param dynamicIndices Индексы динамических аргументов из входных параметров составной команды
     * @throws std::invalid_argument Если имя команды пустое или индексы отрицательные
     */
    void addCommand(const std::string& commandName, const std::vector<std::string>& fixedArgs = {}, const std::vector<int>& dynamicIndices = {});

    /**
     * @brief Удаляет подкоманду по индексу
     * @param index Индекс удаляемой подкоманды
     * @return true, если подкоманда успешно удалена, false если индекс выходит за границы
     */
    bool removeCommand(size_t index);

    /**
     * @brief Очищает все подкоманды
     */
    void clearCommands();

    /**
     * @brief Получает список всех подкоманд
     * @return Константная ссылка на вектор подкоманд
     */
    const std::vector<CommandEntry>& getSubCommands() const { return commands; }

    /**
     * @brief Получает количество подкоманд
     * @return Количество подкоманд в составной команде
     */
    size_t getCommandCount() const { return commands.size(); }
};

#endif