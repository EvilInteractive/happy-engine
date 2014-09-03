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
#include "HappyPCH.h"
#include "ThreadTicket.h"

namespace he {

ThreadTicket::ThreadTicket()
    : m_Owner(0)
    , m_Name("")
    , m_File("NotClaimed")
    , m_Line(0)
{

}

ThreadTicket::~ThreadTicket()
{

}

ThreadTicketManager::ThreadTicketManager()
{

}

ThreadTicketManager::~ThreadTicketManager()
{

}

void ThreadTicketManager::registerTicket(const size_t id, const char* name)
{
    if (m_Tickets.size() <= id)
    {
        m_Tickets.resize(id + 1);
    }
    ThreadTicket& ticket(m_Tickets[id]);
    ticket.m_Name = name;
}

void ThreadTicketManager::claim(const size_t id, const he::ThreadID thread, const char* file, int line)
{
    ThreadTicket& ticket(m_Tickets[id]);
    ticket.m_Owner = thread;
    ticket.m_File = file;
    ticket.m_Line = line;
}

void ThreadTicketManager::check(const size_t id, const char* file, int line)
{
    HE_IF_ASSERT(m_Tickets.size() > id, "Ticket with id %u was not registered!", id)
    {
        const ThreadTicket& ticket(m_Tickets[id]);
        HE_ASSERT(ticket.m_Owner == he::Thread::getCurrentThread(),
            "Thread '%s' access violation @ %s(%d)!\nThread is claimed by thread with ID: %u, calling thread is %u.\nLast claim was by %s(%d)",
                  ticket.m_Name, file, line, ticket.m_Owner, he::Thread::getCurrentThread(), ticket.m_File, ticket.m_Line);
    }
}

} //end namespace

