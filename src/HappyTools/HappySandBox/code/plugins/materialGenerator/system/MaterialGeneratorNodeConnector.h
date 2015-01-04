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
//Created: 2014/10/31

#ifndef _HE_MaterialGeneratorNodeConnector_H_
#define _HE_MaterialGeneratorNodeConnector_H_
#pragma once

#include "forms/nodegraph/NodeGraphNodeConnector.h"
#include "forms/nodegraph/NodeGraphNodeAttachments.h"
#include "MaterialGeneratorNodeTypes.h"

namespace hs {
  
class MaterialGeneratorNode;

struct MaterialGeneratorNodeConnectorDesc
{
    MaterialGeneratorNodeConnectorDesc() {}
    MaterialGeneratorNodeConnectorDesc(const he::FixedString& id, const he::String& name, const he::Color& color)
        : m_Id(id)
        , m_Name(name)
        , m_Color(color) {}
    he::FixedString m_Id;
    he::String m_Name;
    he::Color m_Color;
};

class MaterialGeneratorNodeConnector : public hs::NodeGraphNodeConnector
{
public:
    MaterialGeneratorNodeConnector(NodeGraphNodeAttachment* const parent, const MaterialGeneratorNodeConnectorDesc& desc);
    ~MaterialGeneratorNodeConnector();

    bool connect(NodeGraphNodeConnector* other) final;
    bool disconnect(NodeGraphNodeConnector* other) final;
    void setType(const ENodeGraphNodeConnectorType type) final;

    void setVariableType(const MaterialGeneratorVariableType type) { m_VariableType = type; }
    MaterialGeneratorVariableType getVariableType() const { return m_VariableType; }

    he::ObjectHandle getVar() const { return m_Variable; }
    void setVar(const he::ObjectHandle var);

private:
    MaterialGeneratorVariableType m_VariableType;
    he::ObjectHandle m_Variable;

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNodeConnector(const MaterialGeneratorNodeConnector&);
    MaterialGeneratorNodeConnector& operator=(const MaterialGeneratorNodeConnector&);
};

class MaterialGeneratorNodeConnectorAttachment : public hs::NodeGraphNodeTextConnector
{
public:
    MaterialGeneratorNodeConnectorAttachment(NodeGraphNode* const parent, const MaterialGeneratorNodeConnectorDesc& desc);

    NodeGraphNodeConnector* getNodeConnector() final { return &m_Connector; }
    MaterialGeneratorNodeConnector& getMaterialNodeConnector() { return m_Connector; }

private:
    MaterialGeneratorNodeConnector m_Connector;
};

} //end namespace

#endif
