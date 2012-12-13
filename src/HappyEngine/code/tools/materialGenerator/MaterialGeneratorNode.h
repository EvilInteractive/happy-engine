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
//Created: 14/11/2012

#ifndef _HE_MaterialGeneratorNode_H_
#define _HE_MaterialGeneratorNode_H_
#pragma once

#include "NodeGraphNode.h"
#include "MaterialGeneratorNodeInOut.h"

namespace he {
namespace gfx {
    class Canvas2D;
}
namespace gui {
    class Sprite;
    class BezierShape2D;
}
namespace tools {

typedef NodeGraphNode<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorNodeBase;
typedef NodeGraphError<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorError;
typedef NodeGraphConnection<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> MaterialGeneratorConnection;

class MaterialGeneratorNode : public MaterialGeneratorNodeBase
{
protected:
    struct ConnecterDesc
    {
        ConnecterDesc(const std::string& name, const Color& color)
            : m_Name(name)
            , m_Color(color) {}
        std::string m_Name;
        Color m_Color;
    };
public:
    class Connecter
    {
    public:
        Connecter(const bool isInput, const uint8 index, const ConnecterDesc& desc);
        ~Connecter();

        void setPosition(const vec2& position);
        const vec2& getPosition() const { return m_Position; }

        void setConnectedConnecter(Connecter* connecter);
        void setConnected(const bool connected);

        const vec2& getSize() const { return m_Size; }
        bool isInput() const { return m_IsInput; }
        uint8 getIndex() const { return m_Index; }

        bool pick(const vec2& worldPos) const;
        bool doHoover(const vec2& worldPos, const bool undoHoover);
        void setSelected(const bool isSelected) { m_IsSelected = isSelected; }
        void setHoover(const bool isHoovered) { m_IsHooverd = isHoovered; }

        void renderSprites();
        void draw2D(gfx::Canvas2D* const canvas, const mat33& transform) const;

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

        bool m_IsConnected;
        gui::BezierShape2D* m_Bezier;
        Connecter* m_ConnectedConnecter;
        he::eventCallback0<void> m_ConnectionMovedCallback;

        //Disable default copy constructor and default assignment operator
        Connecter(const Connecter&);
        Connecter& operator=(const Connecter&);
    };
public:
    MaterialGeneratorNode(const vec2& pos);
    virtual ~MaterialGeneratorNode();

    virtual bool evaluate(MaterialGeneratorError& error);

    virtual bool canConnect(const MaterialGeneratorNodeOutput& fromOutput, 
                            const MaterialGeneratorNodeInput& toInput, MaterialGeneratorError& error) const;

    bool pick(const vec2& worldPos) const;
    bool doHoover(const vec2& worldPos, const bool undoHoover);
    void setSelected(bool selected) { m_IsSelected = selected; }
    bool isSelected() const { return m_IsSelected; }

    void setPosition(const vec2& position);
    const vec2& getPosition() const { return m_Position; }

    const Guid& getGuid() const { return m_Guid; }
    const he::PrimitiveList<Connecter*>& getConnecters() const { return m_Connecters; }

    void draw2D(gfx::Canvas2D* const canvas, const mat33& transform, const RectF& clipRect);

protected:
    void addOverload(uint8 outputCount, uint8 inputCount, ...);   // Takes MaterialGeneratorVariableType's
    void addConnecters(uint8 outputCount, uint8 inputCount, ...); // Takes ConnecterDesc's
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

    vec2 m_Position;
    vec2 m_Size;
    bool m_IsSelected;
    bool m_IsHoovering;
    Guid m_Guid;

    void activateOverload(uint8 overload);
    bool findOverload(uint8& outOverload) const;

    void updateConnecterPositions();

    he::PrimitiveList<Connecter*> m_Connecters;
    he::PrimitiveList<gui::Sprite*> m_Sprites;
    
    //Disable default copy constructor and default assignment operator
    MaterialGeneratorNode(const MaterialGeneratorNode&);
    MaterialGeneratorNode& operator=(const MaterialGeneratorNode&);
};

} } //end namespace

#endif
