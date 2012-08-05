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
//Created: 19/03/2012

#include "HappyPCH.h" 

#include "Renderer2D.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Renderer2D.h"
#include "ControlsManager.h"
#include "IMouse.h"
#include "IKeyboard.h"
#include "Vertex.h"

namespace he {
namespace gfx {

Renderer2D::Renderer2D() :  m_WebCore(nullptr),
                            m_TextureEffect(NEW Simple2DTextureEffect()),
                            m_TextureQuad(nullptr)
{
}

Renderer2D::~Renderer2D()
{
    if (m_WebCore != nullptr)
    {
        Awesomium::WebCore::Shutdown();
    }

    delete m_TextureEffect;
    m_TextureQuad->release();
}

/* GENERAL */
Canvas2D* Renderer2D::createCanvas(const vec2& size)
{
    vec2 dim = size;;

    if (size.x <= 0 || size.y <= 0)
    {
        dim.x = (float)GRAPHICS->getActiveView()->getScreenWidth();
        dim.y = (float)GRAPHICS->getActiveView()->getScreenHeight();
    }

    Canvas2D::Data* data = Canvas2D::create(dim);

    if (data == nullptr)
    {
        HE_ERROR("Failed to create Canvas2D!");
        return nullptr;
    }

    return NEW Canvas2D(data, dim);
}

WebView* Renderer2D::createWebView(bool enableUserInput, const vec2& size)
{
    if (m_WebCore == nullptr)
    {
        m_WebCore = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
    }

    vec2 dim = size;
    bool fullscreen = false;

    if (size == vec2(0,0)) // TODO: do not allow this, pass bool if fullscreen or register on a Resized event of the View
    {
        dim.x = (float)GRAPHICS->getScreenWidth();
        dim.y = (float)GRAPHICS->getScreenHeight();

        fullscreen = true;
    }

    Awesomium::WebView* view = m_WebCore->CreateWebView((int)dim.x, (int)dim.y); // TODO: put this in WebView, keep ownership of pointers simple
    WebView* web = NEW WebView(view, enableUserInput, fullscreen);
    m_WebViews.push_back(web); // TODO: nothing is done with m_WebViews? unsafe because ownership of the pointer is passed to the client

    return web;
}

void Renderer2D::tick()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    if (m_WebCore != nullptr)
    {
        m_WebCore->Update();
    }
}

void Renderer2D::draw()
{
}

void Renderer2D::init()
{
    m_TextureEffect->load();

    BufferLayout vLayout;
    vLayout.addElement(BufferElement(0, BufferElement::Type_Vec2, BufferElement::Usage_Position, 8, 0));
    vLayout.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 8));

    // model texturequad
    m_TextureQuad = ResourceFactory<ModelMesh>::getInstance()->get(ResourceFactory<ModelMesh>::getInstance()->create());
    m_TextureQuad->init();

    std::vector<VertexPosTex2D> vertices;
    vertices.push_back(
        VertexPosTex2D(vec2(-0.5f, 0.5f),
        vec2(0, 0)));

    vertices.push_back(
        VertexPosTex2D(vec2(0.5f, 0.5f),
        vec2(1, 0)));

    vertices.push_back(
        VertexPosTex2D(vec2(-0.5f, -0.5f),
        vec2(0, 1)));

    vertices.push_back(
        VertexPosTex2D(vec2(0.5f, -0.5f),
        vec2(1, 1)));

    std::vector<byte> indices;
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);

    m_TextureQuad->setVertices(&vertices[0], 4, vLayout);
    m_TextureQuad->setIndices(&indices[0], 6, IndexStride_Byte);
}

void Renderer2D::drawTexture2DToScreen( const Texture2D* tex2D, const vec2& pos,
                                        bool useBlending, const vec2& newDimensions,
                                        const RectF& regionToDraw)
{
    vec2 tcOffset(0.0f,0.0f);
    vec2 tcScale(1.0f,1.0f);
    vec2 size;

    if (regionToDraw != RectF(vec2(0.0f,0.0f), vec2(0.0f,0.0f)))
    {
        tcScale.x = regionToDraw.width / tex2D->getWidth();
        tcScale.y = regionToDraw.height / tex2D->getHeight();

        tcOffset.x = regionToDraw.x / tex2D->getWidth();
        tcOffset.y = 1 - (regionToDraw.y / tex2D->getHeight()) - tcScale.y;
    }

    if (newDimensions != vec2(0.0f,0.0f))
    {
        size = vec2(abs(newDimensions.x), abs(newDimensions.y));

        if (newDimensions.x < 0) tcScale.x *= -1.0f;
        if (newDimensions.y < 0) tcScale.y *= -1.0f;
    }
    else
    {
        size = vec2(static_cast<float>(tex2D->getWidth()), static_cast<float>(tex2D->getHeight()));
    }

    mat44 world(mat44::createTranslation(vec3(pos.x + size.x/2, pos.y + size.y/2, 0.0f)) * mat44::createScale(size.x, size.y, 1.0f));
    mat44 orthographicMatrix = mat44::createOrthoLH(0.0f, (float)GRAPHICS->getScreenWidth(), 0.0f, (float)GRAPHICS->getScreenHeight(), 0.0f, 100.0f);

    m_TextureEffect->begin();
    m_TextureEffect->setWorldMatrix(orthographicMatrix * world);
    m_TextureEffect->setDiffuseMap(tex2D);
    m_TextureEffect->setAlpha(1.0f);
    m_TextureEffect->setTCOffset(tcOffset);
    m_TextureEffect->setTCScale(tcScale);
    m_TextureEffect->setDepth(0.0f);

    GL::heSetDepthFunc(DepthFunc_Always);

    if (useBlending == true)
    {
        GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
        GL::heBlendEquation(BlendEquation_Add);
        GL::heBlendEnabled(true);
    }
    else
    {
        GL::heBlendEnabled(false);
    }

    GL::heBindFbo(0);

    GL::heBindVao(m_TextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
}

} } //end namespace