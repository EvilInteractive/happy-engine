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
//Created: 23/11/2012

#ifndef _HE_MaterialGeneratorGraph_H_
#define _HE_MaterialGeneratorGraph_H_
#pragma once

#include "ITickable.h"
#include "IDrawable2D.h"
#include "MaterialGeneratorNodeInOut.h"
#include "NodeGraph.h"

namespace he {
namespace gfx {
    class Window;
    class View;
    class Renderer2D;
}
namespace ct {
    class ShaderGenerator;
}
namespace tools {
class MaterialGeneratorNode;

class MaterialGeneratorGraph : public ge::ITickable, public gfx::IDrawable2D
{
    enum State
    {
        State_Idle,
        State_Pan,
        State_MoveNode
    };
public:
    MaterialGeneratorGraph();
    virtual ~MaterialGeneratorGraph();

    ct::ShaderGenerator* getGenerator() const { return m_Generator; }

    void init();

    void open();
    void close();

    virtual void tick(float dTime);
    virtual void draw2D(gfx::Canvas2D* canvas);

    bool isOpen() const;

private:
    vec2 mouseToWorldPos(const vec2& mousePos) const;

    ct::ShaderGenerator* m_Generator;
    
    NodeGraph<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput> m_NodeGraph;
    he::ObjectList<MaterialGeneratorNode*> m_NodeList;
    he::ObjectList<NodeGraphError<MaterialGeneratorNodeInput, MaterialGeneratorNodeOutput>> m_ErrorList;

    gfx::Window* m_Window;
    gfx::View* m_View;

    gfx::Renderer2D* m_Renderer;

    State m_State;
    vec2 m_StartDragOffset;
    vec2 m_StartDragMousePos;

    vec2 m_Offset;
    float m_Scale;
    bool m_IsActive;

    //Disable default copy constructor and default assignment operator
    MaterialGeneratorGraph(const MaterialGeneratorGraph&);
    MaterialGeneratorGraph& operator=(const MaterialGeneratorGraph&);
};

} } //end namespace

#endif
