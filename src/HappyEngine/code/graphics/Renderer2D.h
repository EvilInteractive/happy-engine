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
//Created: 19/03/2012

#ifndef _HE_RENDERER2D_H_
#define _HE_RENDERER2D_H_
#pragma once

#include "IRenderer.h"

namespace he {
namespace gui {
class Canvas2D;
#ifdef USE_WEB
class WebView;
#endif
}
namespace gfx {
class IDrawable2D;
class Texture2D;

class HAPPY_ENTRY Renderer2D : public IRenderer
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Renderer2D();
    virtual ~Renderer2D();

    /* GENERAL */
    gui::Canvas2D* createCanvasAbsolute(const RectI& viewport);
    gui::Canvas2D* createCanvasRelative(const RectF& viewportPercent);
    gui::Canvas2D* getDefaultCanvas() { return m_DefaultCanvas; }
    void removeCanvas(gui::Canvas2D* canvas);

#ifdef USE_WEB
    gui::WebView* createWebViewAbsolute(const RectI& viewport, bool enableUserInput = false);
    gui::WebView* createWebViewRelative(const RectF& viewportPercent, bool enableUserInput = false);
    void removeWebView(gui::WebView* webview);
#endif

    View* getView() const { return m_View; }
    const RenderTarget* getRTG() const {return m_RenderTarget;}

    /* Attach */
    void attachToRender(IDrawable2D* drawable, uint16 depth = 0x8000);
    void detachFromRender(IDrawable2D* drawable);

    /* IRenderer */
    virtual void init(View* view, const RenderTarget* target);
    virtual RenderPass getRenderPass() const { return RenderPass_Gui; }
    virtual uint8 getRenderPriority() const { return static_cast<uint8>(50); }
    virtual void preRender();
    virtual void render();

private:

    /* DATAMEMBERS */
    View* m_View;
    const RenderTarget* m_RenderTarget;
#ifdef USE_WEB
    he::PrimitiveList<gui::WebView*> m_WebViews;
#endif
    he::PrimitiveList<gui::Canvas2D*> m_Canvas2Ds;
    he::PrimitiveList<IDrawable2D*> m_Drawables;
    he::PrimitiveList<std::pair<uint32,uint16> > m_DrawablesDepth;

    gui::Canvas2D* m_DefaultCanvas;

    /* DEFAULT COPY & ASSIGNMENT */
    Renderer2D(const Renderer2D&);
    Renderer2D& operator=(const Renderer2D&);
};

} } //end namespace

#endif
