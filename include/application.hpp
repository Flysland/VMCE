////////////////////////
//
//  Created: Sun Jun 02 2024
//  File: application.hpp
//
////////////////////////

#pragma once

#include <singleton.hpp>

int main();

namespace vmce
{
    class Application : public Singleton<Application>
    {
        friend int ::main();

        public:
            inline void close();
            inline bool isRunning() const;

        private:
            bool _is_running;

            bool init();
            void run();
            void terminate();
    };
}

#include "inl/application.inl"
