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

#ifdef HE_MAC
#include "Mutex.h"
#include "Thread.h"

namespace he {

const char* mutexErrorCodeToString(const int error)
{
    const char* result("")';'
    switch (error)
    {
    case EINVAL: result = "EINVAL, The value specified by mutex does not refer to an initialized mutex object."; break;
    case EBUSY: result = "EBUSY, The mutex could not be acquired because it was already locked."; break;
    case EAGAIN: result = "EAGAIN, The mutex could not be acquired because the maximum number of recursive locks for mutex has been exceeded."; break;
    case EDEADLK: result = "EDEADLK, The current thread already owns the mutex."; break;
    case EPERM: result = "EPERM, The current thread does not own the mutex."; break;
    default: result = "Unknown"; break;
    }
}

Mutex::Mutex()
{
    pthread_mutex_init(&m_Internal, 0);
#ifdef _DEBUG
    pthread_mutexattr_settype(&m_Internal, PTHREAD_MUTEX_ERRORCHECK);
#else
    pthread_mutexattr_settype(&m_Internal, PTHREAD_MUTEX_NORMAL);
#endif
}


Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_Internal);
}

#ifdef _DEBUG
void Mutex::lock( const char* file, int line )
{
    const int result(pthread_mutex_lock(&m_Internal));
    if (result == 0)
    {
        m_File = file;
        m_Line = line;
        m_Thread = he::Thread::getCurrentThread();
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Failed to lock thread! error code: %d: \n%s", result, mutexErrorCodeToString(result));
    }
}
bool Mutex::tryLock( const char* file, int line )
{
    if (pthread_mutex_trylock(&m_Internal) == 0)
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
    pthread_mutex_lock(&m_Internal)
}
bool Mutex::tryLock()
{
    return pthread_mutex_trylock(&m_Internal) == 0;
}
#endif

void Mutex::unlock()
{
#ifdef _DEBUG
    const int result(pthread_mutex_unlock(&m_Internal));
    HE_ASSERT(result == 0, "Failed to lock thread! error code: %d: \n%s", result, mutexErrorCodeToString(result));
#else
    pthread_mutex_unlock(&m_Internal);
#endif
}

} //end namespace

#endif
