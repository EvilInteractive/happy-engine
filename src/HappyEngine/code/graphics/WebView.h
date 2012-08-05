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
    void loadUrl(const std::string& url); // for web
    void loadFile(const std::string& path); // for local
    void excecuteJavaScript(const std::string& script);
    void focus();
    void unfocus();
    void setTransparent(bool transparent);

    /* GETTERS */
    Awesomium::WebView* getAWEView() const;
    bool inputEnabled() const;

private:

    /* DATAMEMBERS */
    Awesomium::WebView* m_WebView;

    bool m_bInputEnabled;
    bool m_FullScreen;
    
    Texture2D* m_pRenderTexture;

    he::eventCallback1<void, io::Key> m_KeyPressedHandler;
    he::eventCallback1<void, io::Key> m_KeyReleasedHandler;
    he::eventCallback1<void, io::MouseButton> m_MouseButtonPressedHandler;
    he::eventCallback1<void, io::MouseButton> m_MouseButtonReleasedHandler;
    he::eventCallback1<void, int> m_MouseScrollHandler;
    he::eventCallback1<void, const vec2&> m_MouseMoveHandler;

    /* DEFAULT COPY & ASSIGNMENT */
    WebView(const WebView&);
    WebView& operator=(const WebView&);
};

}} //end namespace

#endif
