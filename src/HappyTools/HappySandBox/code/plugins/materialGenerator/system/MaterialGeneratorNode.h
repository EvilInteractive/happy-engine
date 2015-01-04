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
    explicit MaterialGeneratorNode(MaterialGraph* const parent);
    virtual ~MaterialGeneratorNode();

    // Override me
    virtual const he::FixedString& getType() const { return HSFS::strUnassigned; }
    virtual bool evaluate();

    virtual void init();
    virtual void destroy();
    
    void notifyNodeConnected(MaterialGeneratorNodeConnector* a, MaterialGeneratorNodeConnector* b);
    void notifyNodeDisconnected(MaterialGeneratorNodeConnector* a, MaterialGeneratorNodeConnector* b);

    size_t getParamCount() const;
    MaterialGeneratorNodeParam& getParam(const size_t index);

    // Serializing
    virtual void visit(he::io::StructuredVisitor* const visitor) override;
    
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

    bool m_CompileState;
    bool m_Evaluating;

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNode(const MaterialGeneratorNode&);
    MaterialGeneratorNode& operator=(const MaterialGeneratorNode&);
};

}

#endif
