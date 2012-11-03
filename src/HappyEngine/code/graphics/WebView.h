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

#include "IDrawable2D.h"

#pragma warning(disable:4100)
#include "Awesomium/WebCore.h"
#pragma warning(default:4100)

namespace Awesomium {
    class WebView;
}

namespace he {
namespace gfx {
class Texture2D;

class WebView : public IDrawable2D, public Awesomium::WebViewListener::Load
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    WebView(const RectI& viewport, bool bEnableUserInput);
    WebView(View* view, const RectF& viewportPercent, bool bEnableUserInput);
    virtual ~WebView();

    /* GENERAL */
    virtual void draw2D(Canvas2D* canvas);
    void loadUrl(const std::string& url);   // for web
    void loadFile(const he::Path& path);    // for local
    void excecuteJavaScript(const std::string& script);
    void focus();
    void unfocus();
    void setTransparent(bool transparent);

    /* GETTERS */
    Awesomium::WebView* getAWEView() const;
    bool inputEnabled() const;

    /* SETTERS */
    void setPosition(const vec2& position) { m_Position = position; }

	// webview load listeners
	virtual void OnFailLoadingFrame(
		Awesomium::WebView *  		caller,
		int64  						frame_id,
		bool  						is_main_frame,
		const Awesomium::WebURL&  	url,
		int  						error_code,
		const Awesomium::WebString& error_desc 
	);

	virtual void OnFinishLoadingFrame(
		Awesomium::WebView *  		caller,
		int64  						frame_id,
		bool  						is_main_frame,
		const Awesomium::WebURL&  	url 
	);

	virtual void OnDocumentReady(
		Awesomium::WebView *  		caller,
		const Awesomium::WebURL &  	url 
	);

	virtual void OnBeginLoadingFrame(
		Awesomium::WebView*			caller,
		int64						frame_id,
		bool						is_main_frame,
		const Awesomium::WebURL&	url,
		bool						is_error_page
	);

private:
    void init();
    void onViewResized();
    void resize(const vec2& newSize);

    /* DATAMEMBERS */
    Awesomium::WebView* m_WebView;

    bool m_bInputEnabled;
    
    Texture2D* m_pRenderTexture;
    vec2 m_Position;
    vec2 m_Size;
    RectF m_ViewportPercent;

    uint8* m_Buffer;

    // View
    View* m_View;
    he::eventCallback0<void> m_ViewResizedHandler;

    // Controls
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
