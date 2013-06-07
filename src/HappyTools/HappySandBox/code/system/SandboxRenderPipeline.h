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
//Created: 10/11/2012

#ifndef _HE_SandboxRenderPipeline_H_
#define _HE_SandboxRenderPipeline_H_
#pragma once

#include <Renderer2D.h>

namespace he {
namespace gfx {
    class Renderer2D;
    class ShapeRenderer;
    class View;
} }
namespace hs {

class UIRenderer : public he::gfx::Renderer2D
{
public:
    he::uint8 getRenderPriority() const { return 100; }
};
class DebugRenderer : public he::gfx::Renderer2D
{
public:
    he::uint8 getRenderPriority() const { return 50; }
};

class SandboxRenderPipeline
{
public:
    SandboxRenderPipeline();
    virtual ~SandboxRenderPipeline();

    void init(he::gfx::View* const view);

    he::gfx::ShapeRenderer* getShapeRenderer() const { return m_ShapeRenderer; }
    UIRenderer* getUIRenderer() const { return m_UIRenderer; }
    DebugRenderer* getDebugRenderer() const { return m_DebugRenderer; }

private:
    UIRenderer* m_UIRenderer;
    DebugRenderer* m_DebugRenderer;
    he::gfx::ShapeRenderer* m_ShapeRenderer;
    
    //Disable default copy constructor and default assignment operator
    SandboxRenderPipeline(const SandboxRenderPipeline&);
    SandboxRenderPipeline& operator=(const SandboxRenderPipeline&);
};

} //end namespace

#endif
