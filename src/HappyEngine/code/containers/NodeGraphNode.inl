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
//Created: 11/11/2012

namespace he {

template<typename TInput, typename TOutput>
NodeGraphNode<TInput, TOutput>::NodeGraphNode(): m_LastMarchId(UINT8_MAX), m_CurrentMarchId(UINT8_MAX)
{
}

template<typename TInput, typename TOutput>
NodeGraphNode<TInput, TOutput>::~NodeGraphNode()
{
}

template<typename TInput, typename TOutput>
uint8 NodeGraphNode<TInput, TOutput>::addInput( const TInput& input )
{
    m_Inputs.add(input);
    m_InputSlots.add(NodeGraphConnection<TInput, TOutput>());
    return static_cast<uint8>(m_Inputs.size() - 1);
}
template<typename TInput, typename TOutput>
uint8 NodeGraphNode<TInput, TOutput>::addOutput( const TOutput& output )
{
    m_Outputs.add(output); 
    m_OutputSlots.resize(m_OutputSlots.size() + 1);
    return static_cast<uint8>(m_Outputs.size() - 1); 
}

template<typename TInput, typename TOutput>
bool NodeGraphNode<TInput, TOutput>::connectToInput( NodeGraphNode* fromNode, uint8 fromOutput, uint8 toInput, NodeGraphError<TInput, TOutput>& error )
{
    if (canConnect(fromNode->getOutput(fromOutput), getInput(toInput), error))
    {
        disconnect(toInput);
        m_InputSlots[toInput].node = fromNode;
        m_InputSlots[toInput].connecter = fromOutput;
        NodeConnected(true, toInput);

        NodeGraphConnection<TInput, TOutput> connection;
        connection.node = this;
        connection.connecter = toInput;
        fromNode->m_OutputSlots[fromOutput].add(connection);
        fromNode->NodeConnected(false, fromOutput);
        return true;
    }
    else
    {
        error.m_Node = this;
        error.m_Connecter = getInputConnection(toInput);
        error.m_Fixed = true;
        return false;
    }
}

template<typename TInput, typename TOutput>
bool he::NodeGraphNode<TInput, TOutput>::connectToOutput( NodeGraphNode* fromNode, uint8 fromInput, uint8 toOutput, NodeGraphError<TInput, TOutput>& error )
{
    if (canConnect(getOutput(toOutput), fromNode->getInput(fromInput), error))
    {
        fromNode->disconnect(fromInput);
        fromNode->m_InputSlots[fromInput].node = this;
        fromNode->m_InputSlots[fromInput].connecter = toOutput;
        fromNode->NodeConnected(true, fromInput);

        NodeGraphConnection<TInput, TOutput> connection;
        connection.node = fromNode;
        connection.connecter = fromInput;
        m_OutputSlots[toOutput].add(connection);
        NodeConnected(false, toOutput);
        return true;
    }
    else
    {
        error.m_Node = fromNode;
        error.m_Connecter = fromNode->getInputConnection(fromInput);
        error.m_Fixed = true;
        return false;
    }
}


template<typename TInput, typename TOutput>
void NodeGraphNode<TInput, TOutput>::disconnect( uint8 input )
{
    NodeGraphConnection<TInput, TOutput>& connection(m_InputSlots[input]);
    if (connection.isConnected())
    {
        HE_ASSERT(connection.node != nullptr, "Node is connected but other node is nullptr!");

        // Disconnect output
        NodeGraphConnection<TInput, TOutput>& inputConnection(m_InputSlots[input]);
        size_t outputIndex(0);
        const bool foundOutput(inputConnection.node->m_OutputSlots[inputConnection.connecter].find_if(
            [this](const NodeGraphConnection<TInput, TOutput>& conn) -> bool { return conn.node == this; }, outputIndex));
        HE_ASSERT(foundOutput, "Output connection not found when disconnecting"); foundOutput;
        inputConnection.node->m_OutputSlots[inputConnection.connecter].removeAt(outputIndex);

        // Throw output event
        connection.node->NodeDisconnected(false, connection.connecter);

        // Reset
        inputConnection.node = nullptr;
        inputConnection.connecter = UINT8_MAX;

        // Throw event
        NodeDisconnected(true, input);
    }
}

template<typename TInput, typename TOutput>
bool NodeGraphNode<TInput, TOutput>::evaluteMarch(uint8 marchId, he::ObjectList<NodeGraphError<TInput, TOutput>>& errors)
{
    if (m_LastMarchId == marchId) return true; // node already evaluated
    if (m_CurrentMarchId == marchId)
    {
        NodeGraphError<TInput, TOutput> error;
        error.m_ErrorMessage = "Warning! Endless loop detected - stopped evaluating branch";
        error.m_Node = this;
        error.m_Fixed = false;
        errors.add(error);
        return false;
    }

    m_CurrentMarchId = marchId;
    m_InputSlots.forEach([marchId, &errors](const NodeGraphConnection<TInput, TOutput>& connection)
    {
        if (connection.isConnected())
        {
            connection.node->evaluteMarch(marchId, errors);
        }
    });
    NodeGraphError<TInput, TOutput> error;
    if (evaluate(error) == false)
    {
        error.m_Node = this;
        error.m_Fixed = false;
        errors.add(error);
        return false;
    }
    m_LastMarchId = m_CurrentMarchId;
    return true;
}

template<typename TInput, typename TOutput>
bool he::NodeGraphNode<TInput, TOutput>::customMarch( uint8 marchId, const boost::function0<NodeGraphNode<TInput, TOutput>* const>& callback )
{
    bool result(true);
    if (m_LastMarchId != marchId)  // node already passed
    {
        if (m_CurrentMarchId == marchId)
        {
            HE_WARNING("Warning! Endless loop detected - stopped evaluating branch");
            result = false;
        }
        else
        {
            m_CurrentMarchId = marchId;
            m_InputSlots.forEach([marchId, &callback](const NodeGraphConnection<TInput, TOutput>& connection)
            {
                connection.node->customMarch(marchId, callback);
            });
            m_LastMarchId = m_CurrentMarchId;
        }
    }
    return result;
}

} //end namespace
