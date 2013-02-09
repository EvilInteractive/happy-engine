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
//Created: 11/11/2012

#ifndef _HE_NodeGraphNode_H_
#define _HE_NodeGraphNode_H_
#pragma once

namespace he {

template<typename TInput, typename TOutput>
struct NodeGraphError;

template<typename TInput, typename TOutput>
class NodeGraphNode;

template<typename TInput, typename TOutput>
class NodeGraphConnection
{
public:
    NodeGraphConnection(): node(nullptr), connecter(UINT8_MAX) {}
    ~NodeGraphConnection() {}

    NodeGraphNode<TInput, TOutput>* node;
    uint8 connecter;

    const TOutput& getConnection() const { return node->getOutput(connecter); }
    bool isConnected() const { return node != nullptr; }
};

template<typename TInput, typename TOutput>
class NodeGraphNodeOutputConnections : public ObjectList<NodeGraphConnection<TInput, TOutput>>
{
public:
    NodeGraphNodeOutputConnections() {}
    virtual ~NodeGraphNodeOutputConnections() {}

    NodeGraphNodeOutputConnections(const NodeGraphNodeOutputConnections<TInput, TOutput>& other):
        ObjectList<NodeGraphConnection<TInput, TOutput>>(other.m_Connections.size())
    {
        append(other);
    }
    NodeGraphNodeOutputConnections& operator=(const NodeGraphNodeOutputConnections<TInput, TOutput>& other)
    {
        this->clear();
        append(other);
        return *this;
    }
};

template<typename TInput, typename TOutput>
class NodeGraphNode
{
public:
    NodeGraphNode();
    virtual ~NodeGraphNode();

    uint8 addInput(const TInput& input);
    uint8 addOutput(const TOutput& output);

    const TInput& getInput(uint8 index) const { return m_Inputs[index]; }
    const TOutput& getOutput(uint8 index) const { return m_Outputs[index]; }
    TInput& getInput(uint8 index) { return m_Inputs[index]; }
    TOutput& getOutput(uint8 index) { return m_Outputs[index]; }
    const NodeGraphConnection<TInput, TOutput>& getInputConnection(const uint8 input) const { return m_InputSlots[input]; }
    const size_t getOutputConnectionCount(const uint8 output) const { return m_OutputSlots[output].size(); }
    const NodeGraphConnection<TInput, TOutput>& getOutputConnection(const uint8 output, const uint8 index) const { return m_OutputSlots[output][index]; }
    uint8 getInputCount() const { return static_cast<uint8>(m_Inputs.size()); }
    uint8 getOutputCount() const { return static_cast<uint8>(m_Outputs.size()); }

    bool connectToInput(NodeGraphNode* fromNode, uint8 fromOutput, uint8 toInput, NodeGraphError<TInput, TOutput>& error);
    bool connectToOutput(NodeGraphNode* fromNode, uint8 fromInput, uint8 toOutput, NodeGraphError<TInput, TOutput>& error);
    void disconnect(uint8 input);

    bool evaluteMarch(uint8 marchId, he::ObjectList<NodeGraphError<TInput, TOutput>>& errors);
    bool customMarch(uint8 marchId, const boost::function0<NodeGraphNode<TInput, TOutput>* const>& callback);

    virtual bool evaluate(NodeGraphError<TInput, TOutput>& error) = 0;
    virtual bool canConnect(const TOutput& fromOutput, const TInput& toInput, NodeGraphError<TInput, TOutput>& error) const = 0;

    // void, isInput, id
    event2<void, bool, uint8> NodeDisconnected;
    event2<void, bool, uint8> NodeConnected;

private:

    he::ObjectList<TInput> m_Inputs;
    he::ObjectList<TOutput> m_Outputs;

    uint8 m_LastMarchId;
    uint8 m_CurrentMarchId;

    he::ObjectList<NodeGraphConnection<TInput, TOutput>> m_InputSlots;
    he::ObjectList<NodeGraphNodeOutputConnections<TInput, TOutput>> m_OutputSlots;

    //Disable default copy constructor and default assignment operator
    NodeGraphNode(const NodeGraphNode&);
    NodeGraphNode& operator=(const NodeGraphNode&);
};

} //end namespace

#include "NodeGraphNode.inl"

#endif
