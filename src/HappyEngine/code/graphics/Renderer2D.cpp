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
#include "Simple2DTextureEffect.h"
#include "Awesomium/WebCore.h"
#include "ModelMesh.h"
#include "Canvas2D.h"
#include "WebView.h"
#include "RenderTarget.h"
#include "Texture2D.h"
#include "View.h"
#include "I2DDrawable.h"

namespace he {
namespace gfx {

Renderer2D::Renderer2D() :
                            m_TextureEffect(NEW Simple2DTextureEffect()),
                            m_TextureQuad(nullptr),
                            m_View(nullptr),
                            m_RenderTarget(nullptr)
{
}

Renderer2D::~Renderer2D()
{
    delete m_TextureEffect;
    m_TextureQuad->release();
}

/* GENERAL */
Canvas2D* Renderer2D::createCanvasAbsolute(const RectI& viewport)
{
    HE_ASSERT(viewport.width > 0 && viewport.height > 0, "viewport width and height must be > 0");
    Canvas2D* canvas(NEW Canvas2D(viewport));
    m_Canvas2Ds.push_back(canvas);
    return canvas;
}
Canvas2D* Renderer2D::createCanvasRelative(const RectF& percent)
{
    HE_ASSERT(percent.width > 0 && percent.height > 0, "viewport width and height must be > 0");
    Canvas2D* canvas(NEW Canvas2D(m_View, percent));
    m_Canvas2Ds.push_back(canvas);
    return canvas;
}
void Renderer2D::removeCanvas( Canvas2D* canvas )
{
    std::vector<Canvas2D*>::iterator it(std::find(m_Canvas2Ds.begin(), m_Canvas2Ds.end(), canvas));
    HE_IF_ASSERT(it != m_Canvas2Ds.cend(), "Canvas is not a member of this Renderer2D!")
    {
        *it = m_Canvas2Ds.back();
        m_Canvas2Ds.pop_back();
        delete canvas;
    }
}


WebView* Renderer2D::createWebViewAbsolute(const RectI& viewport, bool enableUserInput)
{
    WebView* web(NEW WebView(viewport, enableUserInput));
    m_WebViews.push_back(web);
    return web;
}
WebView* Renderer2D::createWebViewRelative(const RectF& viewportPercent, bool enableUserInput)
{
    WebView* web(NEW WebView(m_View, viewportPercent, enableUserInput));
    m_WebViews.push_back(web);
    return web;
}
void Renderer2D::removeWebView( WebView* webview )
{
    std::vector<WebView*>::iterator it(std::find(m_WebViews.begin(), m_WebViews.end(), webview));
    HE_IF_ASSERT(it != m_WebViews.cend(), "Canvas is not a member of this Renderer2D!")
    {
        *it = m_WebViews.back();
        m_WebViews.pop_back();
        delete webview;
    }
}

void Renderer2D::draw()
{
    m_RenderTarget->prepareForRendering(1);

    std::for_each(m_Drawables.cbegin(), m_Drawables.cend(), [this](I2DDrawable* drawable)
    {
        drawable->draw2D(this);
    });
}

void Renderer2D::init( View* view, const RenderTarget* target )
{
    m_View = view;
    m_RenderTarget = target;

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
    m_TextureQuad->setLoaded();
}

void Renderer2D::drawTexture2DToScreen( const Texture2D* tex2D, const vec2& pos,
                                        bool useBlending, const vec2& newDimensions,
                                        const RectF& regionToDraw)
{
    m_RenderTarget->prepareForRendering();

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
    mat44 orthographicMatrix = mat44::createOrthoLH(0.0f, 
        static_cast<float>(m_View->getViewport().width), 0.0f, static_cast<float>(m_View->getViewport().height), 0.0f, 100.0f);

    m_TextureEffect->begin();
    m_TextureEffect->setWorldMatrix(orthographicMatrix * world);
    m_TextureEffect->setDiffuseMap(tex2D);
    m_TextureEffect->setAlpha(1.0f);
    m_TextureEffect->setTCOffset(tcOffset);
    m_TextureEffect->setTCScale(tcScale);
    m_TextureEffect->setDepth(0.0f);

    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

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

    GL::heBindVao(m_TextureQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_TextureQuad->getNumIndices(), m_TextureQuad->getIndexType(), 0);
}

void Renderer2D::attachToRender( I2DDrawable* drawable )
{
    m_Drawables.push_back(drawable);
}

void Renderer2D::detachFromRender( I2DDrawable* drawable )
{
    std::vector<I2DDrawable*>::iterator it(std::find(m_Drawables.begin(), m_Drawables.end(), drawable));
    HE_IF_ASSERT(it != m_Drawables.end(), "drawable not found in draw list")
    {
        *it = m_Drawables.back();
        m_Drawables.pop_back();
    }
}

} } //end namespace