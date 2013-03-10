//HappyEngine Copyright (C) 2011 - 2013  Bastian Damman, Sebastiaan Sprengers 
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
//Author:  Bastian Damman
//Created: 02/01/2013

#ifndef _HE_WEBWINDOW_H_
#define _HE_WEBWINDOW_H_
#pragma once

#pragma warning(disable:4100)
#include "Awesomium/WebCore.h"
#pragma warning(default:4100)

namespace Awesomium {
    class WebWindow;
}

namespace sf
{
    class Window;
}

namespace he {
namespace gfx {
class Texture2D;
class WebListener;

class WebWindow : public Awesomium::WebViewListener::Load
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    WebWindow();
    virtual ~WebWindow();

    void init(const int width, const int height);

    /* GENERAL */
    void loadUrl(const he::String& url);
    void focus();
    void unfocus();
    void tick();
    void close();
    void setTitle(const he::String& title);

    /* GETTERS */
    Awesomium::WebView* getAWEView() const { return m_WebView; } 
    bool isOpen() const;
    WebListener* getWebListener() const { return m_WebListener; }
    
    /* EVENTS */
    event0<void> OnUrlLoaded;
    event0<void> WindowClosed;

    // WebWindow load listeners
    virtual void OnFailLoadingFrame(
        Awesomium::WebView *  		caller,
        ::int64  						frame_id,
        bool  						is_main_frame,
        const Awesomium::WebURL&  	url,
        int  						error_code,
        const Awesomium::WebString& error_desc 
    );

    virtual void OnFinishLoadingFrame(
        Awesomium::WebView *  		caller,
        ::int64  						frame_id,
        bool  						is_main_frame,
        const Awesomium::WebURL&  	url 
    );

    virtual void OnDocumentReady(
        Awesomium::WebView *  		caller,
        const Awesomium::WebURL &  	url 
    );

    virtual void OnBeginLoadingFrame(
        Awesomium::WebView*			caller,
        ::int64						frame_id,
        bool						is_main_frame,
        const Awesomium::WebURL&	url,
        bool						is_error_page
        );

private:
    /* DATAMEMBERS */
    Awesomium::WebView* m_WebView;
    WebListener* m_WebListener;

    sf::Window* m_Window;
    
    /* DEFAULT COPY & ASSIGNMENT */
    WebWindow(const WebWindow&);
    WebWindow& operator=(const WebWindow&);
};

}} //end namespace

#endif
