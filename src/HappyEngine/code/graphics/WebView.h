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
//Created: 14/04/2012

#ifndef _HE_WEBVIEW_H_
#define _HE_WEBVIEW_H_
#pragma once

#include "Awesomium/WebCore.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

class WebView
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    WebView(Awesomium::WebView* pView, bool bEnableUserInput, bool fullscreen);
    virtual ~WebView();

    /* GENERAL */
    void draw(const vec2& pos = vec2());
    void loadUrl(const std::string& url);
    void excecuteJavaScript(const std::string& script);
    void focus();
    void unfocus();

    /* GETTERS */
    Awesomium::WebView* getAWEView() const;
    bool inputEnabled() const;

private:

    /* DATAMEMBERS */
    Awesomium::WebView* m_pWebView;

    bool m_bInputEnabled;
    bool m_FullScreen;
    
    Texture2D* m_pRenderTexture;

    /* DEFAULT COPY & ASSIGNMENT */
    WebView(const WebView&);
    WebView& operator=(const WebView&);
};

}} //end namespace

#endif
