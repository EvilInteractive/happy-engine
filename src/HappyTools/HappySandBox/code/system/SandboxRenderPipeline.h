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

namespace he {
namespace gfx {
    class Renderer2D;
    class View;
} }
namespace hs {

class SandboxRenderPipeline
{
public:
    SandboxRenderPipeline();
    virtual ~SandboxRenderPipeline();

    void init(he::gfx::View* const view);

    he::gfx::Renderer2D* getUIRenderer() const { return m_UIRenderer; }
    he::gfx::Renderer2D* getDebugRenderer() const { return m_DebugRenderer; }

private:
    he::gfx::Renderer2D* m_UIRenderer;
    he::gfx::Renderer2D* m_DebugRenderer;
    
    //Disable default copy constructor and default assignment operator
    SandboxRenderPipeline(const SandboxRenderPipeline&);
    SandboxRenderPipeline& operator=(const SandboxRenderPipeline&);
};

} //end namespace

#endif
