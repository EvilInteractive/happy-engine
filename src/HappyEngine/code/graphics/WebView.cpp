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

#include "HappyPCH.h" 

// warnings in awesomium lib
#pragma warning(disable:4100)
#include "WebView.h"
#include "Awesomium/BitmapSurface.h"
#include "Awesomium/WebView.h"
#include "Canvas2D.h"
#include "Renderer2D.h"
#include "GraphicsEngine.h"
#include "Texture2D.h"
#include "View.h"

#include "ControlsManager.h"
#include "IKeyboard.h"
#include "IMouse.h"
#pragma warning(default:4100)

#define COMMON_ASCII_CHAR 128

namespace he {
namespace gfx {

WebView::WebView(const RectI& viewport, bool enableUserInput) :   
m_WebView(nullptr),
    m_InputEnabled(enableUserInput),
    m_RenderTexture(nullptr),
    m_Position(static_cast<float>(viewport.x), static_cast<float>(viewport.y)),
    m_Size(-1, -1),
    m_View(nullptr),
    m_ViewportPercent(0, 0, 1, 1),
    m_Buffer(nullptr)
{    
    init();
    resize(vec2(static_cast<float>(viewport.width), static_cast<float>(viewport.height)));
}
#pragma warning(disable:4355) // this is initializer list
WebView::WebView(View* view, const RectF& viewportPercent, bool enableUserInput) :   
    m_WebView(nullptr),
    m_InputEnabled(enableUserInput),
    m_RenderTexture(nullptr),
    m_Position(0, 0),
    m_Size(-1, -1),
    m_View(view),
    m_ViewportPercent(viewportPercent),
    m_ViewResizedHandler(boost::bind(&WebView::onViewResized, this)),
    m_Buffer(nullptr)
{    
    init();
    m_View->ViewportSizeChanged += m_ViewResizedHandler;
    onViewResized();
}
#pragma warning(default:4355)
void WebView::init()
{
    // create buffer
    m_Buffer = static_cast<uint8*>(he_malloc(4));

    // create webview
    m_WebView = GRAPHICS->getWebCore()->CreateWebView(1,1);

    ObjectHandle handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_RenderTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_RenderTexture->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest,
        gfx::TextureFormat_RGBA8, false);

    if (m_InputEnabled)
    {
        io::IKeyboard* keyboard(CONTROLS->getKeyboard());
        io::IMouse* mouse(CONTROLS->getMouse());

        Awesomium::WebView* w(m_WebView);
        m_KeyPressedHandler = eventCallback1<void, io::Key>([w,this](io::Key key)
        {
            // check if we cant get focus
            if (CONTROLS->getFocus(this) == false)
                return;

            Awesomium::WebKeyboardEvent keyEvent;

            uint32 chr = io::getWebKeyFromKey(key);

            keyEvent.virtual_key_code = chr;

            char buf[20];
            char* buffPtr(buf);
            Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buffPtr);
            strcpy(keyEvent.key_identifier, buf);

            keyEvent.modifiers = 0;
            keyEvent.native_key_code = 0;
            keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyDown;

            w->InjectKeyboardEvent(keyEvent);

            // if it is an ASCII char
            if (chr < COMMON_ASCII_CHAR)
            {
                // if it is a letter
                if (chr >= 65 && chr <= 90)
                {
                    if (!(CONTROLS->getKeyboard()->isKeyDown(io::Key_Lshift) ||
                        CONTROLS->getKeyboard()->isKeyDown(io::Key_Rshift)))
                    {
                        chr += 32; // to lowercase ASCII
                    }
                }

                keyEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;
                keyEvent.text[0] = (wchar16)chr;
                keyEvent.unmodified_text[0] = (wchar16)chr;
                keyEvent.native_key_code = chr;

                w->InjectKeyboardEvent(keyEvent);
            }

            CONTROLS->returnFocus(this);
        });

        m_KeyReleasedHandler = eventCallback1<void, io::Key>([w,this](io::Key key)
        {
            // check if we cant get focus
            if (CONTROLS->getFocus(this) == false)
                return;

            Awesomium::WebKeyboardEvent keyEvent;

            char buf[20];
            char* buffPtr(buf);
            keyEvent.virtual_key_code = io::getWebKeyFromKey(key);

            Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buffPtr);
            strcpy(keyEvent.key_identifier, buf);

            keyEvent.modifiers = 0;
            keyEvent.native_key_code = 0;
            keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;

            w->InjectKeyboardEvent(keyEvent);

            CONTROLS->returnFocus(this);
        });

        m_MouseButtonPressedHandler = eventCallback1<void, io::MouseButton>([w,this](io::MouseButton but)
        {
            // check if we cant get focus
            if (CONTROLS->getFocus(this) == false)
                return;

            if (but == io::MouseButton_Left)
                w->InjectMouseDown(Awesomium::kMouseButton_Left);
            else if (but == io::MouseButton_Right)
                w->InjectMouseDown(Awesomium::kMouseButton_Right);
            else if (but == io::MouseButton_Middle)
                w->InjectMouseDown(Awesomium::kMouseButton_Middle);

            CONTROLS->returnFocus(this);
        });

        m_MouseButtonReleasedHandler = eventCallback1<void, io::MouseButton>([w,this](io::MouseButton but)
        {
            // check if we cant get focus
            if (CONTROLS->getFocus(this) == false)
                return;

            if (but == io::MouseButton_Left)
                w->InjectMouseUp(Awesomium::kMouseButton_Left);
            else if (but == io::MouseButton_Right)
                w->InjectMouseUp(Awesomium::kMouseButton_Right);
            else if (but == io::MouseButton_Middle)
                w->InjectMouseUp(Awesomium::kMouseButton_Middle);

            CONTROLS->returnFocus(this);
        });

        m_MouseMoveHandler = eventCallback1<void, const vec2&>([w,this](const vec2& pos)
        {
            // check if we cant get focus
            if (CONTROLS->getFocus(this) == false)
                return;

            w->InjectMouseMove(static_cast<int>(pos.x), static_cast<int>(pos.y));

            CONTROLS->returnFocus(this);
        });

        m_MouseScrollHandler = eventCallback1<void, int>([w,this](int move)
        {
            // check if we cant get focus
            if (CONTROLS->getFocus(this) == false)
                return;

            w->InjectMouseWheel(move * 30, 0);

            CONTROLS->returnFocus(this);
        });

        keyboard->KeyPressed += m_KeyPressedHandler;
        keyboard->KeyReleased += m_KeyReleasedHandler;
        mouse->MouseButtonPressed += m_MouseButtonPressedHandler;
        mouse->MouseButtonReleased += m_MouseButtonReleasedHandler;
        mouse->MouseMoved += m_MouseMoveHandler;
        mouse->MouseWheelMoved += m_MouseScrollHandler;
    }
}


WebView::~WebView()
{
    if (m_View != nullptr)
        m_View->ViewportSizeChanged -= m_ViewResizedHandler;
    if (m_InputEnabled)
    {
        io::IKeyboard* keyboard(CONTROLS->getKeyboard());
        io::IMouse* mouse(CONTROLS->getMouse());
        keyboard->KeyPressed -= m_KeyPressedHandler;
        keyboard->KeyReleased -= m_KeyReleasedHandler;
        mouse->MouseButtonPressed -= m_MouseButtonPressedHandler;
        mouse->MouseButtonReleased -= m_MouseButtonReleasedHandler;
        mouse->MouseMoved -= m_MouseMoveHandler;
        mouse->MouseWheelMoved -= m_MouseScrollHandler;
    }

    he_free(m_Buffer);
    m_WebView->Destroy();
    m_RenderTexture->release();
}

/* GENERAL */
void WebView::draw2D(Canvas2D* canvas)
{
    if (m_WebView->surface())
    {
        Awesomium::BitmapSurface* pSurface = static_cast<Awesomium::BitmapSurface*>(m_WebView->surface());

        if (pSurface->is_dirty())
        {
            pSurface->CopyTo(m_Buffer, pSurface->width() * 4, 4, false, true);

            m_RenderTexture->setData(
                pSurface->width(), pSurface->height(), 
                m_Buffer, 
                gfx::TextureBufferLayout_BGRA, gfx::TextureBufferType_Byte, 0);
        }
    }

    canvas->drawImage(m_RenderTexture, m_Position);
}

void WebView::loadUrl(const std::string& url)
{
    Awesomium::WebURL webUrl(Awesomium::WebString::CreateFromUTF8(url.c_str(), strlen(url.c_str())));
    m_WebView->LoadURL(webUrl);

    m_WebView->set_load_listener(this);
}

void WebView::loadFile(const he::Path& /*path*/)
{
    //m_WebView->LoadURL()
}

void WebView::executeJavaScript(const std::string& /*script*/)
{
    //Awesomium::WebString string(script.c_str());
    //m_WebView->ExecuteJavascriptWithResult(string);
}

void WebView::focus()
{
    m_WebView->Focus();
}

void WebView::unfocus()
{
    m_WebView->Unfocus();
}

void WebView::setTransparent(bool transparent)
{
    m_WebView->SetTransparent(transparent);
}

/* GETTERS */
Awesomium::WebView* WebView::getAWEView() const
{
    return m_WebView;
}

bool WebView::inputEnabled() const
{
    return m_InputEnabled;
}

/* EXTRA */
void WebView::onViewResized()
{
    const RectI& viewport(m_View->getViewport());
    m_Position.x = viewport.x * m_ViewportPercent.x;
    m_Position.y = viewport.y * m_ViewportPercent.y;

    resize(vec2(viewport.width * m_ViewportPercent.width, viewport.height * m_ViewportPercent.height));
}

void WebView::resize( const vec2& newSize )
{
    if (m_Size != newSize)
    {
        if (m_WebView != nullptr)
            m_WebView->Resize((int)newSize.x, (int)newSize.y);
        else
            m_WebView = GRAPHICS->getWebCore()->CreateWebView((int)newSize.x, (int)newSize.y);

        m_Buffer = static_cast<uint8*>(he_realloc(m_Buffer, (int)newSize.x * 4 * (int)newSize.y));
        m_Size = newSize;
    }
}

void WebView::OnFailLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        int64  						/*frame_id*/,
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

    delete[] buff0, buff2;
}

void WebView::OnFinishLoadingFrame(
        Awesomium::WebView *  		/*caller*/,
        int64  						/*frame_id*/,
        bool  						/*is_main_frame*/,
        const Awesomium::WebURL&  	url 
    )
{
    char buff0[200];
    HE_ASSERT(200 > url.path().length(), "Output buffer to small!");
    url.path().ToUTF8(buff0, std::max(url.path().length(), 200ui32));

    HE_INFO("Finished loading url: '%s'", buff0);

    OnUrlLoaded();
}

void WebView::OnDocumentReady(
        Awesomium::WebView *  		/*caller*/,
        const Awesomium::WebURL &  	/*url */
    )
{
}

void WebView::OnBeginLoadingFrame(
        Awesomium::WebView*			/*caller*/,
        int64						/*frame_id*/,
        bool						/*is_main_frame*/,
        const Awesomium::WebURL&	/*url*/,
        bool						/*is_error_page*/
    )
{
}

}} //end namespace