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
//Created: 2013/03/09
#include "HappyPCH.h" 

#ifdef HE_WINDOWS
#include "Mutex.h"
#include "Thread.h"

namespace he {

Mutex::Mutex()
{
    InitializeCriticalSection(&m_Internal);
}


Mutex::~Mutex()
{
    DeleteCriticalSection(&m_Internal);
}

#ifdef HE_DEBUG
void Mutex::lock( const char* file, int line )
{
    EnterCriticalSection(&m_Internal);
    m_File = file;
    m_Line = line;
    m_Thread = he::Thread::getCurrentThread();
}
bool Mutex::tryLock( const char* file, int line )
{
    if (TryEnterCriticalSection(&m_Internal) == TRUE)
    {
        m_File = file;
        m_Line = line;
        m_Thread = he::Thread::getCurrentThread();
        return true;
    }
    else
    {
        return false;
    }
}
#else
void Mutex::lock()
{
    EnterCriticalSection(&m_Internal);
}
bool Mutex::tryLock()
{
    return TryEnterCriticalSection(&m_Internal) == TRUE;
}
#endif

void Mutex::unlock()
{
    LeaveCriticalSection(&m_Internal);
}

} //end namespace

#endif