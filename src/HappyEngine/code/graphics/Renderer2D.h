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

#ifndef _HE_RENDERER2D_H_
#define _HE_RENDERER2D_H_
#pragma once

#include "IRenderer.h"

namespace he {
namespace gui {
class Canvas2Dnew;
}
namespace gfx {
class WebView;
class Canvas2D;
class Simple2DTextureEffect;
class ModelMesh;
class IDrawable2D;
class Texture2D;

class Renderer2D : public IRenderer
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Renderer2D();
    virtual ~Renderer2D();

    /* GENERAL */
    Canvas2D* createCanvasAbsolute(const RectI& viewport);
    Canvas2D* createCanvasRelative(const RectF& viewportPercent);
    Canvas2D* getDefaultCanvas() { return m_DefaultCanvas; }
    void removeCanvas(Canvas2D* canvas);

    WebView* createWebViewAbsolute(const RectI& viewport, bool enableUserInput = false);
    WebView* createWebViewRelative(const RectF& viewportPercent, bool enableUserInput = false);
    void removeWebView(WebView* webview);
 
    void drawTexture2DToScreen( const Texture2D* tex2D, const vec2& pos = vec2(),
                                bool useBlending = true,
                                const vec2& newDimensions = vec2(),
                                const RectF& regionToDraw = RectF());

    View* getView() const { return m_View; }
    const RenderTarget* getRTG() const {return m_RenderTarget;}

    gui::Canvas2Dnew* getNewCanvas() const;

    /* Attach */
    void attachToRender(IDrawable2D* drawable);
    void detachFromRender(IDrawable2D* drawable);

    /* IRenderer */
    virtual void init(View* view, const RenderTarget* target);
    virtual RenderPass getRenderPass() const { return RenderPass_Gui; }
    virtual uint8 getRenderPriority() const { return 50ui8; }
    virtual void render();

private:

    /* DATAMEMBERS */
    View* m_View;
    const RenderTarget* m_RenderTarget;

    he::PrimitiveList<WebView*> m_WebViews;
    he::PrimitiveList<Canvas2D*> m_Canvas2Ds;
    he::PrimitiveList<IDrawable2D*> m_Drawables;

    Simple2DTextureEffect* m_TextureEffect;
    ModelMesh* m_TextureQuad;

    Canvas2D* m_DefaultCanvas;
    he::gui::Canvas2Dnew* m_CanvasNew;

    /* DEFAULT COPY & ASSIGNMENT */
    Renderer2D(const Renderer2D&);
    Renderer2D& operator=(const Renderer2D&);
};

} } //end namespace

#endif
