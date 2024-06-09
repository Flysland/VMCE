////////////////////////
//
//  Created: Sun Jun 09 2024
//  File: singleton.inl
//
////////////////////////

#include "singleton.hpp"

namespace vmce
{
    template<typename T>
    inline T &Singleton<T>::getInstance()
    {
        static T instance;
        return instance;
    }
}
