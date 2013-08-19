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

#ifndef _HE_Thread_H_
#define _HE_Thread_H_
#pragma once

#include "ThreadTypes.h"

namespace he {

class HAPPY_ENTRY Thread
{
    friend ThreadProc;
public:
    Thread();
    ~Thread();

    void startThread(const boost::function0<void>& threadWorker, const char* name);
    void join();
    bool isRunning() const { return m_IsRunning; }

    static ThreadID getCurrentThread();
    static void sleep(const size_t millisec);

private:

    ThreadID m_ID;
    details::ThreadHandle m_Handle;
    bool m_IsRunning;
    boost::function0<void> m_Worker;

    //Disable default copy constructor and default assignment operator
    Thread(const Thread&);
    Thread& operator=(const Thread&);
};

} //end namespace

#endif
