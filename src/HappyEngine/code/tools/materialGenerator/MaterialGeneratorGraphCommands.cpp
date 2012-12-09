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
#include "HappyPCH.h" 

#include "MaterialGeneratorGraphCommands.h"

#include "MaterialGeneratorGraph.h"
#include "MaterialGeneratorNode.h"

namespace he {
namespace tools {

//////////////////////////////////////////////////////////////////////////
// Move
//////////////////////////////////////////////////////////////////////////
void MaterialGeneratorGraphMoveCommand::operator()( const CommandType type )
{
    const he::PrimitiveList<MaterialGeneratorNode*>& m_SelectedNodes(m_Parent->m_SelectedNodeList);
    const vec2 move(type == CommandType_Do? m_Move : -m_Move);
    m_SelectedNodes.forEach([&move](MaterialGeneratorNode* const node)
    {
        node->setPosition(node->getPosition() + move);
    });
}

void MaterialGeneratorGraphMoveCommand::beginTransaction()
{
    m_Move.x = 0.0f;
    m_Move.y = 0.0f;
}

void MaterialGeneratorGraphMoveCommand::doMove( const vec2& move )
{
    m_Parent->m_SelectedNodeList.forEach([&move](MaterialGeneratorNode* const node)
    {
        node->setPosition(node->getPosition() + move);
    });
    m_Move += move;
}

void MaterialGeneratorGraphMoveCommand::endTransaction()
{
    m_Parent->m_CommandStack.pushCommand(Command(*this));
}

//////////////////////////////////////////////////////////////////////////
// Edit Selection
//////////////////////////////////////////////////////////////////////////

void MaterialGeneratorGraphEditSelectionCommand::operator()( const CommandType type )
{
    he::PrimitiveList<MaterialGeneratorNode*>& selectedNodes(m_Parent->m_SelectedNodeList);
    const he::PrimitiveList<MaterialGeneratorNode*>& nodes(m_Parent->m_NodeList);
    const he::ObjectList<Guid>& deselectList(type == CommandType_Do? m_DeselectedNodes : m_SelectedNodes);
    const he::ObjectList<Guid>& selectList(type == CommandType_Do? m_SelectedNodes : m_DeselectedNodes);
    deselectList.forEach([&selectedNodes](const Guid& guid)
    {
        size_t index(0);
        if (selectedNodes.find_if([&guid](const MaterialGeneratorNode* const node) -> bool
                                    {
                                        return node->getGuid() == guid;
                                    }, index))
        {
            MaterialGeneratorNode* const selectedNode(selectedNodes[index]);
            selectedNode->setSelected(false);
            selectedNodes.removeAt(index);
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Could not find node for deselecting");
        }
    });
    selectList.forEach([&nodes, &selectedNodes](const Guid& guid)
    {
        size_t index(0);
        if (nodes.find_if([&guid](const MaterialGeneratorNode* const node) -> bool
        {
            return node->getGuid() == guid;
        }, index))
        {
            MaterialGeneratorNode* const selectedNode(nodes[index]);
            selectedNode->setSelected(true);
            selectedNodes.add(selectedNode);
        }
        else
        {
            LOG(LogType_ProgrammerAssert, "Could not find node for selecting");
        }
    });
}

void MaterialGeneratorGraphEditSelectionCommand::doEditSelection( const bool keepSelection, const bool removeSelection, 
    const he::PrimitiveList<MaterialGeneratorNode*>& nodes )
{
    if (keepSelection == false && removeSelection == false)
    {
        deselectAll();
    }
    nodes.forEach([&](MaterialGeneratorNode* node)
    {
        doEditSelection(true, removeSelection, node);
    });
}

void MaterialGeneratorGraphEditSelectionCommand::beginTransaction()
{
    m_SelectedNodes.clear();
    m_DeselectedNodes.clear();
}

void MaterialGeneratorGraphEditSelectionCommand::endTransaction()
{
    m_Parent->m_CommandStack.pushCommand(Command(*this));
}

void MaterialGeneratorGraphEditSelectionCommand::doEditSelection( const bool keepSelection, const bool removeSelection, 
    MaterialGeneratorNode* const node )
{
    bool isSelected(node->isSelected());
    he::PrimitiveList<MaterialGeneratorNode*>& selectedNodes(m_Parent->m_SelectedNodeList);
    if (removeSelection)
    {
        if (isSelected)
        {
            selectedNodes.remove(node);
            node->setSelected(false);
            m_DeselectedNodes.add(node->getGuid());
        }
    }
    else
    {
        if (keepSelection == false && isSelected == false)
        {
            deselectAll();
        }
        if (isSelected == false)
        {
            node->setSelected(true);
            selectedNodes.add(node);
            m_SelectedNodes.add(node->getGuid());
        }
    }
}

void MaterialGeneratorGraphEditSelectionCommand::deselectAll()
{
    he::PrimitiveList<MaterialGeneratorNode*>& selectedNodes(m_Parent->m_SelectedNodeList);
    selectedNodes.forEach([&](MaterialGeneratorNode* node)
    {
        m_DeselectedNodes.add(node->getGuid());
        node->setSelected(false);
    });
    selectedNodes.clear();
}

} } //end namespace
