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
//Created: 2013/03/09

#ifndef _HE_Mutex_H_
#define _HE_Mutex_H_
#pragma once

#include "ThreadTypes.h"

namespace he {

class HAPPY_ENTRY Mutex
{
public:
    Mutex();
    ~Mutex();

#ifdef HE_DEBUG
    void lock(const char* file, int line);
    bool tryLock(const char* file, int line);
#else
    void lock();
    bool tryLock();
#endif
    void unlock();

private:
#ifdef HE_WINDOWS
    CRITICAL_SECTION m_Internal;
#else
    pthread_mutex_t m_Internal;
#endif

#ifdef HE_DEBUG
    const char* m_File;
    int m_Line;
    ThreadID m_Thread;
#endif

    //Disable default copy constructor and default assignment operator
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);
};

} //end namespace

#endif
