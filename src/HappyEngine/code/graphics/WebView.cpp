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

#include "WebView.h"
#include "Awesomium/BitmapSurface.h"
#include "Renderer2D.h"
#include "GraphicsEngine.h"

#include "ControlsManager.h"
#include "IKeyboard.h"
#include "IMouse.h"

#define COMMON_ASCII_CHAR 128

namespace he {
namespace gfx {

WebView::WebView(Awesomium::WebView* pView, bool bEnableUserInput, bool fullscreen) :   m_WebView(pView),
                                                                                        m_bInputEnabled(bEnableUserInput),
                                                                                        m_FullScreen(fullscreen)
{
    ObjectHandle handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_pRenderTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_pRenderTexture->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Nearest,
                           gfx::Texture2D::TextureFormat_RGBA8, false);

    if (m_bInputEnabled)
    {
        io::IKeyboard* keyboard(CONTROLS->getKeyboard());
        io::IMouse* mouse(CONTROLS->getMouse());

        m_KeyPressedHandler = eventCallback1<void, io::Key>([&](io::Key key)
        {
            Awesomium::WebView* w(getAWEView());
            Awesomium::WebKeyboardEvent keyEvent;

            uint chr = io::getWebKeyFromKey(key);

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
        });

        m_KeyReleasedHandler = eventCallback1<void, io::Key>([&](io::Key key)
        {
            Awesomium::WebView* w(getAWEView());
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
        });

        m_MouseButtonPressedHandler = eventCallback1<void, io::MouseButton>([&](io::MouseButton but)
        {
            Awesomium::WebView* w(getAWEView());
            if (but == io::MouseButton_Left)
                w->InjectMouseDown(Awesomium::kMouseButton_Left);
            else if (but == io::MouseButton_Right)
                w->InjectMouseDown(Awesomium::kMouseButton_Right);
            else if (but == io::MouseButton_Middle)
                w->InjectMouseDown(Awesomium::kMouseButton_Middle);
        });

        m_MouseButtonReleasedHandler = eventCallback1<void, io::MouseButton>([&](io::MouseButton but)
        {
            Awesomium::WebView* w(getAWEView());
            if (but == io::MouseButton_Left)
                w->InjectMouseUp(Awesomium::kMouseButton_Left);
            else if (but == io::MouseButton_Right)
                w->InjectMouseUp(Awesomium::kMouseButton_Right);
            else if (but == io::MouseButton_Middle)
                w->InjectMouseUp(Awesomium::kMouseButton_Middle);
        });

        m_MouseMoveHandler = eventCallback1<void, const vec2&>([&](const vec2& pos)
        {
            Awesomium::WebView* w(getAWEView());
            w->InjectMouseMove(static_cast<int>(pos.x), static_cast<int>(pos.y));
        });

        m_MouseScrollHandler = eventCallback1<void, int>([&](int move)
        {
            Awesomium::WebView* w(getAWEView());
            w->InjectMouseWheel(move * 30, 0);
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
    if (m_bInputEnabled)
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
    m_WebView->Destroy();
    m_pRenderTexture->release();
}

/* GENERAL */
void WebView::draw(const vec2& pos)
{
    if (m_WebView->surface())
    {
        Awesomium::BitmapSurface* pSurface = static_cast<Awesomium::BitmapSurface*>(m_WebView->surface());

        if (pSurface->is_dirty())
        {
            byte* buffer = NEW byte[pSurface->width() * 4 * pSurface->height()];
            // TODO: seeb
            // make this buffer a member
            // register to View->ViewportSizeChanged
            // and realloc the buffer there
            // also use he_malloc he_realloc and he_free
            // realloc will first try to extend or shrink the buffer before searching for a new random spot in memory
            // keeps mem fragmentation at lowest

            pSurface->CopyTo(buffer, pSurface->width() * 4, 4, false, true);

            m_pRenderTexture->setData(
                pSurface->width(), pSurface->height(), 
                buffer, 
                gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte, 0);

            delete[] buffer;
        }

        if (m_FullScreen)
        {
            // TODO: seeb
            vec2 dim((float)GRAPHICS->getScreenWidth(), (float)GRAPHICS->getScreenHeight());
            vec2 dim2((float)pSurface->width(), (float)pSurface->height());

            if (dim != dim2)
            {
                m_WebView->Resize((int)dim.x,(int)dim.y);
            }
        }
    }

    GUI->drawTexture2DToScreen(m_pRenderTexture, pos);
}

void WebView::loadUrl(const std::string& url)
{
    Awesomium::WebURL webUrl(Awesomium::WebString::CreateFromUTF8(url.c_str(), strlen(url.c_str())));
    m_WebView->LoadURL(webUrl);
}

void WebView::loadFile(const std::string& /*path*/)
{
    //m_WebView->LoadURL()
}

void WebView::excecuteJavaScript(const std::string& /*script*/)
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
    return m_bInputEnabled;
}

}} //end namespace