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
#include "ModelMesh.h"

namespace he {
namespace gfx {

class Shader;

class Forward3DRenderer : public IRenderer
{
public:

    /* CONSTRUCTOR - DESCTRUCTOR */
    Forward3DRenderer();
    virtual ~Forward3DRenderer();

    /* GENERAL */
    virtual void init(const RenderSettings& settings, 
        const Texture2D* pOutTarget, const Texture2D* pOutNormalTarget, const Texture2D* pOutDepthTarget);

    virtual void setRenderSettings(const RenderSettings& settings);
    virtual void onScreenResized();

    virtual void draw(const DrawListContainer& drawList, uint renderFlags);

    virtual void clear(bool color, bool normal, bool depth);

    virtual bool getSupportsTranslucency() const { return true; }

private:
    void compileShaders();
    void initFbo();

    uint m_FboId;
    uint m_DrawBuffers[2];

    const Texture2D* m_pOutColorTexture;
    bool m_OwnsColorBuffer;
    const Texture2D* m_pOutNormalTexture;
    const Texture2D* m_pOutDepthTexture;

    ModelMesh::pointer m_pQuad;

    Shader* m_pQuadShader;
    uint m_QuadShaderTexPos;

    RenderSettings m_Settings;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Forward3DRenderer(const Forward3DRenderer&);
    Forward3DRenderer& operator=(const Forward3DRenderer&);
};

} } //end namespace

#endif
