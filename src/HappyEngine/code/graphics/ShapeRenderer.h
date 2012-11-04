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
// Bastian Damman: changed name from ExtraForward3DRenderer to ShapeRenderer

#ifndef _HE_SHAPE_RENDERER_H_
#define _HE_SHAPE_RENDERER_H_
#pragma once

#include "BufferLayout.h"
#include "IRenderer.h"

namespace he {
namespace gfx {

class Texture2D;
class ModelMesh;
class BillboardEffect;
class SimpleColorEffect;
class View;
class RenderTarget;
class IShapeDrawable;

class ShapeRenderer : public IRenderer3D
{
public:

    /* CONSTRUCTOR - DESCTRUCTOR */
    ShapeRenderer();
    virtual ~ShapeRenderer();

    /* GENERAL */
    virtual void init(View3D* view, const RenderTarget* target);
    virtual void draw();

    void attachToRenderer(IShapeDrawable* drawable);
    void detachFromRenderer(IShapeDrawable* drawable);

    /* DRAW METHODS */
    void drawColored(const ModelMesh* model, const mat44& world, const Color& color) const;
    void drawColoredNoDepth(const ModelMesh* model, const mat44& world, const Color& color) const;
    void drawMeshColor(const ModelMesh* spline, const mat44& world, const Color& color) const;
    void drawBillboard(const Texture2D* tex2D, const vec3& pos);

private:

    void createBillboardQuad();

    /* DATAMEMBERS */
    SimpleColorEffect* m_pColorEffect;
    
    ModelMesh* m_pBillboardQuad;
    BufferLayout m_VertexLayoutBillboard;
    BillboardEffect* m_pBillboardEffect;

    View3D* m_View;
    const RenderTarget* m_RenderTarget;

    mat44 m_BillboardMatrix;
    mat44 m_ViewProjection;

    he::PrimitiveList<IShapeDrawable*> m_Drawables;
    
    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    ShapeRenderer(const ShapeRenderer&);
    ShapeRenderer& operator=(const ShapeRenderer&);
};

} } //end namespace

#endif
