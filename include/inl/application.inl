////////////////////////
//
//  Created: Sun Jun 09 2024
//  File: application.inl
//
////////////////////////

#include "application.hpp"

namespace vmce
{
    inline void Application::close()
    {
        _is_running = false;
    }

    inline bool Application::isRunning() const
    {
        return _is_running;
    }
}
