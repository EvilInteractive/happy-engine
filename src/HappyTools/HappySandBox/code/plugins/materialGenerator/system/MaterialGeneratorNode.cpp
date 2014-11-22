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

#include "StructuredVisitor.h"
#include "BinaryFileVisitor.h"

namespace hs {

//////////////////////////////////////////////////////////////////////////
// MaterialGeneratorNode
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_OBJECT(MaterialGeneratorNode)

MaterialGeneratorNode::MaterialGeneratorNode(): 
    m_Parent(nullptr),
    m_Evaluating(false), m_CompileState(true)
{
    startEdit();
}

void MaterialGeneratorNode::init()
{
    setTitle(materialGeneratorNodeTypeToString(getType()));
    endEdit();

    evaluate();
}

void MaterialGeneratorNode::destroy()
{

}

MaterialGeneratorNode::~MaterialGeneratorNode()
{
}

void MaterialGeneratorNode::addInput( const MaterialGeneratorNodeConnectorDesc& desc )
{
    MaterialGeneratorNodeConnectorAttachment* att(NEW MaterialGeneratorNodeConnectorAttachment(this, desc));
    att->init(eNodeGraphNodeConnectorType_Input, desc.m_Name.c_str());
    addAttachment(att);
    m_Inputs.add(att);
}

void MaterialGeneratorNode::addOutput( const MaterialGeneratorNodeConnectorDesc& desc )
{
    MaterialGeneratorNodeConnectorAttachment* att(NEW MaterialGeneratorNodeConnectorAttachment(this, desc));
    att->init(eNodeGraphNodeConnectorType_Output, desc.m_Name.c_str());
    addAttachment(att);
    m_Outputs.add(att);
}

bool MaterialGeneratorNode::evaluate()
{
    if (!m_Evaluating) // Prevent infinite loops
    {
        m_Evaluating = true;
        m_Outputs.forEach([](MaterialGeneratorNodeConnectorAttachment* output)
        {
            output->getMaterialNodeConnector().getConnections().forEach([](NodeGraphNodeConnector* input)
            {
                he::checked_cast<MaterialGeneratorNodeConnector*>(input)->getParent()->evaluate();
            });
        });
        m_Evaluating = false;
    }
    return true;
}

void MaterialGeneratorNode::addParam( MaterialGeneratorNodeParam param )
{
    m_Params.add(std::move(param));
}

size_t MaterialGeneratorNode::getParamCount() const
{
    return m_Params.size();
}

MaterialGeneratorNodeParam& MaterialGeneratorNode::getParam( const size_t index )
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

void MaterialGeneratorNode::notifyNodeConnected( MaterialGeneratorNodeConnector* a, MaterialGeneratorNodeConnector* b )
{
    MaterialGeneratorNodeConnector* outputNode(a);
    MaterialGeneratorNodeConnector* inputNode(b);
    if (outputNode->getType() == eNodeGraphNodeConnectorType_Input)
    {
        outputNode = b;
        inputNode = a;
    }
    if (inputNode->getParent() == this) // only operator when input node is this node (else calculations are done twice)
    {
        inputNode->setVar(outputNode->getVar());
        evaluate();
    }
}

void MaterialGeneratorNode::notifyNodeDisconnected( MaterialGeneratorNodeConnector* a, MaterialGeneratorNodeConnector* b )
{
    MaterialGeneratorNodeConnector* outputNode(a);
    MaterialGeneratorNodeConnector* inputNode(b);
    if (outputNode->getType() == eNodeGraphNodeConnectorType_Input)
    {
        outputNode = b;
        inputNode = a;
    }
    if (inputNode->getParent() == this) // only operator when input node is this node (else calculations are done twice)
    {
        inputNode->setVar(he::ObjectHandle::unassigned);
        evaluate();
    }
}

void MaterialGeneratorNode::setCompileState( const bool ok )
{
    if (m_CompileState != ok)
    {
        m_CompileState = ok;
        if (ok)
            setStyle(&Style::s_DefaultStyle);
        else
            setStyle(&Style::s_ErrorStyle);
    }
}

} //end namespace
