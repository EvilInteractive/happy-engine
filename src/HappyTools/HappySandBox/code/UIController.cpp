//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 10/07/2012

#include "HappySandBoxPCH.h" 

#include "UIController.h"
#include "Path.h"
#include "WebView.h"
#include "Renderer2D.h"
#include "View.h"
#include "Canvas2D.h"
#include "Font.h"
#include "ContentManager.h"

namespace hs {

/* CONSTRUCTOR - DESTRUCTOR */
UIController::UIController() :    m_GUIDirectory(""),
                                  m_WebView(nullptr),
                                  m_Renderer(nullptr)
{
}
UIController::~UIController()
{
    if (m_Renderer != nullptr && m_WebView != nullptr)
    {
        m_Renderer->detachFromRender(m_WebView);
        m_Renderer->removeWebView(m_WebView);
    }
}

/* GENERAL */
void UIController::init(he::gfx::Renderer2D* renderer)
{
    m_Renderer = renderer;

    // get gui dir
    m_GUIDirectory = he::Path::getWorkingDir().append("../../data/gui/").str();

    // create webview for gui to load
    m_WebView = m_Renderer->createWebViewRelative(he::RectF(0, 0, 1, 1), true); // fullscreen with user input enabled

    m_Renderer->attachToRender(this);
    m_Renderer->attachToRender(m_WebView, 0x1000);
    
    he::gui::Font* font(CONTENT->getDefaultFont(10));
    m_SceneInfo.setFont(font);
    font->release();
}
void UIController::load(const std::string& file)
{
    m_WebView->loadUrl(m_GUIDirectory + file);
    m_WebView->setTransparent(true);

    // awesomium focus bug
    m_WebView->unfocus();
    m_WebView->focus();
}

void UIController::draw2D(he::gui::Canvas2D* canvas)
{
//    canvas->setFillColor(he::Color(1.0f,1.0f,1.0f));
  //  canvas->fillRect(he::vec2(5,5),he::vec2(1,1));
    canvas->fillText(m_SceneInfo, he::vec2(50,30));
}

void UIController::updateSceneInfo(const he::vec3& camPos)
{
    m_CamPos = camPos;

    m_SceneInfo.clear();

    char buff[100];
    sprintf(buff, "Camera Position: %.2f, %.2f, %.2f\0", m_CamPos.x, m_CamPos.y, m_CamPos.z);

    m_SceneInfo.addLine(buff);
}

/* GETTERS */
he::gfx::WebView* UIController::getWebView() const
{
    return m_WebView;
}

} //end namespace