#ifndef LAB3_CONSOLE_VIEW_H
#define LAB3_CONSOLE_VIEW_H

#include "../interface/i_view.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Консольная реализация представления (View).
 *
 * Обеспечивает взаимодействие с пользователем через стандартный
 * ввод/вывод консоли, реализуя интерфейс IView.
 */
class ConsoleView : public IView {
public:
    /**
     * @brief Отобразить сообщение пользователю в консоли
     * @param message Текст сообщения
     */
    void displayMessage(const std::string& message) override;

    /**
     * @brief Отобразить несколько сообщений пользователю в консоли
     * @param messages Вектор текстовых сообщений
     */
    void displayMessage(const std::vector<std::string>& messages) override;

    /**
     * @brief Отобразить сообщение об ошибке в консоли
     * @param error Текст ошибки
     */
    void displayError(const std::string& error) override;

    /**
     * @brief Получить ввод от пользователя из консоли
     * @param prompt Приглашение для ввода (по умолчанию "> ")
     * @return Введенная пользователем строка
     */
    std::string getInput(const std::string& prompt = "> ") override;

    /**
     * @brief Показать приветственное сообщение в консоли
     */
    void showWelcome() override;

    /**
     * @brief Показать прощальное сообщение в консоли
     */
    void showGoodbye() override;
};

#endif