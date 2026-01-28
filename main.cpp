#include "View/realisation/view.h"
#include "Controller/realisation/controller.h"
#include "Loader/realisation/loader.h"
#include <iostream>
#include <memory>

int main() {
    try {
        std::unique_ptr<ILoader> loader = std::make_unique<FSLoader>();
        auto controller = std::make_unique<Controller>(std::move(loader));
        controller->run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}