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

#include "MaterialGeneratorGraphCommands.h"

#include "MaterialGeneratorGraph.h"
#include "MaterialGeneratorNode.h"
#include "MaterialGeneratorNodeFactory.h"

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
    if (type == CommandType_Do)
    {
        doCommand();
    }
    else
    {
        undoCommand();
    }
}

void MaterialGeneratorGraphNodeConnectCommand::startConnect( const Guid& startNode, const bool startAtOutput, const uint8 startIndex )
{
    HE_ASSERT(m_IsConnecting == false, "Already connecting a node!");
    m_NodeStart = startNode;
    m_StartAtOutput = startAtOutput;
    m_IndexStart = startIndex;
    m_IsConnecting = true;
}

bool MaterialGeneratorGraphNodeConnectCommand::doConnect( const Guid& endNode, const uint8 endIndex )
{
    m_IsConnect = true;
    bool result(false);
    HE_IF_ASSERT(m_IsConnecting == true, "Ending connection when not started one!")
    {
        m_NodeEnd = endNode;
        m_IndexEnd = endIndex;
        result = doCommand();
        if (result == true)
            m_Parent->m_CommandStack.pushCommand(Command(*this));
    }
    m_IsConnecting = false;
    return result;
}

bool MaterialGeneratorGraphNodeConnectCommand::doCommand()
{
    MaterialGeneratorNode* nodeStart(m_Parent->getNode(m_NodeStart));
    MaterialGeneratorNode* nodeEnd(m_Parent->getNode(m_NodeEnd));
    bool result(false);
    HE_IF_ASSERT(nodeStart != nullptr && nodeEnd != nullptr, "Node start or end could not be found when connecting nodes")
    {
        if (m_IsConnect)
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
            if (error.getMessage().empty() == false)
            {
                m_Parent->pushError(error);
            }
        }
        else
        {
            nodeStart->disconnect(m_IndexStart);
            result = true;
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
        if (m_IsConnect)
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
        else
        {
            MaterialGeneratorError error;
            nodeEnd->connectToOutput(nodeStart, m_IndexStart, m_IndexEnd, error);
            if (error.getMessage().empty() == false)
            {
                m_Parent->pushError(error);
            }
        }
    }
}

void MaterialGeneratorGraphNodeConnectCommand::cancelConnect()
{
    m_IsConnecting = false;
}

bool MaterialGeneratorGraphNodeConnectCommand::doDisconnect( const Guid& nodeId, const bool isInput, const uint8 index  )
{
    MaterialGeneratorNode* const node(m_Parent->getNode(nodeId));
    return doDisconnect(node, isInput, index);
}

bool MaterialGeneratorGraphNodeConnectCommand::doDisconnect( MaterialGeneratorNode* const node, const bool isInput, const uint8 index )
{
    if (isInput)
    {
        const MaterialGeneratorConnection& connection(node->getInputConnection(index));
        MaterialGeneratorNode* const endNode(static_cast<MaterialGeneratorNode*>(connection.node));

        m_IsConnect = false;
        m_StartAtOutput = false;
        m_NodeStart = node->getGuid();
        m_NodeEnd = endNode->getGuid();
        m_IndexStart = index;
        m_IndexEnd = connection.connecter;

        node->disconnect(index);

        m_Parent->m_CommandStack.pushCommand(Command(*this));
    }
    else
    {
        for (uint8 i(0); i < node->getOutputConnectionCount(index); ) // we do not increase i because the size will be lowered by the disconnect
        {
            const MaterialGeneratorConnection& conn(node->getOutputConnection(index, i));
            doDisconnect(static_cast<MaterialGeneratorNode*>(conn.node), true, conn.connecter);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
// Create Node
//////////////////////////////////////////////////////////////////////////
void MaterialGeneratorGraphCreateCommand::operator()( const CommandType type )
{
    MaterialGeneratorNodeFactory* const factory(MaterialGeneratorNodeFactory::getInstance());
    if (type == CommandType_Do)
    {
        MaterialGeneratorNode* const node(factory->create(m_Type, m_Id));
        m_Parent->addNode(node);
        node->setPosition(m_Position);
    }
    else
    {
        size_t index(0);
        m_Parent->m_NodeList.find_if([this](MaterialGeneratorNode* node) { return node->getGuid() == m_Id; }, index);
        factory->destroy(m_Parent->m_NodeList[index]);
        m_Parent->m_NodeList.removeAt(index);
    }
}

void MaterialGeneratorGraphCreateCommand::create( const MaterialGeneratorNodeType type, const vec2& position )
{
    MaterialGeneratorNodeFactory* const factory(MaterialGeneratorNodeFactory::getInstance());
    MaterialGeneratorNode* const node(factory->create(type));
    node->setPosition(position);
    m_Parent->addNode(node);
    m_Id = node->getGuid();
    m_Type = type;
    m_Position = position;
    m_Parent->m_CommandStack.pushCommand(Command(*this));
}

//////////////////////////////////////////////////////////////////////////
// Delete Node
//////////////////////////////////////////////////////////////////////////

void MaterialGeneratorGraphDeleteCommand::operator()( const CommandType type )
{
    if (type == CommandType_Do)
    {
        MaterialGeneratorNode* const node(m_Parent->getNode(m_Id));
        size_t index(0);
        if (m_Parent->m_SelectedNodeList.find(node, index))
        {
            m_Parent->m_SelectedNodeList.removeAt(index);
        }
        m_Parent->m_NodeList.remove(node);
        MaterialGeneratorNodeFactory::getInstance()->destroy(node);
    }
    else
    {
        MaterialGeneratorNodeFactory* const factory(MaterialGeneratorNodeFactory::getInstance());
        MaterialGeneratorNode* node(factory->create(m_Type, m_Id));
        m_Parent->addNode(node);
        node->setPosition(m_Position);
        m_Parent->m_SelectedNodeList.add(node);
    }
}

void MaterialGeneratorGraphDeleteCommand::doDelete( const Guid& id )
{
    MaterialGeneratorNode* const node(m_Parent->getNode(id));
    doDelete(node);
}

void MaterialGeneratorGraphDeleteCommand::doDelete( MaterialGeneratorNode* const node )
{
    // Disconnect node with commands
    const uint8 inputCount(node->getInputCount());
    MaterialGeneratorGraphNodeConnectCommand command(m_Parent);
    for (uint8 i(0); i < inputCount; ++i)
    {
        const MaterialGeneratorConnection& conn(node->getInputConnection(i));
        if (conn.isConnected())
        {
            command.doDisconnect(node->getGuid(), true, i);
        }
    }
    const uint8 outputCount(node->getOutputCount());
    for (uint8 i(0); i < outputCount; ++i)
    {
        const size_t outputConnections(node->getOutputConnectionCount(i));
        if (outputConnections > 0)
        {
            command.doDisconnect(node->getGuid(), false, i);
        }
    }

    // Real delete
    m_Id = node->getGuid();
    m_Type = node->getType();
    m_Position = node->getPosition();
    m_Parent->m_NodeList.remove(node);
    MaterialGeneratorNodeFactory::getInstance()->destroy(node);

    m_Parent->m_CommandStack.pushCommand(Command(*this));
}

} } //end namespace
