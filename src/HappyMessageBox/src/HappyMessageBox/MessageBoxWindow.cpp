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
//Created: 2013/03/16

#include "MessageBoxWindow.h"

#ifdef USE_WEB
#include "Awesomium/BitmapSurface.h"
#include "Awesomium/WebView.h"
#include "Awesomium/STLHelpers.h"
#endif

#include <sstream>

#include <SDL2/SDL.h>
#include <assert.h>

namespace he {
namespace hmb {
    
#ifdef USE_WEB
MessageBoxWindow::MessageBoxWindow()
    : m_Result(HappyMessageBox::Button_None)
    , m_WebView(nullptr)
{    
}
void MessageBoxWindow::init(const char* html, const int width, const int height, const char* caption)
{
    // create MessageBoxWindow
    m_Window = new sf::Window();
    m_Window->create(sf::VideoMode(width, height), "");
    m_Window->setVerticalSyncEnabled(true);
    m_Window->setTitle(caption);
    m_WebView = Awesomium::WebCore::instance()->CreateWebView(width, height, 0, Awesomium::kWebViewType_Window);
    m_WebView->set_load_listener(this);
    m_WebView->set_js_method_handler(this);
    m_WebView->set_parent_window(static_cast<Awesomium::NativeWindow>(m_Window->getSystemHandle()));
    m_WebPage = html;
}

MessageBoxWindow::~MessageBoxWindow()
{
    m_WebView->Destroy();
    m_Window->close();
    delete m_Window;
}

void MessageBoxWindow::show()
{
    m_Result = HappyMessageBox::Button_None;
    
    m_WebView->LoadURL(Awesomium::WebURL(Awesomium::WSLit(m_WebPage.c_str())));

    while (m_Window->isOpen())
    {
        Awesomium::WebCore::instance()->Update();
        tick();
    }
}
    
void MessageBoxWindow::tick()
{
    sf::Event event;
    while (m_Window->pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                m_Window->close();
                m_Result = HappyMessageBox::Button_Escape;
                break;
            case sf::Event::Resized:
                m_WebView->Resize(event.size.width, event.size.height);
                break;
        }
    }
}
    
/* EXTRA */
void MessageBoxWindow::OnFailLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        ::int64  						/*frame_id*/,
        bool  						/*is_main_frame*/,
        const Awesomium::WebURL&  	/*url*/,
        int  						/*error_code*/,
        const Awesomium::WebString& /*error_desc*/ 
    )
{
    m_Window->close();
}

void MessageBoxWindow::OnFinishLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        ::int64  						/*frame_id*/,
        bool  						/*is_main_frame*/,
        const Awesomium::WebURL&  	/*url*/ 
    )
{
    Awesomium::JSArray array;

    const size_t buttons(m_Buttons.size());
    for (size_t i(0); i < buttons; ++i)
    {
        array.Clear();
        array.Push(Awesomium::JSValue(Awesomium::WSLit(m_Buttons[i].c_str())));
        executeFunction("addButton", array);
    }

    const size_t messages(m_Messages.size());
    for (size_t i(0); i < messages; ++i)
    {
        array.Clear();
        array.Push(Awesomium::JSValue(Awesomium::WSLit(m_Messages[i].first.c_str())));
        array.Push(Awesomium::JSValue(Awesomium::WSLit(m_Messages[i].second.c_str())));
        executeFunction("addTab", array);
    }
}

void MessageBoxWindow::OnDocumentReady(
        Awesomium::WebView *  		/*caller*/,
        const Awesomium::WebURL &  	/*url */
    )
{
    Awesomium::JSValue value(m_WebView->CreateGlobalJavascriptObject(Awesomium::WSLit("HE")));    
    Awesomium::WebString aweMethod = Awesomium::WSLit("onButtonClicked");
    assert(value.IsObject());
    Awesomium::JSObject& obj(value.ToObject());
    obj.SetCustomMethod(aweMethod, false);
}

void MessageBoxWindow::OnBeginLoadingFrame(
        Awesomium::WebView*			/*caller*/,
        ::int64						/*frame_id*/,
        bool						/*is_main_frame*/,
        const Awesomium::WebURL&	/*url*/,
        bool						/*is_error_page*/
    )
{
}
    
void MessageBoxWindow::executeFunction( const char* method, const Awesomium::JSArray& args )
{
    Awesomium::JSValue window(
        m_WebView->ExecuteJavascriptWithResult(
        Awesomium::WSLit("window"), Awesomium::WSLit("")));
    Awesomium::JSObject& obj = window.ToObject();

    obj.Invoke(Awesomium::WSLit(method), args);
}

void MessageBoxWindow::OnMethodCall( Awesomium::WebView* /*caller*/, unsigned int /*remote_object_id*/, const Awesomium::WebString& method_name, const Awesomium::JSArray& args )
{
    if (strcmp(Awesomium::ToString(method_name).c_str(), "onButtonClicked") == 0)
    {
        assert(args.size() == 1);
        assert(args[0].IsString());
        std::string button(Awesomium::ToString(args[0].ToString()));

        const size_t buttons(m_Buttons.size());
        for (size_t i(0); i < buttons; ++i)
        {
            if (strcmp(m_Buttons[i].c_str(), button.c_str()) == 0)
            {
                m_Result = static_cast<HappyMessageBox::Button>(HappyMessageBox::Button_Button1 + i);
                break;
            }
        }
        assert(m_Result != HappyMessageBox::Button_None);
        m_Window->close();
    }
}

Awesomium::JSValue MessageBoxWindow::OnMethodCallWithReturnValue( Awesomium::WebView* /*caller*/, unsigned int /*remote_object_id*/, const Awesomium::WebString& /*method_name*/, const Awesomium::JSArray& /*args*/ )
{
    return Awesomium::JSValue();
}
    
#else
MessageBoxWindow::MessageBoxWindow()
: m_Result(HappyMessageBox::Button_None)
{
}
void MessageBoxWindow::init(const char* html, const int width, const int height, const char* caption)
{
    // create MessageBoxWindow
    m_Title = caption;
}

MessageBoxWindow::~MessageBoxWindow()
{
}

void MessageBoxWindow::show()
{
    m_Result = HappyMessageBox::Button_None;
    
    SDL_MessageBoxData data;
    data.flags = 0;
    switch (m_Icon)
    {
        case HappyMessageBox::Icon_Info: data.flags |= SDL_MESSAGEBOX_INFORMATION; break;
        case HappyMessageBox::Icon_Warning: data.flags |= SDL_MESSAGEBOX_WARNING; break;
        case HappyMessageBox::Icon_Error: data.flags |= SDL_MESSAGEBOX_ERROR; break;
        case HappyMessageBox::Icon_Success: data.flags |= SDL_MESSAGEBOX_INFORMATION; break;
        case HappyMessageBox::Icon_ProgrammerAssert: data.flags |= SDL_MESSAGEBOX_ERROR; break;
        case HappyMessageBox::Icon_ArtAssert: data.flags |= SDL_MESSAGEBOX_WARNING; break;
    }
    data.window = nullptr;
    data.title = m_Title.c_str();
    
    std::stringstream msgStream;
    const size_t messageCount(m_Messages.size());
    for (size_t i(0); i < messageCount; ++i)
    {
        const std::pair<std::string, std::string>& pair(m_Messages[i]);
        msgStream << pair.first << ":\n";
        msgStream << pair.second << "\n\n";
    }
    std::string message(msgStream.str());
    data.message = message.c_str();

    const size_t buttonCount(m_Buttons.size());
    data.numbuttons = buttonCount;
    
    SDL_MessageBoxButtonData* buttons(static_cast<SDL_MessageBoxButtonData*>(malloc(sizeof(SDL_MessageBoxButtonData) * buttonCount)));
    for (size_t i(0); i < buttonCount; ++i)
    {
        SDL_MessageBoxButtonData& button(buttons[i]);
        button.buttonid = i;
        button.text = m_Buttons[i].c_str();
        button.flags = 0;
    }
    data.buttons = buttons;
    data.colorScheme = NULL;
    
    int button(0);
    if (SDL_ShowMessageBox(&data, &button) == 0)
    {
        m_Result = static_cast<HappyMessageBox::Button>(HappyMessageBox::Button_Button1 + button);
    }
    
    free(buttons);
}

void MessageBoxWindow::tick()
{
}
#endif
    
void MessageBoxWindow::addButton( const char* button )
{
    m_Buttons.push_back(button);
}

void MessageBoxWindow::addText( const char* tabName, const char* text )
{
    m_Messages.push_back(std::make_pair(tabName, text));
}

void MessageBoxWindow::setIcon( const HappyMessageBox::Icon icon )
{
    m_Icon = icon;
}

} } //end namespace
