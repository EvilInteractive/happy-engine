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
//Created: 09/12/2012

#ifndef _HE_MaterialGeneratorGraphCommands_H_
#define _HE_MaterialGeneratorGraphCommands_H_
#pragma once

#include "Command.h"

namespace he {
namespace tools {

class MaterialGeneratorGraph;
class MaterialGeneratorNode;

class MaterialGeneratorGraphMoveCommand
{ // Move is always on current selection
public:
    MaterialGeneratorGraphMoveCommand(MaterialGeneratorGraph* const parent)
        : m_Parent(parent)
        , m_Move(0, 0) 
    {}
    // Copy == ok

    void operator()(const CommandType type);

    void beginTransaction();
    void endTransaction();
    void doMove(const vec2& move);

private:
    MaterialGeneratorGraph* m_Parent;
    vec2 m_Move;
};

class MaterialGeneratorGraphEditSelectionCommand
{
public:
    explicit MaterialGeneratorGraphEditSelectionCommand(MaterialGeneratorGraph* const parent)
        : m_Parent(parent)
    {
    }
    MaterialGeneratorGraphEditSelectionCommand(const MaterialGeneratorGraphEditSelectionCommand& other)
        : m_Parent(other.m_Parent)
        , m_SelectedNodes(other.m_SelectedNodes.size())
        , m_DeselectedNodes(other.m_DeselectedNodes.size())
    {
        m_SelectedNodes.append(other.m_SelectedNodes);
        m_DeselectedNodes.append(other.m_DeselectedNodes);
    }
    MaterialGeneratorGraphEditSelectionCommand& operator=(const MaterialGeneratorGraphEditSelectionCommand& other)
    {
        m_Parent = other.m_Parent;
        m_SelectedNodes.clear();
        m_DeselectedNodes.clear();
        m_SelectedNodes.append(other.m_SelectedNodes);
        m_DeselectedNodes.append(other.m_DeselectedNodes);
        return *this;
    }

    void operator()(const CommandType type);

    void beginTransaction();
    void endTransaction();
    void doEditSelection(const bool keepSelection, const bool removeSelection, const he::PrimitiveList<MaterialGeneratorNode*>& nodes);
    void doEditSelection(const bool keepSelection, const bool removeSelection, MaterialGeneratorNode* const node);
    void deselectAll();

private:
    MaterialGeneratorGraph* m_Parent;
    he::ObjectList<Guid> m_SelectedNodes;
    he::ObjectList<Guid> m_DeselectedNodes;
};

} } //end namespace

#endif
