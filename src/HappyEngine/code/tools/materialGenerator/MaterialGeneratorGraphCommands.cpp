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

void MaterialGeneratorGraphMoveCommand::beginCommand()
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

void MaterialGeneratorGraphMoveCommand::endCommand()
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

void MaterialGeneratorGraphEditSelectionCommand::beginCommand()
{
    m_SelectedNodes.clear();
    m_DeselectedNodes.clear();
}

void MaterialGeneratorGraphEditSelectionCommand::endCommand()
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


//////////////////////////////////////////////////////////////////////////
// Connect Node
//////////////////////////////////////////////////////////////////////////
void MaterialGeneratorGraphNodeConnectCommand::operator()( const CommandType type )
{
    MaterialGeneratorNode* nodeStart(m_Parent->getNode(m_NodeStart));
    MaterialGeneratorNode* nodeEnd(m_Parent->getNode(m_NodeEnd));
    HE_IF_ASSERT(nodeStart != nullptr && nodeEnd != nullptr, "Node start or end could not be found when connecting nodes")
    {
        if (type == CommandType_Do)
        {
            MaterialGeneratorError error;
            if (m_StartAtOutput == true)
            {
                nodeEnd->connectToInput(nodeStart, m_IndexStart, m_IndexEnd, error);
            }
            else
            {
                nodeEnd->connectToOutput(nodeStart, m_IndexStart, m_IndexEnd, error);
            }
        }
        else
        {
            if (m_StartAtOutput == true)
            {
                nodeEnd->disconnect(m_IndexEnd);
            }
            else
            {
                nodeStart->disconnect(m_IndexStart);
            }
        }
    }
}

void MaterialGeneratorGraphNodeConnectCommand::startConnect( const Guid& startNode, const bool startAtOutput, const uint8 startIndex )
{
    HE_ASSERT(m_IsConnection == false, "Already connecting a node!");
    m_NodeStart = startNode;
    m_StartAtOutput = startAtOutput;
    m_IndexStart = startIndex;
    m_IsConnection = true;
}

bool MaterialGeneratorGraphNodeConnectCommand::doConnect( const Guid& endNode, const uint8 endIndex )
{
    bool result(false);
    HE_IF_ASSERT(m_IsConnection == true, "Ending connection when not started one!")
    {
        m_NodeEnd = endNode;
        m_IndexEnd = endIndex;
        result = doCommand();
        if (result == true)
            m_Parent->m_CommandStack.pushCommand(Command(*this));
    }
    m_IsConnection = false;
    return result;
}

bool MaterialGeneratorGraphNodeConnectCommand::doCommand()
{
    MaterialGeneratorNode* nodeStart(m_Parent->getNode(m_NodeStart));
    MaterialGeneratorNode* nodeEnd(m_Parent->getNode(m_NodeEnd));
    bool result(false);
    HE_IF_ASSERT(nodeStart != nullptr && nodeEnd != nullptr, "Node start or end could not be found when connecting nodes")
    {
        MaterialGeneratorError error;
        if (m_StartAtOutput == true)
        {
            result = nodeEnd->connectToInput(nodeStart, m_IndexStart, m_IndexEnd, error);
        }
        else
        {
            result = nodeEnd->connectToOutput(nodeStart, m_IndexStart, m_IndexEnd, error);
        }
    }
    return result;
}

void MaterialGeneratorGraphNodeConnectCommand::undoCommand()
{
    MaterialGeneratorNode* nodeStart(m_Parent->getNode(m_NodeStart));
    MaterialGeneratorNode* nodeEnd(m_Parent->getNode(m_NodeEnd));
    HE_IF_ASSERT(nodeStart != nullptr && nodeEnd != nullptr, "Node start or end could not be found when connecting nodes")
    {
        if (m_StartAtOutput == true)
        {
            nodeEnd->disconnect(m_IndexEnd);
        }
        else
        {
            nodeStart->disconnect(m_IndexStart);
        }
    }
}

void MaterialGeneratorGraphNodeConnectCommand::cancelConnect()
{
    m_IsConnection = false;
}

} } //end namespace
