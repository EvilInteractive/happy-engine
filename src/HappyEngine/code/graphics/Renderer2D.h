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

#include "Canvas2D.h"
#include "WebView.h"
#include "Awesomium/WebCore.h"
#include "Texture2D.h"
#include "Simple2DTextureEffect.h"
#include "ModelMesh.h"

namespace he {
namespace gfx {

class Renderer2D
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
    Renderer2D();
    virtual ~Renderer2D();

	/* GENERAL */
    Canvas2D* createCanvas(const vec2& size = vec2());
    WebView* createWebView(bool enableUserInput = false, const vec2& size = vec2());

    void tick();
    void draw();

    void init();

    void drawTexture2DToScreen( const Texture2D* tex2D, const vec2& pos, bool useBlending = true,
                                const vec2& newDimensions = vec2(),
                                const RectF& regionToDraw = RectF());

private:

    /* EXTRA */

	/* DATAMEMBERS */
    Awesomium::WebCore* m_WebCore;

    std::vector<WebView*> m_WebViews;
    std::vector<Canvas2D*> m_Canvas2Ds;

    Simple2DTextureEffect* m_TextureEffect;
    ModelMesh* m_TextureQuad;

    /* DEFAULT COPY & ASSIGNMENT */
    Renderer2D(const Renderer2D&);
    Renderer2D& operator=(const Renderer2D&);
};

} } //end namespace

#endif
