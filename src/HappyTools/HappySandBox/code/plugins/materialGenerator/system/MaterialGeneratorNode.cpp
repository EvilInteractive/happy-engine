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
//Created: 14/11/2012
#include "HappySandBoxPCH.h" 
#include "MaterialGeneratorNode.h"

#include "MaterialGeneratorNodeConnector.h"

#include "NodeGraphError.h"
#include "StructuredVisitor.h"
#include "BinaryFileVisitor.h"

namespace hs {
    
//////////////////////////////////////////////////////////////////////////
// MaterialGeneratorNode
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_OBJECT(MaterialGeneratorNode)

MaterialGeneratorNode::MaterialGeneratorNode(): 
    m_SelectedOverload(0), m_Overloads(1)
{
    startEdit();
}

void MaterialGeneratorNode::init()
{
    HE_ASSERT(m_Overloads.size() > 0, "Node does not have any overloads?");
    if (m_Overloads.size() > 0)
    {
        activateOverload(0);
    }
    setTitle(materialGeneratorNodeTypeToString(getType()));
    endEdit();
}

MaterialGeneratorNode::~MaterialGeneratorNode()
{
}

void MaterialGeneratorNode::addInput( const MaterialGeneratorNodeConnectorDesc& desc )
{
    MaterialGeneratorNodeConnectorAttachment* att(NEW MaterialGeneratorNodeConnectorAttachment(this, desc));
    att->init(eNodeGraphNodeConnectorType_Input, desc.m_Name.c_str());
    addAttachment(att);
}

void MaterialGeneratorNode::addOutput( const MaterialGeneratorNodeConnectorDesc& desc )
{
    MaterialGeneratorNodeConnectorAttachment* att(NEW MaterialGeneratorNodeConnectorAttachment(this, desc));
    att->init(eNodeGraphNodeConnectorType_Output, desc.m_Name.c_str());
    addAttachment(att);
}

void MaterialGeneratorNode::addOverload( MaterialGeneratorNodeOverload overload )
{
    HE_ASSERT(m_Overloads.size() == 0 || 
        (m_Overloads[0].getInputs().size() == overload.getInputs().size() && 
         m_Overloads[0].getOutputs().size() == overload.getOutputs().size()), 
        "Incompatible amount of inputs or outputs supplied with overload:\n Outputs: %d/%d\n Inputs: %d/%d", 
        overload.getOutputs().size(), m_Overloads[0].getOutputs().size(), overload.getInputs().size(), m_Overloads[0].getInputs().size());

    m_Overloads.add(std::move(overload));
}

bool MaterialGeneratorNode::evaluate()
{
    bool result(false);
    size_t overload(0);
    if (findOverload(overload))
    {
        result = true;
        activateOverload(overload);
    }
    else
    {
        //error.setMessage("Error! Incorrect input/output types, could not find compatible overload! Check your input and outputs.");
        result = false;
    }
    return result;
}

void MaterialGeneratorNode::activateOverload( size_t overloadIndex )
{
    HE_IF_ASSERT(overloadIndex < m_Overloads.size(), "Overload out of range!")
    {
        MaterialGeneratorNodeOverload& overload(m_Overloads[overloadIndex]);
        const MaterialGeneratorNodeOverload::TTypeList& outputs(overload.getOutputs());
        const MaterialGeneratorNodeOverload::TTypeList& inputs(overload.getInputs());

        size_t outputCount(m_Outputs.size());
        size_t inputCount(m_Inputs.size());

        for (size_t i(0); i < outputCount; ++i)
        {
            MaterialGeneratorNodeConnectorAttachment* connectorAtt(m_Outputs[i]);
            MaterialGeneratorNodeConnector& connector(connectorAtt->getMaterialNodeConnector());
            connector.setVariableType(outputs[i]);
        }
        for (size_t i(0); i < inputCount; ++i)
        {
            MaterialGeneratorNodeConnectorAttachment* connectorAtt(m_Inputs[i]);
            MaterialGeneratorNodeConnector& connector(connectorAtt->getMaterialNodeConnector());
            connector.setVariableType(inputs[i]);
        }
    }
}

bool MaterialGeneratorNode::findOverload( size_t& outOverload ) const
{
    bool result(false);
    const size_t inputCount(m_Inputs.size());

    const size_t overloadCount(m_Overloads.size());
    for (size_t i(0); i < overloadCount && result == false; ++i)
    {
        const MaterialGeneratorNodeOverload& overload(m_Overloads[i]);
        result = true;
        outOverload = i;
        for (size_t inputIndex(0); inputIndex < inputCount; ++inputIndex)
        {
            const MaterialGeneratorNodeConnector& connection(m_Inputs[inputIndex]->getMaterialNodeConnector());
            if (connection.isConnected()) // if input is connected
            {
                const MaterialGeneratorNodeConnector* connectedConnector(he::checked_cast<const MaterialGeneratorNodeConnector*>(connection.getConnections()[0]));
                if (connectedConnector->getVariableType() != overload.getInputs()[inputIndex]) // if inputs type is not compatible with overload
                {
                    result = false; // result = false -> try other overload
                }
                break;
            }
        }
    }
    return result;
}

void MaterialGeneratorNode::addParam( const MaterialGeneratorNodeParam& param )
{
    m_Params.add(param);
}

const MaterialGeneratorNodeParam& MaterialGeneratorNode::getParam( const size_t index ) const
{
    return m_Params[index];
}

void MaterialGeneratorNode::visit( he::io::StructuredVisitor* const /*visitor*/ )
{
    /*visitor->visit(HEFS::strPosition, m_Position);
    visitor->visit(HEFS::strID, m_Guid);*/
}

void MaterialGeneratorNode::visit( he::io::BinaryFileVisitor* const /*visitor*/ )
{
   /* visitor->visit(m_Position);
    visitor->visit(m_Guid);*/
}

MaterialGeneratorNodeConnector& MaterialGeneratorNode::getInputConnector( const size_t index ) const
{
    return m_Inputs[index]->getMaterialNodeConnector();
}

MaterialGeneratorNodeConnector& MaterialGeneratorNode::getOutputConnector( const size_t index ) const
{
    return m_Outputs[index]->getMaterialNodeConnector();
}

void MaterialGeneratorNode::notifyNodeConnected( MaterialGeneratorNodeConnector* /*a*/, MaterialGeneratorNodeConnector* /*b*/ )
{

}

void MaterialGeneratorNode::notifyNodeDisconnected( MaterialGeneratorNodeConnector* /*a*/, MaterialGeneratorNodeConnector* /*b*/ )
{

}


} //end namespace
