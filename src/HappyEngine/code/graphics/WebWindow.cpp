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
//Author:  Bastian Damman
//Created: 02/01/2013

#include "HappyPCH.h" 

#include "WebWindow.h"
#include "Awesomium/BitmapSurface.h"
#include "Awesomium/WebView.h"
#include "Awesomium/STLHelpers.h"

#include "GraphicsEngine.h"
#include "Window.h"
#include "WebListener.h"

#include <SFML/Window.hpp>

namespace he {
namespace gui {

WebWindow::WebWindow() :   
m_WebView(nullptr),
m_WebListener(nullptr)
{    
}
void WebWindow::init(const int width, const int height)
{
    // create WebWindow
    m_WebView = GRAPHICS->getWebCore()->CreateWebView(width, height, 0, Awesomium::kWebViewType_Window);
    m_WebListener = NEW WebListener(m_WebView);
    m_Window = NEW sf::Window();
    m_Window->create(sf::VideoMode(width, height), "");
    m_Window->setVerticalSyncEnabled(true);
    m_WebView->set_parent_window(static_cast<Awesomium::NativeWindow>(m_Window->getSystemHandle()));
}

WebWindow::~WebWindow()
{
    m_WebView->Destroy();
    m_Window->close();
    delete m_Window;
    delete m_WebListener;
}

/* GENERAL */
void WebWindow::loadUrl(const std::string& url)
{
    Awesomium::WebURL webUrl(Awesomium::WebString::CreateFromUTF8(url.c_str(), static_cast<unsigned int>(strlen(url.c_str()))));
    m_WebView->LoadURL(webUrl);

    m_WebView->set_load_listener(this);
    m_WebView->set_js_method_handler(m_WebListener);
}

void WebWindow::focus()
{
    m_WebView->Focus();
}

void WebWindow::unfocus()
{
    m_WebView->Unfocus();
}

/* EXTRA */

void WebWindow::OnFailLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        ::int64  						/*frame_id*/,
        bool  						/*is_main_frame*/,
        const Awesomium::WebURL&  	url,
        int  						/*error_code*/,
        const Awesomium::WebString& error_desc 
    )
{
    char* buff0 = NEW char[url.path().length()];
    url.path().ToUTF8(buff0, url.path().length());

    char* buff2 = NEW char[error_desc.length()];
    error_desc.ToUTF8(buff2, error_desc.length());

    HE_WARNING("Failed to load url: '%s', '%s'", buff0, buff2);

    delete[] buff0;
    delete[] buff2;
}

void WebWindow::OnFinishLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        ::int64  						/*frame_id*/,
        bool  						/*is_main_frame*/,
        const Awesomium::WebURL&  	url 
    )
{
    char buff0[200];
    HE_ASSERT(200 > url.path().length(), "Output buffer to small!");
    url.path().ToUTF8(buff0, std::max<uint32>(url.path().length(), 200));

    HE_INFO("Finished loading url: '%s'", buff0);

    OnUrlLoaded();
}

void WebWindow::OnDocumentReady(
        Awesomium::WebView *  		/*caller*/,
        const Awesomium::WebURL &  	/*url */
    )
{
}

void WebWindow::OnBeginLoadingFrame(
        Awesomium::WebView*			/*caller*/,
        ::int64						/*frame_id*/,
        bool						/*is_main_frame*/,
        const Awesomium::WebURL&	/*url*/,
        bool						/*is_error_page*/
    )
{
}

void WebWindow::tick()
{
    sf::Event event;
    while (m_Window->pollEvent(event))
    {
        switch(event.type) 
        {
        case sf::Event::Closed:
            m_Window->close();
            WindowClosed();
            break;
        case sf::Event::Resized:
            m_WebView->Resize(event.size.width, event.size.height);
            break;
        }
    }
}

void WebWindow::close()
{
    m_Window->close();
}

void WebWindow::setTitle( const std::string& title )
{
    m_Window->setTitle(title);
}

bool WebWindow::isOpen() const
{
    return m_Window->isOpen();
}

}} //end namespace