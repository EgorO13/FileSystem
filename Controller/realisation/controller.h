#ifndef LAB3_CONTROLLER_H
#define LAB3_CONTROLLER_H

#include "Controller/interface/i_controller.h"
#include "View/interface/i_view.h"
#include "../../FileSystem/realisation/file_system.h"
#include "../../Command/CommandService/interface/i_command_service.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <map>

/**
 * @brief Контроллер в архитектуре MVC.
 *
 * Управляет потоком выполнения приложения, обрабатывает команды пользователя,
 * координирует взаимодействие между представлением, файловой системой и сервисом команд.
 */
class Controller : public IController {
private:
    bool isRun;                                                 ///< Флаг активности контроллера
    IView& view;                                                ///< Ссылка на представление
    ICommandService& commandService;                            ///< Ссылка на сервис команд
    std::unique_ptr<IFileSystem> fileSystem;                    ///< Уникальный указатель на файловую систему
    std::map<std::string, std::function<void(const std::vector<std::string>&)>> controllerCommands; ///< Карта команд контроллера

    /**
     * @brief Разобрать введенную строку на команду и аргументы
     * @param input Входная строка
     * @param command Извлеченная команда
     * @param args Вектор аргументов
     */
    void parseCommand(const std::string& input, std::string& command, std::vector<std::string>& args) override;

    /**
     * @brief Убрать пробелы в начале и конце строки
     * @param str Обрабатываемая строка
     */
    void trim(std::string& str) override;

    /**
     * @brief Разделить строку на аргументы с учетом кавычек
     * @param input Входная строка
     * @param args Вектор для аргументов
     * @return true если разбор успешен, иначе false
     */
    bool splitArguments(const std::string& input, std::vector<std::string>& args) override;

    /**
     * @brief Инициализировать встроенные команды контроллера
     */
    void initializeControllerCommands() override;

    /**
     * @brief Команда вывода списка доступных команд
     * @param args Аргументы команды
     */
    void cmdHelp(const std::vector<std::string>& args) override;

    /**
     * @brief Команда выхода из приложения
     * @param args Аргументы команды
     */
    void cmdExit(const std::vector<std::string>& args) override;

    /**
     * @brief Команда входа в систему
     * @param args Аргументы команды (имя пользователя)
     */
    void cmdLogin(const std::vector<std::string>& args) override;

    /**
     * @brief Команда выхода из системы
     * @param args Аргументы команды
     */
    void cmdLogout(const std::vector<std::string>& args) override;

    /**
     * @brief Команда отображения текущего пользователя
     * @param args Аргументы команды
     */
    void cmdWhoami(const std::vector<std::string>& args) override;

    /**
     * @brief Команда отображения текущей рабочей директории
     * @param args Аргументы команды
     */
    void cmdPwd(const std::vector<std::string>& args) override;

    /**
     * @brief Команда вывода справки по конкретной команде
     * @param args Аргументы команды (имя команды для справки)
     */
    void cmdMan(const std::vector<std::string>& args) override;

    /**
     * @brief Команда выполнения составной команды
     * @param args Аргументы команды
     */
    void cmdComposite(const std::vector<std::string>& args) override;

    /**
     * @brief Выполнить команду файловой системы
     * @param command Имя команды
     * @param args Аргументы команды
     */
    void executeFSCommand(const std::string& command, const std::vector<std::string>& args) override;

public:
    /**
     * @brief Конструктор контроллера
     * @param loader Загрузчик зависимостей
     */
    Controller(std::unique_ptr<ILoader> loader);

    /**
     * @brief Запустить основной цикл контроллера
     */
    void run() override;

    /**
     * @brief Остановить контроллер
     */
    void stop() override { isRun = false; }

    /**
     * @brief Проверить, активен ли контроллер
     * @return true если контроллер активен, иначе false
     */
    bool isRunning() const override{ return isRun; }

    /**
     * @brief Получить список доступных команд
     * @return Вектор имен доступных команд
     */
    std::vector<std::string> getAvailableCommands() const override;

    /**
     * @brief Показать приветственное сообщение
     */
    void showWelcomeMessage() override;
};

#endif