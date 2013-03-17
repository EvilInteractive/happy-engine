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
//Created: 2013/03/16

#ifndef _HE_MessageBoxWindow_H_
#define _HE_MessageBoxWindow_H_
#pragma once

#pragma warning(disable:4100)
#include "Awesomium/WebCore.h"
#pragma warning(default:4100)

#include "HappyMessageBox.h"
#include <string>
#include <vector>

namespace Awesomium {
    class MessageBoxWindow;
}

namespace sf {
    class Window;
}

namespace he {
namespace hmb {
class WebListener;

class MessageBoxWindow : public Awesomium::WebViewListener::Load, public Awesomium::JSMethodHandler
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    MessageBoxWindow();
    ~MessageBoxWindow();
    
    /* GENERAL */
    void init(const char* html, const int width, const int height, const char* caption);
    void addButton(const char* button);
    void addText(const char* tabName, const char* text);
    void setIcon(const HappyMessageBox::Icon icon);
    void show();
        
    HappyMessageBox::Button getResult() { return m_Result; }

    // MessageBoxWindow load listeners
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

    virtual void OnMethodCall(Awesomium::WebView* caller,
        unsigned int remote_object_id,
        const Awesomium::WebString& method_name,
        const Awesomium::JSArray& args);

    virtual Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView* caller,
        unsigned int remote_object_id,
        const Awesomium::WebString& method_name,
        const Awesomium::JSArray& args);

private:
    void tick();
    void executeFunction(const char* method, const Awesomium::JSArray& args);

    /* DATAMEMBERS */
    Awesomium::WebView* m_WebView;
    sf::Window* m_Window;

    std::string m_WebPage;

    std::vector<std::string> m_Buttons;
    std::vector<std::pair<std::string, std::string>> m_Messages;

    HappyMessageBox::Icon m_Icon;
    HappyMessageBox::Button m_Result;
    
    /* DEFAULT COPY & ASSIGNMENT */
    MessageBoxWindow(const MessageBoxWindow&);
    MessageBoxWindow& operator=(const MessageBoxWindow&);
};

} } //end namespace

#endif
