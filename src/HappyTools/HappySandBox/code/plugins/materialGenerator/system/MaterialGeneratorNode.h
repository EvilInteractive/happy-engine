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

#ifndef _HE_MaterialGeneratorNode_H_
#define _HE_MaterialGeneratorNode_H_
#pragma once

#include "forms/nodegraph/NodeGraphNode.h"
#include "MaterialGeneratorNodeOverload.h"
#include "MaterialGeneratorNodeParam.h"

namespace he {
namespace io {
    class StructuredVisitor;
    class BinaryFileVisitor;
}
namespace gui {
    class Sprite;
    class BezierShape2D;
    class Canvas2D;
}
}

namespace hs {
class MaterialGraph;
struct MaterialGeneratorNodeConnectorDesc;
class MaterialGeneratorNodeConnectorAttachment;
class MaterialGeneratorNodeConnector;

class MaterialGeneratorNode : public NodeGraphNode
{
DECLARE_OBJECT(MaterialGeneratorNode);
public:
    MaterialGeneratorNode();
    virtual ~MaterialGeneratorNode();

    // Override me
    virtual MaterialGeneratorNodeType getType() const { return MaterialGeneratorNodeType_Unassigned; }
    virtual bool evaluate();

    virtual void init();
    virtual void destroy();

    void setParent(MaterialGraph* const parent) { m_Parent = parent; }
    MaterialGraph* getParent() { return m_Parent; }

    void setGuid(const he::Guid& id) { m_Guid = id; }
    const he::Guid& getGuid() const { return m_Guid; }

    void notifyNodeConnected(MaterialGeneratorNodeConnector* a, MaterialGeneratorNodeConnector* b);
    void notifyNodeDisconnected(MaterialGeneratorNodeConnector* a, MaterialGeneratorNodeConnector* b);

    size_t getParamCount() const;
    MaterialGeneratorNodeParam& getParam(const size_t index);

    // Serializing
    void visit(he::io::StructuredVisitor* const visitor);
    void visit(he::io::BinaryFileVisitor* const visitor);
    
protected:
    // Setters
    void addInput(const MaterialGeneratorNodeConnectorDesc& desc);
    void addOutput(const MaterialGeneratorNodeConnectorDesc& desc);
    void setCompileState(const bool ok);

    void addParam(MaterialGeneratorNodeParam param);

    void setName(const he::String name);

    // Usage
    MaterialGeneratorNodeConnector& getInputConnector(const size_t index) const;
    MaterialGeneratorNodeConnector& getOutputConnector(const size_t index) const;

private:
    he::PrimitiveList<MaterialGeneratorNodeConnectorAttachment*> m_Inputs;
    he::PrimitiveList<MaterialGeneratorNodeConnectorAttachment*> m_Outputs;

    he::ObjectList<MaterialGeneratorNodeParam> m_Params;

    he::Guid m_Guid;
    MaterialGraph* m_Parent;

    bool m_CompileState;
    bool m_Evaluating;

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNode(const MaterialGeneratorNode&);
    MaterialGeneratorNode& operator=(const MaterialGeneratorNode&);
};

}

#endif
