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

    if (app.init())
        app.run();

    app.terminate();
    return 0;
}
