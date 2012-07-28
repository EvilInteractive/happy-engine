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
//Author:  Sebastiaan Sprengers
//Created: 06/11/2011

#ifndef _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#define _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#pragma once

#include "BufferLayout.h"
#include "IRenderer.h"

namespace he {
namespace gfx {

class Texture2D;
class ModelMesh;
class CameraPerspective;
class BillboardEffect;
class SimpleColorEffect;
class ModelMesh;

// TODO: seeb
// Is this still used?
// if so consider moving it or renaming to ShapeRenderer
// it also needs to be attached to a view
// since it is a renderer it should inherit IRenderer and can then be used in a View
class ExtraForward3DRenderer
{
public:

    /* CONSTRUCTOR - DESCTRUCTOR */
    ExtraForward3DRenderer();
    virtual ~ExtraForward3DRenderer();

    /* GENERAL */
    void init();
    void begin(const CameraPerspective* pCamera);
    void end();
    void resize();

    /* DRAW METHODS */
    void drawColored(const ModelMesh* model, const mat44& world, const Color& color) const;
    void drawColoredNoDepth(const ModelMesh* model, const mat44& world, const Color& color) const;
    void drawSpline(const ModelMesh* spline, const mat44& world, const Color& color) const;
    void drawBillboard(const Texture2D* tex2D, const vec3& pos);

private:

    void createBillboardQuad();

    /* DATAMEMBERS */
    SimpleColorEffect* m_pColorEffect;

    mat44 m_ViewProjection;
    mat44 m_BillboardMatrix;

    uint m_RenderFboID;
    Texture2D* m_pRenderTexture;

    ModelMesh* m_pBillboardQuad;
    BufferLayout m_VertexLayoutBillboard;
    BillboardEffect* m_pBillboardEffect;

    vec2 m_ScreenDimensions;

    const CameraPerspective* m_pCamera;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    ExtraForward3DRenderer(const ExtraForward3DRenderer&);
    ExtraForward3DRenderer& operator=(const ExtraForward3DRenderer&);
};

} } //end namespace

#endif
