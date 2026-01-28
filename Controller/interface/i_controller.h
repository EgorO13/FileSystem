#ifndef LAB3_I_CONTROLLER_H
#define LAB3_I_CONTROLLER_H

#include <string>
#include <vector>

/**
 * @brief Интерфейс контроллера.
 *
 * Для управления потоком выполнения приложения,
 * обработки команд пользователя и координации работы компонентов.
 */
class IController {
private:
    /**
     * @brief Разобрать введенную строку на команду и аргументы
     * @param input Входная строка
     * @param command Извлеченная команда
     * @param args Вектор аргументов
     */
    virtual void parseCommand(const std::string& input, std::string& command, std::vector<std::string>& args) = 0;

    /**
     * @brief Убрать пробелы в начале и конце строки
     * @param str Обрабатываемая строка
     */
    virtual void trim(std::string& str) = 0;

    /**
     * @brief Разделить строку на аргументы с учетом кавычек
     * @param input Входная строка
     * @param args Вектор для аргументов
     * @return true если разбор успешен, иначе false
     */
    virtual bool splitArguments(const std::string& input, std::vector<std::string>& args) = 0;

    /**
     * @brief Инициализировать встроенные команды контроллера
     */
    virtual void initializeControllerCommands() = 0;

    /**
     * @brief Команда вывода списка доступных команд
     * @param args Аргументы команды
     */
    virtual void cmdHelp(const std::vector<std::string>& args) = 0;

    /**
     * @brief Команда выхода из приложения
     * @param args Аргументы команды
     */
    virtual void cmdExit(const std::vector<std::string>& args) = 0;

    /**
     * @brief Команда входа в систему
     * @param args Аргументы команды (имя пользователя)
     */
    virtual void cmdLogin(const std::vector<std::string>& args) = 0;

    /**
     * @brief Команда выхода из системы
     * @param args Аргументы команды
     */
    virtual void cmdLogout(const std::vector<std::string>& args) = 0;

    /**
     * @brief Команда отображения текущего пользователя
     * @param args Аргументы команды
     */
    virtual void cmdWhoami(const std::vector<std::string>& args) = 0;

    /**
     * @brief Команда отображения текущей рабочей директории
     * @param args Аргументы команды
     */
    virtual void cmdPwd(const std::vector<std::string>& args) = 0;

    /**
     * @brief Команда вывода справки по конкретной команде
     * @param args Аргументы команды (имя команды для справки)
     */
    virtual void cmdMan(const std::vector<std::string>& args) = 0;

    /**
     * @brief Команда выполнения составной команды
     * @param args Аргументы команды
     */
    virtual void cmdComposite(const std::vector<std::string>& args) = 0;

    /**
     * @brief Выполнить команду файловой системы
     * @param command Имя команды
     * @param args Аргументы команды
     */
    virtual void executeFSCommand(const std::string& command, const std::vector<std::string>& args) = 0;

public:
    virtual ~IController() = default;

    /**
     * @brief Запустить основной цикл контроллера
     */
    virtual void run() = 0;

    /**
     * @brief Остановить контроллер
     */
    virtual void stop() = 0;

    /**
     * @brief Проверить, активен ли контроллер
     * @return true если контроллер активен, иначе false
     */
    virtual bool isRunning() const = 0;

    /**
     * @brief Получить список доступных команд
     * @return Вектор имен доступных команд
     */
    virtual std::vector<std::string> getAvailableCommands() const = 0;

    /**
     * @brief Показать приветственное сообщение
     */
    virtual void showWelcomeMessage() = 0;
};

#endif