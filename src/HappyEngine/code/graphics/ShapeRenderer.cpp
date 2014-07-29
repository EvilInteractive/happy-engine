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

#include "HappyPCH.h" 
#include "ShapeRenderer.h"

#include "BillboardEffect.h"
#include "DrawContext.h"
#include "ICamera.h"
#include "IShapeDrawable.h"
#include "ModelMesh.h"
#include "RenderTarget.h"
#include "ShapeMesh.h"
#include "SimpleColorEffect.h"
#include "Vertex.h"
#include "VertexLayout.h"
#include "View.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
ShapeRenderer::ShapeRenderer() :	
    m_ShapeEffect(NEW SimpleColorEffect()),
    m_AABBEffect(NEW SimpleColorEffect()),
    m_BillboardEffect(NEW BillboardEffect()),
    m_BillboardQuad(nullptr),
    m_AABB(nullptr),
    m_View(nullptr),
    m_RenderTarget(nullptr)
{
}

ShapeRenderer::~ShapeRenderer()
{
    delete m_ShapeEffect;
    delete m_AABBEffect;
    delete m_BillboardEffect;
    if (m_BillboardQuad != nullptr)
        m_BillboardQuad->release();
    delete m_AABB;
}

void ShapeRenderer::createBillboardQuad()
{
    VertexLayout vertexLayoutBillboard;
    vertexLayoutBillboard.addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_3, 0));
    vertexLayoutBillboard.addElement(VertexElement(eShaderAttribute_TextureCoordiante, eShaderAttributeType_Float, eShaderAttributeTypeComponents_2, 12));

    m_BillboardQuad = ResourceFactory<ModelMesh>::getInstance()->get(ResourceFactory<ModelMesh>::getInstance()->create());
    m_BillboardQuad->init(vertexLayoutBillboard, gfx::MeshDrawMode_Triangles);

    he::ObjectList<VertexPosTex> vertices(4);
    vertices.add(
        VertexPosTex(vec3(-0.5f, 0.5f,0.0f),
        vec2(0, 1)));

    vertices.add(
        VertexPosTex(vec3(0.5f, 0.5f,0.0f),
        vec2(1, 1)));

    vertices.add(
        VertexPosTex(vec3(-0.5f, -0.5f,0.0f),
        vec2(0, 0)));

    vertices.add(
        VertexPosTex(vec3(0.5f, -0.5f,0.0f),
        vec2(1, 0)));

    he::PrimitiveList<uint8> indices(6);
    indices.add(0); indices.add(1); indices.add(2);
    indices.add(1); indices.add(3); indices.add(2);

    m_BillboardQuad->setVertices(&vertices[0], 4, gfx::MeshUsage_Static, true);
    m_BillboardQuad->setIndices(&indices[0], 6, IndexStride_Byte, gfx::MeshUsage_Static);
    m_BillboardQuad->setName("ShapeRenderer-BillboardQuad");
    m_BillboardQuad->setLoaded(eLoadResult_Success);
}

void ShapeRenderer::createAABB()
{
    VertexLayout vertexLayoutBillboard;
    vertexLayoutBillboard.addElement(VertexElement(eShaderAttribute_Position, eShaderAttributeType_Float, eShaderAttributeTypeComponents_3, 0));

    m_AABB = ResourceFactory<ModelMesh>::getInstance()->get(ResourceFactory<ModelMesh>::getInstance()->create());
    m_AABB->init(vertexLayoutBillboard, gfx::MeshDrawMode_Lines);

    vec3 vertices[8];
    vertices[0] = vec3(-0.5f,  0.5f, 0.5f);
    vertices[1] = vec3( 0.5f,  0.5f, 0.5f);
    vertices[2] = vec3(-0.5f, -0.5f, 0.5f);
    vertices[3] = vec3( 0.5f, -0.5f, 0.5f);

    vertices[4] = vec3(-0.5f,  0.5f, -0.5f);
    vertices[5] = vec3( 0.5f,  0.5f, -0.5f);
    vertices[6] = vec3(-0.5f, -0.5f, -0.5f);
    vertices[7] = vec3( 0.5f, -0.5f, -0.5f);

    uint8 indices[24];
    indices[0] = 0; indices[1] = 1; 
    indices[2] = 0; indices[3] = 2;
    indices[4] = 1; indices[5] = 3;
    indices[6] = 2; indices[7] = 3;

    indices[8] = 4; indices[9] = 5; 
    indices[10] = 4; indices[11] = 6;
    indices[12] = 5; indices[13] = 7;
    indices[14] = 6; indices[15] = 7;

    indices[16] = 0; indices[17] = 4; 
    indices[18] = 1; indices[19] = 5;
    indices[20] = 2; indices[21] = 6;
    indices[22] = 3; indices[23] = 7;

    m_AABB->setVertices(vertices, 8, gfx::MeshUsage_Static, true);
    m_AABB->setIndices(indices, 24, IndexStride_Byte, gfx::MeshUsage_Static);
    m_AABB->setName("ShapeRenderer-AABB");
    m_AABB->setLoaded(eLoadResult_Success);
}

/* GENERAL */
void ShapeRenderer::init(View* view, const RenderTarget* target)
{
    m_View = view;
    m_RenderTarget = target;

    createBillboardQuad();
    createAABB();

    m_ShapeEffect->init(ShapeMesh::getVertexLayout());
    m_AABBEffect->init(m_AABB->getVertexLayout());
    m_BillboardEffect->init(m_BillboardQuad->getVertexLayout());
}

/* DRAW METHODS */

void ShapeRenderer::drawAABB( const vec3& position, const vec3& dimensions, const Color& color ) const
{
    m_AABBEffect->setWorld(mat44::createTranslation(position) * mat44::createScale(dimensions));
    m_AABBEffect->setColor(color);
    m_AABBEffect->setViewProjection(m_ViewProjection);

    DrawContext context;
    context.m_VBO = m_AABB->getVBO();
    context.m_IBO = m_AABB->getIBO();
    m_AABBEffect->apply(context);
    m_AABB->draw();
}

void ShapeRenderer::drawShape( const ShapeMesh* shape, const mat44& world, const Color& color ) const
{
    m_ShapeEffect->setWorld(world);
    m_ShapeEffect->setColor(color);
    m_ShapeEffect->setViewProjection(m_ViewProjection);

    DrawContext context;
    context.m_VBO = shape->getVBO();
    context.m_IBO = shape->getIBO();
    m_ShapeEffect->apply(context);
    shape->draw();
}

void ShapeRenderer::drawBillboard(const Texture2D* tex2D, const vec3& pos)
{
    vec2 tcScale(1.0f,1.0f);
    mat44 world(mat44::createTranslation(pos));
    
    m_BillboardEffect->setWorldViewProjection(m_ViewProjection * world * m_BillboardMatrix);
    m_BillboardEffect->setDiffuseMap(tex2D);
    m_BillboardEffect->setTCScale(tcScale);

    DrawContext context;
    context.m_VBO = m_BillboardQuad->getVBO();
    context.m_IBO = m_BillboardQuad->getIBO();
    m_BillboardEffect->apply(context);
    m_BillboardQuad->draw();
}

void ShapeRenderer::render()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heSetDepthWrite(true);	
    GL::heSetDepthRead(true);

    m_RenderTarget->prepareForRendering(1);

    const gfx::ICamera* const cam(m_View->getCamera());
    if (cam != nullptr)
    {
        m_ViewProjection = cam->getViewProjection();
        m_BillboardMatrix = mat44::createBillboard(cam);

        std::for_each(m_Drawables.cbegin(), m_Drawables.cend(), [this](IShapeDrawable* drawable)
        {
            drawable->drawShapes(this);
        });
    }
}

void ShapeRenderer::attachToRenderer( IShapeDrawable* drawable )
{
    m_Drawables.add(drawable);
}

void ShapeRenderer::detachFromRenderer( IShapeDrawable* drawable )
{
    HE_IF_ASSERT(m_Drawables.contains(drawable), "drawable not found in draw list")
    {
        m_Drawables.remove(drawable);
    }
}

} } //end namespace
