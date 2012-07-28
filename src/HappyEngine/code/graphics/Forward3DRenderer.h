//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 18/12/2011

#ifndef _HE_FORWARD_3D_RENDERER_H_
#define _HE_FORWARD_3D_RENDERER_H_
#pragma once

#include "IRenderer.h"

namespace he {
namespace gfx {

class Shader;
class ModelMesh;
class Texture2D;
class RenderTarget;

class Forward3DRenderer : public IRenderer
{
public:

    /* CONSTRUCTOR - DESCTRUCTOR */
    Forward3DRenderer();
    virtual ~Forward3DRenderer();

    /* GENERAL */
    virtual void init( View* view, const RenderTarget* target, DrawListContainer::BlendFilter blend);

    virtual void draw();

private:    
    const RenderTarget* m_RenderTarget;
    View* m_View;
    DrawListContainer::BlendFilter m_BlendFilter;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Forward3DRenderer(const Forward3DRenderer&);
    Forward3DRenderer& operator=(const Forward3DRenderer&);
};

} } //end namespace

#endif
