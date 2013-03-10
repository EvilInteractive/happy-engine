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

#ifndef _HE_DefaultRenderPipeline_H_
#define _HE_DefaultRenderPipeline_H_
#pragma once

namespace he {
namespace gfx {
    class IRenderer;
    class Forward3DRenderer;
    class Renderer2D;
    class ShapeRenderer;
    class Scene;
    class Picker;
    struct RenderSettings;
}
namespace ge {

class HAPPY_ENTRY DefaultRenderPipeline
{
public:
    DefaultRenderPipeline();
    virtual ~DefaultRenderPipeline();

    void init(gfx::View* const view, gfx::Scene* const scene, const gfx::RenderSettings& settings);

    gfx::IRenderer* getOpacRenderer() const { return m_OpacRenderer; }
    gfx::Forward3DRenderer* getTransparentRenderer() const { return m_TransparentRenderer; }
    gfx::ShapeRenderer* getShapeRenderer() const { return m_ShapeRenderer; }
    gfx::Renderer2D* get2DRenderer() const { return m_2DRenderer; }
    gfx::Picker* getPicker() const { return m_Picker; }

private:
    gfx::IRenderer* m_OpacRenderer;
    gfx::Forward3DRenderer* m_TransparentRenderer;
    gfx::ShapeRenderer* m_ShapeRenderer;
    gfx::Renderer2D* m_2DRenderer;
    gfx::Picker* m_Picker;

    bool m_IsDeferred;

    //Disable default copy constructor and default assignment operator
    DefaultRenderPipeline(const DefaultRenderPipeline&);
    DefaultRenderPipeline& operator=(const DefaultRenderPipeline&);
};

} } //end namespace

#endif
