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
#include "ControlsManager.h"
#include "IMouse.h"
#include "IKeyboard.h"
#ifdef USE_WEB
// warnings in awesomium lib
#pragma warning(disable:4100)
#include "Awesomium/WebCore.h"
#pragma warning(default:4100)
#endif
#include "Canvas2D.h"
#include "WebView.h"
#include "RenderTarget.h"
#include "Texture2D.h"
#include "View.h"
#include "IDrawable2D.h"

namespace he {
namespace gfx {

Renderer2D::Renderer2D() :  m_View(nullptr),
                            m_RenderTarget(nullptr),
                            m_DefaultCanvas(nullptr)
{
}

Renderer2D::~Renderer2D()
{
    removeCanvas(m_DefaultCanvas);
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
    
#ifdef USE_WEB
gui::WebView* Renderer2D::createWebViewAbsolute(const RectI& viewport, bool enableUserInput)
{
    gui::WebView* web(NEW gui::WebView(m_View, viewport, enableUserInput));
    m_WebViews.add(web);
    return web;
}
gui::WebView* Renderer2D::createWebViewRelative(const RectF& viewportPercent, bool enableUserInput)
{
    gui::WebView* web(NEW gui::WebView(m_View, viewportPercent, enableUserInput));
    m_WebViews.add(web);
    return web;
}
void Renderer2D::removeWebView(gui::WebView* webview)
{
    HE_IF_ASSERT(m_WebViews.contains(webview), "Canvas is not a member of this Renderer2D!")
    {
        m_WebViews.remove(webview);
        delete webview;
    }
}
#endif

void Renderer2D::preRender()
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
}

void Renderer2D::render()
{
    m_RenderTarget->prepareForRendering();
    if (m_View->getStereo() == StereoSetting_None)
    {
        m_DefaultCanvas->draw();
    }
    else if (m_View->getCamera() != nullptr)
    {
        m_DefaultCanvas->draw3D(m_View->getCamera());
    }
}

void Renderer2D::init( View* view, const RenderTarget* target )
{
    m_View = view;
    m_RenderTarget = target;

    m_DefaultCanvas = NEW he::gui::Canvas2D(this, RectF(0,0,1,1));
    m_DefaultCanvas->init();

    m_Canvas2Ds.add(m_DefaultCanvas);
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