//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "VertexLayout.h"
#include "IRenderer.h"

namespace he {
namespace gfx {

class Texture2D;
class ModelMesh;
class ShapeMesh;
class Mesh2D;
class BillboardEffect;
class SimpleColorEffect;
class View;
class RenderTarget;
class IShapeDrawable;

class HAPPY_ENTRY ShapeRenderer : public IRenderer
{
public:

    /* CONSTRUCTOR - DESCTRUCTOR */
    ShapeRenderer();
    virtual ~ShapeRenderer();

    /* GENERAL */
    virtual void init(View* view, const RenderTarget* target);
    virtual RenderPass getRenderPass() const { return RenderPass_AfterPost; }
    virtual uint8 getRenderPriority() const { return 50; }
    virtual void preRender() {}
    virtual void render();

    const View* getView() const { return m_View; }

    void attachToRenderer(IShapeDrawable* drawable);
    void detachFromRenderer(IShapeDrawable* drawable);

    /* DRAW METHODS */
    void drawAABB(const vec3& position, const vec3& dimensions, const Color& color) const;
    void drawLine(const vec3& p0, const vec3& p1, const Color& color) const;
    void drawShape(const ShapeMesh* shape, const mat44& world, const Color& color) const;
    void drawBillboard(const Texture2D* tex2D, const vec3& pos);

private:
    void createBillboardQuad();
    void createAABB();
    void createLine();

    /* DATAMEMBERS */
    
    ModelMesh* m_BillboardQuad;
    ModelMesh* m_AABB;
    ShapeMesh* m_LineShape;

    BillboardEffect* m_BillboardEffect;
    SimpleColorEffect* m_AABBEffect;
    SimpleColorEffect* m_ShapeEffect;

    View* m_View;
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
