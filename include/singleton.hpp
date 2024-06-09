////////////////////////
//
//  Created: Sun Jun 09 2024
//  File: singleton.hpp
//
////////////////////////

#pragma once

namespace vmce
{
    template<typename T>
    struct Singleton
    {
        static inline T &getInstance();
    };
}

#include "inl/singleton.inl"
