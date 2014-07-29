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
#include "HappyPCH.h" 


#ifdef HE_MAC
#include "Thread.h"

namespace he {
    
Thread::Thread()
    : m_ID(0)
    , m_IsRunning(false)
{
}

void* threadProc(void* param)
{
    Thread* thread(static_cast<Thread*>(param));
    thread->m_IsRunning = true;
    thread->m_Worker();
    thread->m_IsRunning = false;
    return nullptr;
}

void Thread::startThread(const boost::function0<void>& threadWorker, const char* /*name*/)
{
    m_Worker = threadWorker;
    const int fail(pthread_create(&m_ID, 0, threadProc, this));
    HE_ASSERT(!fail, "Thread failed to create!"); fail;
}

Thread::~Thread()
{

}

void Thread::join()
{
    pthread_join(m_ID, NULL);
}

he::ThreadID Thread::getCurrentThread()
{
    return pthread_self();
}

void Thread::sleep( const size_t millisec )
{
    usleep(millisec * 1000);
}

} //end namespace

#endif
