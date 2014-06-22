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

#ifndef _HE_CommandTransaction_H_
#define _HE_CommandTransaction_H_
#pragma once

#include "Command.h"

namespace he {
namespace tools {

class CommandTransaction
{
public:
    CommandTransaction();
    ~CommandTransaction();
    CommandTransaction(const CommandTransaction& other);
    CommandTransaction& operator=(const CommandTransaction& other);

    void begin(const he::String& name);
    void pushCommand(const Command& command);
    void undoTransaction();
    void redoTransaction();

    size_t getSize() const { return m_Commands.size(); }

    void setName(const he::String& name) { m_Name = name; }
    const he::String& getName() const { return m_Name; }

private:
    he::String m_Name;
    he::ObjectList<Command> m_Commands;
};

} } //end namespace

#endif
