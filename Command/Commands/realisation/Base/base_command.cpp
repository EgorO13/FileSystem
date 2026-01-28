#include "Command/Commands/realisation/Base/base_command.h"
#include <stdexcept>
#include <string>


BaseCommand::BaseCommand(const std::string &n, const std::string &description, const std::string &usage, bool adminOnly)
    : name(n), description(description), usage(usage), onlyForAdmin(adminOnly) {
    if (n.empty() || n.find('/') != std::string::npos || n.find('.') != std::string::npos) {
        throw std::invalid_argument("Command name cannot be empty");
    }
}

