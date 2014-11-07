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
#include "MaterialGeneratorNodeConnector.h"

#include "MaterialGeneratorNode.h"
#include "MaterialGeneratorNodeTypes.h"
#include "../forms/MaterialGraph.h"

#include <ShaderGeneratorVariableFactory.h>
#include <ShaderGenerator.h>

namespace hs {

MaterialGeneratorNodeConnector::MaterialGeneratorNodeConnector( MaterialGeneratorNode* const parent, const MaterialGeneratorNodeConnectorDesc& desc )
    : m_Parent(parent)
    , m_VariableType(MaterialGeneratorVariableType_Unknown)
    , m_Variable(he::ObjectHandle::unassigned)
{
    he::vec3 hsb(desc.m_Color.toHSB());

    ConnectorStyle style;
    style.m_ConnectorBackgroundColor[eConnectorState_Normal] = desc.m_Color;
    style.m_ConnectorBackgroundColor[eConnectorState_Hover] = he::Color::fromHSB(hsb.x, hsb.y - 0.2f, 1.0f);
    style.m_ConnectorBackgroundColor[eConnectorState_Selected] = he::Color::fromHSB(hsb.x, hsb.y - 0.4f, 1.0f);

    style.m_ConnectorBorderColor[eConnectorState_Normal] = he::Color::fromHSB(hsb.x, hsb.y, 0.3f);
    style.m_ConnectorBorderColor[eConnectorState_Hover] = he::Color::fromHSB(hsb.x, 1.0f, 1.0f);
    style.m_ConnectorBorderColor[eConnectorState_Selected] = he::Color::fromHSB(hsb.x, 1.0f, 1.0f);

    setConnectorStyle(style);
}

MaterialGeneratorNodeConnector::~MaterialGeneratorNodeConnector()
{
    if (getType() == eNodeGraphNodeConnectorType_Output && m_Variable != he::ObjectHandle::unassigned)
    {
        m_Parent->getParent()->getShaderGenerator()->removeVariable(m_Variable);
    }
}

void MaterialGeneratorNodeConnector::setType( const ENodeGraphNodeConnectorType type )
{
    if (getType() != type)
    {
        NodeGraphNodeConnector::setType(type);
        if (type == eNodeGraphNodeConnectorType_Output)
        {
            m_Variable = m_Parent->getParent()->getShaderGenerator()->addVariable();
        }
        else
        {
            m_Parent->getParent()->getShaderGenerator()->removeVariable(m_Variable);
            m_Variable = he::ObjectHandle::unassigned;
        }
    }
}

bool MaterialGeneratorNodeConnector::connect( NodeGraphNodeConnector* other )
{
    if (NodeGraphNodeConnector::connect(other))
    {
        m_Parent->notifyNodeConnected(this, he::checked_cast<MaterialGeneratorNodeConnector*>(other));
        return true;
    }
    return false;
}

bool MaterialGeneratorNodeConnector::disconnect( NodeGraphNodeConnector* other )
{
    if (NodeGraphNodeConnector::disconnect(other))
    {
        m_Parent->notifyNodeDisconnected(this, he::checked_cast<MaterialGeneratorNodeConnector*>(other));
        return true;
    }
    return false;
}

void MaterialGeneratorNodeConnector::setVar( const he::ObjectHandle var )
{
    m_Variable = var;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

MaterialGeneratorNodeConnectorAttachment::MaterialGeneratorNodeConnectorAttachment( 
    MaterialGeneratorNode* const parent, const MaterialGeneratorNodeConnectorDesc& desc )
    : m_Connector(parent, desc)
{
    
}

} //end namespace
