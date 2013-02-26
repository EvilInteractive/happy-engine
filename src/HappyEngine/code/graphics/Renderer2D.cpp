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
// warnings in awesomium lib
#pragma warning(disable:4100)
#include "Awesomium/WebCore.h"
#pragma warning(default:4100)
#include "ModelMesh.h"
#include "Canvas2D.h"
#include "WebView.h"
#include "RenderTarget.h"
#include "Texture2D.h"
#include "View.h"
#include "IDrawable2D.h"

namespace he {
namespace gfx {

Renderer2D::Renderer2D() :  m_TextureEffect(NEW Simple2DTextureEffect()),
                            m_TextureQuad(nullptr),
                            m_View(nullptr),
                            m_RenderTarget(nullptr),
                            m_DefaultCanvas(nullptr)
{
}

Renderer2D::~Renderer2D()
{
    removeCanvas(m_DefaultCanvas);

    delete m_TextureEffect;
    m_TextureQuad->release();
}

/* GENERAL */
gui::Canvas2D* Renderer2D::createCanvasAbsolute(const RectI& viewport)
{
    HE_ASSERT(viewport.width > 0 && viewport.height > 0, "viewport width and height must be > 0");
    he::gui::Canvas2D* canvas(NEW gui::Canvas2D(this, viewport));
    m_Canvas2Ds.add(canvas);
    return canvas;
}
gui::Canvas2D* Renderer2D::createCanvasRelative(const RectF& percent)
{
    HE_ASSERT(percent.width > 0 && percent.height > 0, "viewport width and height must be > 0");
    he::gui::Canvas2D* canvas(NEW gui::Canvas2D(this, percent));
    m_Canvas2Ds.add(canvas);
    return canvas;
}
void Renderer2D::removeCanvas(gui::Canvas2D* canvas)
{
    HE_IF_ASSERT(m_Canvas2Ds.contains(canvas) == true, "Canvas is not a member of this Renderer2D!")
    {
        m_Canvas2Ds.remove(canvas);
        delete canvas;
    }
}

WebView* Renderer2D::createWebViewAbsolute(const RectI& viewport, bool enableUserInput)
{
    WebView* web(NEW WebView(m_View, viewport, enableUserInput));
    m_WebViews.add(web);
    return web;
}
WebView* Renderer2D::createWebViewRelative(const RectF& viewportPercent, bool enableUserInput)
{
    WebView* web(NEW WebView(m_View, viewportPercent, enableUserInput));
    m_WebViews.add(web);
    return web;
}
void Renderer2D::removeWebView( WebView* webview )
{
    HE_IF_ASSERT(m_WebViews.contains(webview), "Canvas is not a member of this Renderer2D!")
    {
        m_WebViews.remove(webview);
        delete webview;
    }
}

void Renderer2D::render()
{
    he::PrimitiveList<std::pair<uint32,uint16> > orderList(m_DrawablesDepth.size());

    m_DrawablesDepth.forEach([&orderList](std::pair<uint32,uint16> p)
    {
        orderList.add(p);
    });

    orderList.sort([](std::pair<uint32,uint16> p1, std::pair<uint32,uint16> p2) -> int
    {
        if (p1.second < p2.second)
            return -1;
        else if (p1.second > p2.second)
            return 1;
        else
            return 0;
    });

    m_DefaultCanvas->clear();

    orderList.forEach([this](std::pair<uint32,uint16> p)
    {
        m_Drawables[p.first]->draw2D(m_DefaultCanvas);
    });

    m_RenderTarget->prepareForRendering();
    m_DefaultCanvas->draw();
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
    m_TextureQuad->init(vLayout, gfx::MeshDrawMode_Triangles);

    he::ObjectList<VertexPosTex2D> vertices(4);
    vertices.add(
        VertexPosTex2D(vec2(-0.5f, 0.5f),
        vec2(0, 0)));

    vertices.add(
        VertexPosTex2D(vec2(0.5f, 0.5f),
        vec2(1, 0)));

    vertices.add(
        VertexPosTex2D(vec2(-0.5f, -0.5f),
        vec2(0, 1)));

    vertices.add(
        VertexPosTex2D(vec2(0.5f, -0.5f),
        vec2(1, 1)));

    he::PrimitiveList<uint8> indices(6);
    indices.add(0); indices.add(1); indices.add(2);
    indices.add(1); indices.add(3); indices.add(2);

    m_TextureQuad->setVertices(&vertices[0], 4, gfx::MeshUsage_Static);
    m_TextureQuad->setIndices(&indices[0], 6, IndexStride_Byte, gfx::MeshUsage_Static);
    m_TextureQuad->setLoaded();

    m_DefaultCanvas = NEW he::gui::Canvas2D(this, RectF(0,0,1,1));
    m_DefaultCanvas->init();
    m_Canvas2Ds.add(m_DefaultCanvas);
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
        GL::heBlendFunc(BlendFunc_One, BlendFunc_OneMinusSrcAlpha);
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

void Renderer2D::attachToRender(IDrawable2D* drawable, uint16 depth)
{
    m_Drawables.add(drawable);
    m_DrawablesDepth.add(std::pair<uint32,uint16>(m_Drawables.size() - 1, 0xffff - depth));
}

void Renderer2D::detachFromRender(IDrawable2D* drawable)
{
    HE_IF_ASSERT(m_Drawables.contains(drawable), "drawable not found in draw list")
    {
        size_t i(0);
        m_Drawables.find(drawable, i);
        m_Drawables.remove(drawable);

        // reposition depths
        he::PrimitiveList<std::pair<uint32,uint16> > temp;
        temp.append(m_DrawablesDepth);
        m_DrawablesDepth.clear();
        
        i = 0;
        m_Drawables.forEach([&](IDrawable2D* /*drawable*/)
        {
            m_DrawablesDepth.add(std::pair<uint32,uint16>(i, temp[i].second));
            ++i;
        });
    }
}

} } //end namespace