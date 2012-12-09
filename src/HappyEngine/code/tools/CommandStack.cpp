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
#include "HappyPCH.h" 

#include "CommandStack.h"

namespace he {
namespace tools {

CommandStack::CommandStack(): m_UndoIndex(0)
{
}


CommandStack::~CommandStack()
{
}

void CommandStack::pushCommand( const Command& command )
{
    m_CommandStack.resize(m_UndoIndex);
    m_CommandStack.add(command);
    ++m_UndoIndex;
}

void CommandStack::undo()
{
    if (canUndo())
    {
        m_CommandStack[--m_UndoIndex].undoCommand();
    }
}

void CommandStack::redo()
{
    if (canRedo())
    {
        m_CommandStack[m_UndoIndex++].doCommand();
    }
}

bool CommandStack::canUndo() const
{
    return m_UndoIndex > 0;
}

bool CommandStack::canRedo() const
{
    return m_CommandStack.size() > m_UndoIndex;
}

} } //end namespace
