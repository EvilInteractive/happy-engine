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

#include "HappyPCH.h" 

#include "ExtraForward3DRenderer.h"
#include "OpenGL.h"
#include "HeAssert.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "DrawManager.h"
#include "Happy2DRenderer.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
ExtraForward3DRenderer::ExtraForward3DRenderer() :	m_pColorEffect(NEW SimpleColorEffect()),
                                                    m_RenderFboID(UINT_MAX),
                                                    m_pRenderTexture(NEW Texture2D()),
                                                    m_pBillboardEffect(NEW BillboardEffect()),
                                                    m_pBillboardQuad(NEW ModelMesh("")),
                                                    m_ScreenDimensions(0,0)
{
}

ExtraForward3DRenderer::~ExtraForward3DRenderer()
{
    delete m_pColorEffect;
    delete m_pBillboardEffect;
}

void ExtraForward3DRenderer::createBillboardQuad()
{
    m_VertexLayoutBillboard.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));
    m_VertexLayoutBillboard.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 12));

    m_pBillboardQuad->init();

    std::vector<VertexPosTex> vertices;
    vertices.push_back(
        VertexPosTex(vec3(-0.5f, 0.5f,0.0f),
        vec2(0, 1)));

    vertices.push_back(
        VertexPosTex(vec3(0.5f, 0.5f,0.0f),
        vec2(1, 1)));

    vertices.push_back(
        VertexPosTex(vec3(-0.5f, -0.5f,0.0f),
        vec2(0, 0)));

    vertices.push_back(
        VertexPosTex(vec3(0.5f, -0.5f,0.0f),
        vec2(1, 0)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    m_pBillboardQuad->setVertices(&vertices[0], 4, m_VertexLayoutBillboard);
    m_pBillboardQuad->setIndices(&indices[0], 6, IndexStride_Byte);
}

/* GENERAL */
void ExtraForward3DRenderer::init()
{
    GUI->createLayer("forward", 99);

    createBillboardQuad();

    m_pColorEffect->load();
    m_pBillboardEffect->load();

    vec2 screenDim(static_cast<float>(GRAPHICS->getScreenWidth()), static_cast<float>(GRAPHICS->getScreenHeight()));
    m_ScreenDimensions = screenDim;

    resize();    
}

void ExtraForward3DRenderer::begin(const Camera* pCamera)
{
    vec2 screenDim(static_cast<float>(GRAPHICS->getScreenWidth()), static_cast<float>(GRAPHICS->getScreenHeight()));
    if (m_ScreenDimensions != screenDim)
    {
        m_ScreenDimensions = screenDim;
        resize();
    }

    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heSetDepthWrite(true);	

    GL::heBindFbo(m_RenderFboID);
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    glClear(GL_COLOR_BUFFER_BIT);

    m_ViewProjection = pCamera->getViewProjection();
    m_BillboardMatrix = mat44::createBillboard(pCamera);

    m_pCamera = pCamera;

    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
}

void ExtraForward3DRenderer::end()
{
    GL::heBindFbo(0);
    GL::heBlendEnabled(false);
    GUI->setLayer("forward");
    GUI->drawTexture2D(m_pRenderTexture, vec2(0.0f,0.0f), vec2(-static_cast<float>(m_pRenderTexture->getWidth()), static_cast<float>(m_pRenderTexture->getHeight())));
    GUI->setLayer();
}

void ExtraForward3DRenderer::resize()
{
    uint renderTexture;
    glGenTextures(1, &renderTexture);

    GL::heBindTexture2D(0, renderTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(m_ScreenDimensions.x), static_cast<GLsizei>(m_ScreenDimensions.y), 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    m_pRenderTexture->init(renderTexture, static_cast<uint>(m_ScreenDimensions.x), static_cast<uint>(m_ScreenDimensions.y), GL_RGBA8);

    if (m_RenderFboID != UINT_MAX)
        glDeleteFramebuffers(1, &m_RenderFboID);

    glGenFramebuffers(1, &m_RenderFboID);
    GL::heBindFbo(m_RenderFboID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GRAPHICS->getDrawManager()->getDepthID(), 0);
}

/* DRAW METHODS */
void ExtraForward3DRenderer::drawColored(const ModelMesh::pointer& model, const mat44& world, const Color& color) const
{
    m_pColorEffect->begin();
    m_pColorEffect->setViewProjection(m_ViewProjection);
    m_pColorEffect->setWorld(world);
    m_pColorEffect->setColor(color);

    GL::heBindVao(model->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, model->getNumIndices(), model->getIndexType(), 0);
}

void ExtraForward3DRenderer::drawColoredNoDepth(const ModelMesh::pointer& model, const mat44& world, const Color& color) const
{
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    m_pColorEffect->begin();
    m_pColorEffect->setViewProjection(m_ViewProjection);
    m_pColorEffect->setWorld(world);
    m_pColorEffect->setColor(color);

    GL::heBindVao(model->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, model->getNumIndices(), model->getIndexType(), 0);

    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
}

void ExtraForward3DRenderer::drawSpline(const ModelMesh::pointer& spline, const mat44& world, const Color& color) const
{
    m_pColorEffect->begin();
    m_pColorEffect->setViewProjection(m_ViewProjection);
    m_pColorEffect->setWorld(world);
    m_pColorEffect->setColor(color);

    GL::heBindVao(spline->getVertexArraysID());
    glDrawElements(GL_LINES, spline->getNumIndices(), spline->getIndexType(), 0);
}

void ExtraForward3DRenderer::drawBillboard(const Texture2D::pointer& tex2D, const vec3& pos)
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

    float a1 = acosf(dot(vec3::forward, m_pCamera->getLook()));
    float a2 = acosf(dot(vec3::up, m_pCamera->getUp()));
    float a3 = acosf(dot(vec3::right, m_pCamera->getRight()));

    mat44 world(    //mat44::createScale(size.x / 100.0f, size.y / 100.0f, 1.0f) * 
                    /*mat44::createRotation(vec3::right, a3) * mat44::createRotation(vec3::up, a2) * mat44::createRotation(vec3::forward, a1) **/
                    mat44::createTranslation(pos));

    //vec3 translation(pos.x + size.x/2, pos.y + size.y/2), pos;

    /*world(0,0) = 1.0f;
    world(0,1) = 0.0f;
    world(0,2) = 0.0f;
    world(1,0) = 0.0f;
    world(1,1) = 1.0f;
    world(1,2) = 0.0f;
    world(2,0) = 0.0f;
    world(2,1) = 0.0f;
    world(2,2) = 1.0f;*/

    m_pBillboardEffect->begin();

    m_pBillboardEffect->setWorldViewProjection(m_ViewProjection * world * m_BillboardMatrix);

    m_pBillboardEffect->setDiffuseMap(tex2D);
    m_pBillboardEffect->setTCScale(tcScale);

    GL::heBindVao(m_pBillboardQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pBillboardQuad->getNumIndices(), m_pBillboardQuad->getIndexType(), 0);
}

} } //end namespace