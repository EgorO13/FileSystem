#include "view.h"
#include <iostream>
#include <vector>
#include <string>


void ConsoleView::displayMessage(const std::string &message) {
    std::cout << message << std::endl;
}

void ConsoleView::displayMessage(const std::vector<std::string> &messages) {
    for (const auto &msg: messages) {
        std::cout << msg << std::endl;
    }
}

void ConsoleView::displayError(const std::string &error) {
    std::cout << "Error: " << error << std::endl;
}

std::string ConsoleView::getInput(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void ConsoleView::showWelcome() {
    std::cout << "=== File System Management System ===" << std::endl;
    std::cout << "Type 'help' for commands, 'exit' to quit" << std::endl;
}

void ConsoleView::showGoodbye() {
    std::cout << "Goodbye!" << std::endl;
}
