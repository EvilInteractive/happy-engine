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
//Author:  Sebastiaan Sprengers
//Created: 06/11/2011

#include "HappyPCH.h" 

#include "SimpleForward3DRenderer.h"
#include "OpenGL.h"
#include "HeAssert.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "Happy2DRenderer.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
SimpleForward3DRenderer::SimpleForward3DRenderer() :	m_pColorEffect(NEW SimpleColorEffect()),
                                                        m_RenderFboID(0),
                                                        m_pRenderTexture(NEW Texture2D()),
                                                        m_pBillboardEffect(NEW BillboardEffect()),
                                                        m_pBillboardQuad(NEW ModelMesh(""))
{
}

SimpleForward3DRenderer::~SimpleForward3DRenderer()
{
    delete m_pColorEffect;
    delete m_pBillboardEffect;
}

void SimpleForward3DRenderer::createBillboardQuad()
{
    m_VertexLayoutBillboard.addElement(VertexElement(0, VertexElement::Type_Vec3, VertexElement::Usage_Position, 12, 0));
    m_VertexLayoutBillboard.addElement(VertexElement(1, VertexElement::Type_Vec2, VertexElement::Usage_TextureCoordinate, 8, 12));

    m_pBillboardQuad->init();

    std::vector<VertexPosTex> vertices;
    vertices.push_back(
        VertexPosTex(vec3(-0.5f, 0.5f,0.0f),
        vec2(0, 0)));

    vertices.push_back(
        VertexPosTex(vec3(0.5f, 0.5f,0.0f),
        vec2(1, 0)));

    vertices.push_back(
        VertexPosTex(vec3(-0.5f, -0.5f,0.0f),
        vec2(0, 1)));

    vertices.push_back(
        VertexPosTex(vec3(0.5f, -0.5f,0.0f),
        vec2(1, 1)));

    std::vector<byte> indices;
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);

    m_pBillboardQuad->setVertices(&vertices[0], 4, m_VertexLayoutBillboard);
    m_pBillboardQuad->setIndices(&indices[0], 6, IndexStride_Byte);
}

/* GENERAL */
void SimpleForward3DRenderer::init()
{
    createBillboardQuad();

    m_pColorEffect->load();
    m_pBillboardEffect->load();

    int width = GRAPHICS->getViewport().width, 
        height = GRAPHICS->getViewport().height;

    uint renderTexture;
    glGenTextures(1, &renderTexture);

    GL::heBindTexture2D(0, renderTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    m_pRenderTexture->init(renderTexture, width, height, GL_RGBA8);

    glGenFramebuffers(1, &m_RenderFboID);
    GL::heBindFbo(m_RenderFboID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GRAPHICS->getDeferredRenderer()->getDepthTexture()->getID(), 0);
}

void SimpleForward3DRenderer::begin(const Camera* pCamera)
{
    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heSetDepthWrite(true);	

    GL::heBindFbo(m_RenderFboID);
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    glClear(GL_COLOR_BUFFER_BIT);

    m_ViewProjection = pCamera->getViewProjection();
}

void SimpleForward3DRenderer::end()
{
    GL::heBindFbo(0);
    GL::heBlendEnabled(false);

    GUI->drawTexture2D(m_pRenderTexture, vec2(0.0f,0.0f), vec2(-static_cast<float>(m_pRenderTexture->getWidth()), static_cast<float>(m_pRenderTexture->getHeight())));
}

/* DRAW METHODS */
void SimpleForward3DRenderer::drawSpline(const ModelMesh::pointer& spline, const mat44& world, const Color& color) const
{
    m_pColorEffect->begin();
    m_pColorEffect->setViewProjection(m_ViewProjection);
    m_pColorEffect->setWorld(world);
    m_pColorEffect->setColor(color);

    GL::heBindVao(spline->getVertexArraysID());
    glDrawElements(GL_LINES, spline->getNumIndices(), spline->getIndexType(), 0);
}

void SimpleForward3DRenderer::drawBillboard(const Texture2D::pointer& tex2D, const vec3& pos)
{
    vec2 tcScale(1.0f,1.0f);
    vec2 size;

    /*if (newDimensions != vec2(0.0f,0.0f))
    {
        size = vec2(abs(newDimensions.x), abs(newDimensions.y));

        if (newDimensions.x < 0) tcScale.x *= -1.0f;
        if (newDimensions.y < 0) tcScale.y *= -1.0f;
    }
    else
    {*/
        size = vec2(static_cast<float>(tex2D->getWidth()), static_cast<float>(tex2D->getHeight()));
    //}

    mat44 world(mat44::createScale(size.x, size.y, 1.0f) *
                mat44::createTranslation(pos));

    //vec3 translation(pos.x + size.x/2, pos.y + size.y/2), pos;

    m_pBillboardEffect->begin();

    m_pBillboardEffect->setWorldViewProjection(m_ViewProjection * world);

    m_pBillboardEffect->setDiffuseMap(tex2D);
    m_pBillboardEffect->setTCScale(tcScale);

    GL::heBindVao(m_pBillboardQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pBillboardQuad->getNumIndices(), m_pBillboardQuad->getIndexType(), 0);
}

} } //end namespace