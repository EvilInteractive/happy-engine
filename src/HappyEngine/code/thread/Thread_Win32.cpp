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

#ifdef HE_WINDOWS
#include "Thread.h"

namespace he {

Thread::Thread()
    : m_ID(0)
    , m_IsRunning(false)
    , m_Handle(NULL)
{
}

DWORD WINAPI threadProc(_In_ LPVOID param)
{
    Thread* thread(static_cast<Thread*>(param));
    thread->m_IsRunning = true;
    thread->m_Worker();
    thread->m_IsRunning = false;
    return 0;
}

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void Thread::startThread(const boost::function0<void>& threadWorker, const char* name)
{
    HE_ASSERT(m_Handle == NULL, "Thread '%s' already started!", name);
    m_Worker = threadWorker;
    m_Handle = ::CreateThread(0, 0, he::threadProc, this, 0, &m_ID);
    HE_ASSERT(m_Handle != NULL, "Thread '%s' failed to create!", name);

#ifdef HE_DEBUG
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = m_ID;
    info.dwFlags = 0;

    __try
    {
        ::RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(ULONG_PTR),  reinterpret_cast<ULONG_PTR*>(&info) );
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
    }
#else
    name;
#endif
}

Thread::~Thread()
{
    if (m_Handle != NULL)
    {
        ::CloseHandle(m_Handle);
    }
}

void Thread::join()
{
    if (m_Handle != NULL)
    {
        ::WaitForSingleObject(m_Handle, INFINITE);
        ::CloseHandle(m_Handle);
        m_Handle = NULL;
    }
}

he::ThreadID Thread::getCurrentThread()
{
    return ::GetCurrentThreadId();
}

void Thread::sleep( const size_t millisec )
{
    Sleep(millisec);
}

} //end namespace

#else
#error Only Windows code here
#endif
