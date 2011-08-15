//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 13/08/2011

#ifndef _DEFERRED_3D_RENDERER_H_
#define _DEFERRED_3D_RENDERER_H_
#pragma once

#include "I3DRenderer.h"
#include "HappyTypes.h"
#include "Shader.h"
#include "Model.h"
#include "Texture2D.h"

namespace happyengine {
namespace graphics {

class Deferred3DRenderer : public I3DRenderer
{
public:
	Deferred3DRenderer();
    virtual ~Deferred3DRenderer();
        
    virtual void draw(const Model::pointer& pModel);

    void begin();
    void end();

private:
    uint m_FboId;

    // Textures:
    //      - Color.rgb, ill       GL_RGBA8
    //      - Position.xyz, spec   GL_RGBA16
    //      - Normal.xyz, gloss    GL_RGBA16
    //      - Depth                GL_DEPTH_COMPONENT32
    static const int TEXTURES = 4;
    static const int TEXTURE_FORMAT[TEXTURES];
    static const int TEXTURE_INTERNALFORMAT[TEXTURES];
    static const int TEXTURE_ATTACHMENT[TEXTURES];
    uint m_TextureId[TEXTURES];
    uint m_DepthBufferId;

    Shader* m_pPostShader;
    uint m_ShaderColIllMapPos, m_ShaderPosSpecMapPos, m_ShaderNormGlossMapPos;

    Model::pointer m_pModel;

    Texture2D::pointer m_pTexture[TEXTURES];

    //Disable default copy constructor and default assignment operator
    Deferred3DRenderer(const Deferred3DRenderer&);
    Deferred3DRenderer& operator=(const Deferred3DRenderer&);
};

} } //end namespace

#endif
