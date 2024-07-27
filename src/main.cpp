////////////////////////
//
//  Created: Sun Jun 09 2024
//  File: main.cpp
//
////////////////////////

#include "application.hpp"

int main()
{
    vmce::Application &app = vmce::Application::getInstance();

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
