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

#include "NodeGraphNode.h"
#include "MaterialGeneratorNodeInOut.h"
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
namespace tools {

class MaterialGeneratorGraph;

typedef NodeGraphNode<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorNodeBase;
typedef NodeGraphError<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorError;
typedef NodeGraphConnection<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorConnection;

class MaterialGeneratorNode : public MaterialGeneratorNodeBase
{
DECLARE_OBJECT(MaterialGeneratorNode);
protected:
    struct ConnecterDesc
    {
        ConnecterDesc() {}
        ConnecterDesc(const he::String& name, const Color& color)
            : m_Name(name)
            , m_Color(color) {}
        he::String m_Name;
        Color m_Color;
    };
public:
    class Connecter
    {
    public:
        Connecter(MaterialGeneratorNode* const parent, const bool isInput, const uint8 index, const ConnecterDesc& desc);
        ~Connecter();

        void setPosition(const vec2& position);
        const vec2& getPosition() const { return m_Position; }

        void setConnectedConnecter(Connecter* connecter);
        void setConnected(const bool connected);

        const vec2& getSize() const { return m_Size; }
        bool isInput() const { return m_IsInput; }
        uint8 getIndex() const { return m_Index; }
        bool isConnected() const { return m_IsConnected; }
        MaterialGeneratorNode* getParent() const { return m_Parent; }
        Connecter* getConnection() const { return m_ConnectedConnecter; }

        bool pick(const vec2& worldPos) const;
        bool doHoover(const vec2& worldPos, const bool undoHoover);
        void setSelected(const bool isSelected) { m_IsSelected = isSelected; }
        void setHoover(const bool isHoovered) { m_IsHooverd = isHoovered; }

        void renderSprites();
        void draw2D(gui::Canvas2D* const canvas, const mat33& transform) const;

    private:
        he::event0<void> Moved;

        gui::Sprite* m_Sprites[3];
        vec2 m_Position;
        const vec2 m_Size;
        const ConnecterDesc m_Desc;
        const bool m_IsInput;
        const uint8 m_Index;
        bool m_IsSelected;
        bool m_IsHooverd;
        MaterialGeneratorNode* const m_Parent;

        bool m_IsConnected;
        gui::BezierShape2D* m_Bezier;
        Connecter* m_ConnectedConnecter;
        he::eventCallback0<void> m_ConnectionMovedCallback;

        //Disable default copy constructor and default assignment operator
        Connecter(const Connecter&);
        Connecter& operator=(const Connecter&);
    };
public:
    MaterialGeneratorNode();
    virtual ~MaterialGeneratorNode();

    void init();

    virtual bool evaluate(MaterialGeneratorError& error);

    virtual bool canConnect(const MaterialGeneratorNodeOutput& fromOutput, 
                            const MaterialGeneratorNodeInput& toInput, MaterialGeneratorError& error) const;

    bool pick(const vec2& worldPos) const;
    bool doHoover(const vec2& worldPos, const bool undoHoover);
    void setSelected(bool selected) { m_IsSelected = selected; }
    bool isSelected() const { return m_IsSelected; }

    void setParent(MaterialGeneratorGraph* const parent) { m_Parent = parent; }
    MaterialGeneratorGraph* getParent() { return m_Parent; }

    void setPosition(const vec2& position);
    const vec2& getPosition() const { return m_Position; }

    virtual MaterialGeneratorNodeType getType() const { return MaterialGeneratorNodeType_Unassigned; }

    void setGuid(const Guid& id) { m_Guid = id; }
    const Guid& getGuid() const { return m_Guid; }
    const he::PrimitiveList<Connecter*>& getConnecters() const { return m_Connecters; }
    he::PrimitiveList<Connecter*>& getConnectersForEdit() { return m_Connecters; }
    
    void visit(io::StructuredVisitor* const visitor);
    void visit(io::BinaryFileVisitor* const visitor);

    void draw2D(gui::Canvas2D* const canvas, const mat33& transform, const RectF& clipRect);

protected:
    void addOverload(uint8 outputCount, uint8 inputCount, ...);   // Takes MaterialGeneratorVariableType's
    template<typename T, typename R> // T, R == list of ConnecterDesc
    void addConnecters(const T& outputs, const R& inputs);
    template<typename T> // T == list of ConnecterDesc
    void addConnecterInputs(const T& inputs);
    template<typename T> // T == list of ConnecterDesc
    void addConnecterOutputs(const T& outputs);

    void addParam(const MaterialGeneratorNodeParam& param);
    const MaterialGeneratorNodeParam& getParam(const uint8& index);
    
    void setSize(const vec2& size);
    void setCanBeSelected(const bool canBeSelected) { m_CanBeSelected = canBeSelected; }

    MaterialGeneratorGraph* m_Parent;
private:
    struct Overload
    {
        he::PrimitiveList<MaterialGeneratorVariableType> outputs;
        he::PrimitiveList<MaterialGeneratorVariableType> inputs;
    
        Overload() {}
        ~Overload() {}
        Overload(const Overload& other): outputs(other.outputs.size()), inputs(other.inputs.size()) 
        {
            outputs.append(other.outputs);
            inputs.append(other.inputs);
        }
        Overload& operator=(const Overload& other) 
        { 
            outputs.clear(); outputs.append(other.outputs); 
            inputs.clear();  inputs.append(other.inputs); 
            return *this;
        }
    };

    he::ObjectList<Overload> m_Overloads;
    uint8 m_SelectedOverload;

    he::ObjectList<MaterialGeneratorNodeParam> m_Params;

    vec2 m_Position;
    vec2 m_Size;
    bool m_CanBeSelected;
    bool m_IsSelected;
    bool m_IsHoovering;
    Guid m_Guid;

    void activateOverload(uint8 overload);
    bool findOverload(uint8& outOverload) const;

    void updateConnecterPositions();
    bool isInView(const mat33& transform, const RectF& clipRect);
    
    he::PrimitiveList<Connecter*> m_Connecters;
    he::PrimitiveList<gui::Sprite*> m_Sprites;
    
    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNode(const MaterialGeneratorNode&);
    MaterialGeneratorNode& operator=(const MaterialGeneratorNode&);
};

template<typename T, typename R>
void he::tools::MaterialGeneratorNode::addConnecters( const T& outputs, const R& inputs )
{
    HE_ASSERT(m_Connecters.empty(), "Connecters already set!");

    uint8 outputCount(0);
    outputs.forEach([this, &outputCount](const ConnecterDesc& desc)
    {
        m_Connecters.add(NEW Connecter(this, false, outputCount++, desc));
    });

    uint8 inputCount(0);
    inputs.forEach([this, &inputCount](const ConnecterDesc& desc)
    {
        m_Connecters.add(NEW Connecter(this, true, inputCount++, desc));
    });


    HE_ASSERT(m_Overloads[0].outputs.size() == outputCount && m_Overloads[0].inputs.size() == inputCount,
        "Incompatible amount of inputs or outputs supplied with connecters:\n Outputs: %d/%d\n Inputs: %d/%d",
        outputCount, m_Overloads[0].outputs.size(), inputCount, m_Overloads[0].inputs.size());

    updateConnecterPositions();
}

template<typename T>
void he::tools::MaterialGeneratorNode::addConnecterOutputs( const T& outputs )
{
    HE_ASSERT(m_Connecters.empty(), "Connecters already set!");
    uint8 outputCount(0);
    outputs.forEach([this, &outputCount](const ConnecterDesc& desc)
    {
        m_Connecters.add(NEW Connecter(this, false, outputCount++, desc));
    });
    HE_ASSERT(m_Overloads[0].outputs.size() == outputCount && m_Overloads[0].inputs.size() == 0,
        "Incompatible amount of inputs or outputs supplied with connecters:\n Outputs: %d/%d\n Inputs: %d/%d",
        outputCount, m_Overloads[0].outputs.size(), 0, m_Overloads[0].inputs.size());

    updateConnecterPositions();
}

template<typename T>
void he::tools::MaterialGeneratorNode::addConnecterInputs( const T& inputs )
{
    HE_ASSERT(m_Connecters.empty(), "Connecters already set!");
    uint8 inputCount(0);
    inputs.forEach([this, &inputCount](const ConnecterDesc& desc)
    {
        m_Connecters.add(NEW Connecter(this, true, inputCount++, desc));
    });
    HE_ASSERT(m_Overloads[0].outputs.size() == 0 && m_Overloads[0].inputs.size() == inputCount,
        "Incompatible amount of inputs or outputs supplied with connecters:\n Outputs: %d/%d\n Inputs: %d/%d",
        0, m_Overloads[0].outputs.size(), inputCount, m_Overloads[0].inputs.size());

    updateConnecterPositions();
}

} } //end namespace

#endif
