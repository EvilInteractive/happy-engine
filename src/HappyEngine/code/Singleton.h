//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author:  Bastian Damman
//Created: 31/05/2012

#ifndef _HE_Singleton_H_
#define _HE_Singleton_H_
#pragma once

namespace he {

template<typename T>
class Singleton
{
public:
    virtual ~Singleton() {}

    //////////////////////////////////////////////////////////////////////////
    ///  Singleton
    //////////////////////////////////////////////////////////////////////////
    static void sdmInit()
    {
        HE_ASSERT(s_Instance == nullptr, "initing an already inited singleton");
        s_Instance = NEW T();
    }
    static void sdmDestroy()
    {
        delete s_Instance;
    }
    static T* getInstance()
    {
        HE_ASSERT(s_Instance != nullptr, "singleton has not been initialized!");
        return s_Instance;
    }


protected:
    Singleton() { HE_ASSERT(s_Instance == nullptr, "Singleton is create more than once!"); }

private:
    static T* s_Instance;

    //Disable default copy constructor and default assignment operator
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
};

template<typename T>
T* Singleton<T>::s_Instance = nullptr;

} //end namespace

#endif
