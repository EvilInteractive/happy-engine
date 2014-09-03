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
//Created: 2014/02/09

#ifndef _HE_ThreadTicket_H_
#define _HE_ThreadTicket_H_
#pragma once

#include "Singleton.h"

namespace he {

enum EThreadTicket
{
    eThreadTicket_Main = 0,
    eThreadTicket_Content = 1,

    eThreadTicket_User = 2 // Keep last
};

struct ThreadTicket
{
    ThreadTicket();
    ~ThreadTicket();
    ThreadID m_Owner;
    const char* m_Name;
    const char* m_File;
    int m_Line;
};

class ThreadTicketManager : public he::Singleton<ThreadTicketManager>
{
public:
    ThreadTicketManager();
    ~ThreadTicketManager();

    void registerTicket(const size_t id, const char* name);
    void claim(const size_t id, const he::ThreadID thread, const char* file, int line);
    void check(const size_t id, const char* file, int line);

private:
    he::ObjectList<ThreadTicket> m_Tickets;
};

#ifdef HE_USE_THREADTICKETS
#define CLAIM_THREAD(id) he::ThreadTicketManager::getInstance()->claim(id, he::Thread::getCurrentThread(), __FILE__, __LINE__)
#define GIVE_THREAD(id, thread) he::ThreadTicketManager::getInstance()->claim(id, thread, __FILE__, __LINE__)
#define GUARD(id) he::ThreadTicketManager::getInstance()->check(id, __FILE__, __LINE__)
#else
#define CLAIM_THREAD(id)
#define GIVE_THREAD(id, thread)
#define GUARD(id)
#endif

} //end namespace

#endif
