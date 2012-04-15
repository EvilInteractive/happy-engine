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

namespace he {
namespace gfx {

WebView::WebView(Awesomium::WebView* pView, bool bEnableUserInput) :    m_pWebView(pView),
                                                                        m_bInputEnabled(bEnableUserInput)
{
}

WebView::~WebView()
{
    m_pWebView->Destroy();
}

/* GENERAL */
void WebView::loadUrl(const std::string& url)
{
    Awesomium::WebURL webUrl(Awesomium::WebString::CreateFromUTF8(url.c_str(), strlen(url.c_str())));
    m_pWebView->LoadURL(webUrl);
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

/* SETTERS */
void WebView::enableInput(bool bInput)
{
    m_bInputEnabled = bInput;
}

}} //end namespace