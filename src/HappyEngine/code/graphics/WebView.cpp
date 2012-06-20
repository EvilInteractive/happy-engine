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

namespace he {
namespace gfx {

WebView::WebView(Awesomium::WebView* pView, bool bEnableUserInput, bool fullscreen) :   m_pWebView(pView),
                                                                                        m_bInputEnabled(bEnableUserInput),
                                                                                        m_FullScreen(fullscreen)
{
    ObjectHandle hnd = ResourceFactory<Texture2D>::getInstance()->create();
    m_pRenderTexture = ResourceFactory<Texture2D>::getInstance()->get(hnd);
    m_pRenderTexture->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Nearest,
                           gfx::Texture2D::TextureFormat_RGBA8, false);
}

WebView::~WebView()
{
    m_pWebView->Destroy();
    m_pRenderTexture->release();
}

/* GENERAL */
void WebView::draw(const vec2& pos)
{
    if (m_pWebView->surface())
    {
        Awesomium::BitmapSurface* pSurface = static_cast<Awesomium::BitmapSurface*>(m_pWebView->surface());

        if (pSurface->Dirty())
        {
            byte* buffer = NEW byte[pSurface->width() * 4 * pSurface->height()];

            pSurface->CopyTo(buffer, pSurface->width() * 4, 4, false, true);

            m_pRenderTexture->setData(
                pSurface->width(), pSurface->height(), 
                buffer, 
                gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte, 0);

            delete[] buffer;
        }

        if (m_FullScreen)
        {
            vec2 dim((float)GRAPHICS->getScreenWidth(), (float)GRAPHICS->getScreenHeight());
            vec2 dim2((float)pSurface->width(), (float)pSurface->height());

            if (dim != dim2)
            {
                m_pWebView->Resize((int)dim.x,(int)dim.y);
            }
        }
    }

    GUI->drawTexture2DToScreen(m_pRenderTexture, pos);
}

void WebView::loadUrl(const std::string& url)
{
    Awesomium::WebURL webUrl(Awesomium::WebString::CreateFromUTF8(url.c_str(), strlen(url.c_str())));
    m_pWebView->LoadURL(webUrl);
}

void WebView::excecuteJavaScript(const std::string& /*script*/)
{
    //Awesomium::WebString string(script.c_str());
    //m_pWebView->ExecuteJavascriptWithResult(string);
}

void WebView::focus()
{
    m_pWebView->Focus();
}

void WebView::unfocus()
{
    m_pWebView->Unfocus();
}

/* GETTERS */
Awesomium::WebView* WebView::getAWEView() const
{
    return m_pWebView;
}

bool WebView::inputEnabled() const
{
    return m_bInputEnabled;
}

}} //end namespace