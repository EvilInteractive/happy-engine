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

#include "HappyPCH.h" 

#include "Renderer2D.h"
#include "GraphicsEngine.h"
#include "Awesomium/BitmapSurface.h"
#include "ContentManager.h"
#include "Happy2DRenderer.h"
#include "ControlsManager.h"

namespace he {
namespace gfx {

Renderer2D::Renderer2D() :  m_pWebCore(nullptr)
{
}

Renderer2D::~Renderer2D()
{
    if (m_pWebCore != nullptr)
    {
        Awesomium::WebCore::Shutdown();
    }

    std::for_each(m_WebViewRenderTextures.begin(), m_WebViewRenderTextures.end(), [](Texture2D* pTex2D)
    {
        pTex2D->release();
    });
}

/* GENERAL */
Canvas2D* Renderer2D::createCanvas()
{
    return nullptr;
}

WebView* Renderer2D::createWebView(bool bEnableUserInput)
{
    if (m_pWebCore == nullptr)
    {
        m_pWebCore = Awesomium::WebCore::Initialize(Awesomium::Config());
    }

    Awesomium::WebView* pView = m_pWebCore->CreateWebView(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight());

    WebView* pWeb = NEW WebView(pView, m_WebViews.size(), bEnableUserInput);
    m_WebViews.push_back(pWeb);

    Texture2D* pRenderTexture = CONTENT->makeEmptyTexture(vec2(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    m_WebViewRenderTextures.push_back(pRenderTexture);

    if (bEnableUserInput)
    {
        Awesomium::WebView* pW = pWeb->getAWEView();

        CONTROLS->getKeyboard()->addOnKeyPressedListener([&, pW](io::Key key)
        {
            if (pW != nullptr)
            {
                Awesomium::WebKeyboardEvent keyEvent;

                uint chr = io::getWebKeyFromKey(key);

                keyEvent.virtual_key_code = chr;
                char* buf = new char[20];

                Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
                strcpy(keyEvent.key_identifier, buf);

                delete[] buf;

                keyEvent.modifiers = 0;
                keyEvent.native_key_code = 0;
                keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyDown;

                pW->InjectKeyboardEvent(keyEvent);

                // if it is an ASCII char
                if (chr >= 32 && chr <= 126)
                {
                    // if letter
                    if (chr >= 65 && chr <= 90)
                    {
                        if (!(CONTROLS->getKeyboard()->isKeyDown(io::Key_Lshift) ||
                            CONTROLS->getKeyboard()->isKeyDown(io::Key_Rshift)))
                        {
                            chr += 32; // to lowercase ASCII
                        }
                    }

                    keyEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;
                    keyEvent.text[0] = chr;
                    keyEvent.unmodified_text[0] = chr;
                    keyEvent.native_key_code = chr;

                    pW->InjectKeyboardEvent(keyEvent);
                }
            }
        });

        CONTROLS->getKeyboard()->addOnKeyReleasedListener([&, pW](io::Key key)
        {
            if (pW != nullptr)
            {
                Awesomium::WebKeyboardEvent keyEvent;

                char* buf = new char[20];
                keyEvent.virtual_key_code = io::getWebKeyFromKey(key);

                Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
                strcpy(keyEvent.key_identifier, buf);

                delete[] buf;

                keyEvent.modifiers = 0;
                keyEvent.native_key_code = 0;
                keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;

                pW->InjectKeyboardEvent(keyEvent);
            }
        });

        CONTROLS->getMouse()->addOnButtonPressedListener([&, pW](io::MouseButton but)
        {
            if (pW != nullptr)
            {
                if (but == io::MouseButton_Left)
                    pW->InjectMouseDown(Awesomium::kLeftMouseButton);
                else if (but == io::MouseButton_Right)
                    pW->InjectMouseDown(Awesomium::kRightMouseButton);
                else if (but == io::MouseButton_Middle)
                    pW->InjectMouseDown(Awesomium::kMiddleMouseButton);
            }
        });

        CONTROLS->getMouse()->addOnButtonReleasedListener([&, pW](io::MouseButton but)
        {
            if (pW != nullptr)
            {
                if (but == io::MouseButton_Left)
                    pW->InjectMouseUp(Awesomium::kLeftMouseButton);
                else if (but == io::MouseButton_Right)
                    pW->InjectMouseUp(Awesomium::kRightMouseButton);
                else if (but == io::MouseButton_Middle)
                    pW->InjectMouseUp(Awesomium::kMiddleMouseButton);
            }
        });

        CONTROLS->getMouse()->addOnMouseMovedListener([&, pW](const vec2& pos)
        {
            if (pW != nullptr)
            {
                pW->InjectMouseMove(static_cast<int>(pos.x), static_cast<int>(pos.y));
            }
        });

        CONTROLS->getMouse()->addOnMouseWheelMovedListener([&, pW](int move)
        {
            if (pW != nullptr)
            {
                pW->InjectMouseWheel(move * 30, 0);
            }
        });
    }

    return pWeb;
}

void Renderer2D::tick()
{
    if (m_pWebCore != nullptr)
    {
        m_pWebCore->Update();
    }
}

void Renderer2D::draw()
{
    uint i(0);
    std::for_each(m_WebViews.begin(), m_WebViews.end(), [&](WebView* pView)
    {
        if (pView->getAWEView()->surface())
        {
            Awesomium::BitmapSurface* pSurface = static_cast<Awesomium::BitmapSurface*>(pView->getAWEView()->surface());

            if (pSurface->Dirty())
            {
                byte* buffer = NEW byte[pSurface->width() * 4 * pSurface->height()];

                pSurface->CopyTo(buffer, pSurface->width() * 4, 4, false, true);

                m_WebViewRenderTextures[i]->setData(
                    pSurface->width(), pSurface->height(), 
                    gfx::Texture2D::TextureFormat_RGBA8, buffer, 
                    gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte, false);

                delete[] buffer;
            } 
        }

        ++i;
    });

    std::for_each(m_WebViewRenderTextures.begin(), m_WebViewRenderTextures.end(), [](Texture2D* pTex2D)
    {
        GUI->drawTexture2D(pTex2D, vec2(0,0));
    });
}

void Renderer2D::init()
{
}

} } //end namespace