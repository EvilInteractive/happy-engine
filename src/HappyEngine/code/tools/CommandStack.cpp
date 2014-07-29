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
//Created: 9/12/2012
#include "HappyPCH.h" 

#include "CommandStack.h"

namespace he {
namespace tools {

CommandStack::CommandStack(): m_UndoIndex(0), m_OpenTransaction(false)
{
}


CommandStack::~CommandStack()
{
}

void CommandStack::beginTransaction(const he::String& name)
{
    HE_IF_ASSERT(m_OpenTransaction == false, "There is still an open transaction: %s\nwhen starting a new one: %s!",  m_Transactions[m_UndoIndex].getName().c_str(), name.c_str())
    {
        m_OpenTransaction = true;
        m_CurrentTransaction.begin(name);
    }
}

void CommandStack::endTransaction(const he::String& rename)
{
    HE_IF_ASSERT(m_OpenTransaction == true, "There is no open transaction!")
    {
        if (m_CurrentTransaction.getSize() > 0)
        {
            if (rename.empty() == false)
                m_CurrentTransaction.setName(rename);
            m_Transactions.resize(m_UndoIndex);
            m_Transactions.add(m_CurrentTransaction);
            ++m_UndoIndex;
        }
        m_OpenTransaction = false;
    }
}

void CommandStack::pushCommand( const Command& command )
{
    HE_IF_ASSERT(m_OpenTransaction == true, "There is no open transaction!")
    {
        m_CurrentTransaction.pushCommand(command);
    }
}

void CommandStack::undo()
{
    HE_IF_ASSERT(m_OpenTransaction == false, "Cannot undo when there is an open transaction: %s!",  m_Transactions[m_UndoIndex].getName().c_str())
    {
        if (canUndo())
        {
            m_Transactions[--m_UndoIndex].undoTransaction();
        }
    }
}

void CommandStack::redo()
{
    HE_IF_ASSERT(m_OpenTransaction == false, "Cannot redo when there is an open transaction: %s!",  m_Transactions[m_UndoIndex].getName().c_str())
    {
        if (canRedo())
        {
            m_Transactions[m_UndoIndex++].redoTransaction();
        }
    }
}

bool CommandStack::canUndo() const
{
    return m_UndoIndex > 0;
}

bool CommandStack::canRedo() const
{
    return m_Transactions.size() > m_UndoIndex;
}

void CommandStack::clear()
{
    m_Transactions.clear();
    m_UndoIndex = 0;
}


} } //end namespace
