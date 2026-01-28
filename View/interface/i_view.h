#ifndef LAB3_I_VIEW_H
#define LAB3_I_VIEW_H

#include <string>
#include <vector>

/**
 * @brief Интерфейс представления (View) в архитектуре MVC.
 *
 * Для отображения информации пользователю
 * и получения ввода от пользователя в различных интерфейсах.
 */
class IView {
public:
    virtual ~IView() = default;

    /**
     * @brief Отобразить сообщение пользователю
     * @param message Текст сообщения
     */
    virtual void displayMessage(const std::string& message) = 0;

    /**
     * @brief Отобразить несколько сообщений пользователю
     * @param messages Вектор текстовых сообщений
     */
    virtual void displayMessage(const std::vector<std::string>& messages) = 0;

    /**
     * @brief Отобразить сообщение об ошибке
     * @param error Текст ошибки
     */
    virtual void displayError(const std::string& error) = 0;

    /**
     * @brief Получить ввод от пользователя
     * @param prompt Приглашение для ввода (по умолчанию "> ")
     * @return Введенная пользователем строка
     */
    virtual std::string getInput(const std::string& prompt = "> ") = 0;

    /**
     * @brief Показать приветственное сообщение
     */
    virtual void showWelcome() = 0;

    /**
     * @brief Показать прощальное сообщение
     */
    virtual void showGoodbye() = 0;
};

#endif