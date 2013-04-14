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
//Created: 9/12/2012

#ifndef _HE_CommandStack_H_
#define _HE_CommandStack_H_
#pragma once

#include "CommandTransaction.h"

namespace he {
namespace tools {

class Command;

class CommandStack
{
public:
    CommandStack();
    virtual ~CommandStack();

    void beginTransaction(const he::String& name);
    void endTransaction(const he::String& rename = "");

    void pushCommand(const Command& command);
    void undo();
    void redo();

    void clear();

    bool canUndo() const;
    bool canRedo() const;

    const he::ObjectList<CommandTransaction>& getTransactions() const { return m_Transactions; }
    size_t getUndoIndex() const { return m_UndoIndex; }

private:
    bool m_OpenTransaction;
    CommandTransaction m_CurrentTransaction;
    he::ObjectList<CommandTransaction> m_Transactions;
    size_t m_UndoIndex;

    //Disable default copy constructor and default assignment operator
    CommandStack(const CommandStack&);
    CommandStack& operator=(const CommandStack&);
};

} } //end namespace

#endif
