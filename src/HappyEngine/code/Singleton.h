//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

// Always use this macro when defining singletons in the HappyEngine
// It makes sure the singleton is exported in the shared lib! This so we can share the singletons in the using app

#define SINGLETON(className, ...) \
    className; \
    HAPPY_TEMPLATE_ENTRY template class HAPPY_ENTRY Singleton<className>; \
    class className : public Singleton<className>,##__VA_ARGS__ 

template<typename T>
class Singleton
{
public:
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
        s_Instance = nullptr;
    }
    static T* getInstance()
    {
        return s_Instance;
    }


protected:
    Singleton() { HE_ASSERT(s_Instance == nullptr, "Singleton is created more than once!"); }

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
