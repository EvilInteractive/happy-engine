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
//Created: 09/12/2012
#include "HappyPCH.h" 

#include "CommandTransaction.h"

namespace he {
namespace tools {

CommandTransaction::CommandTransaction()
    : m_Name("")
{
}

CommandTransaction::~CommandTransaction()
{
}

CommandTransaction::CommandTransaction( const CommandTransaction& other )
    : m_Name(other.m_Name)
    , m_Commands(other.m_Commands.size())
{
    m_Commands.append(other.m_Commands);
}
CommandTransaction& CommandTransaction::operator=( const CommandTransaction& other )
{
    m_Name = other.m_Name;
    m_Commands.clear();
    m_Commands.append(other.m_Commands);
    return *this;
}

void CommandTransaction::begin( const he::String& name )
{
    m_Commands.clear();
    m_Name = name;
}

void CommandTransaction::pushCommand(const Command& command)
{
    m_Commands.add(command);
}

void CommandTransaction::undoTransaction()
{
    m_Commands.rForEach([](Command& command)
    {
        command.undoCommand();
    });
}

void CommandTransaction::redoTransaction()
{
    m_Commands.forEach([](Command& command)
    {
        command.doCommand();
    });
}

} } //end namespace
