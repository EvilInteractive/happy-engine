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
#include "boost/filesystem.hpp"
#include "Path.h"
#include "WebView.h"
#include "Renderer2D.h"

namespace hs {

/* CONSTRUCTOR - DESTRUCTOR */
UIController::UIController() :    m_GUIDirectory(""),
                                    m_WebView(nullptr)
{
}
UIController::~UIController()
{
    delete m_WebView;
}

/* GENERAL */
void UIController::init()
{
    // get program dir
    boost::filesystem::path bPath = boost::filesystem::initial_path();
    he::Path hPath(bPath.string());

    // get gui dir
    m_GUIDirectory = hPath.getAbsolutePath(he::Path("../../data/gui/")).str();

    // create webview for gui to load
    m_WebView = GUI->createWebView(true); // fullscreen with user input enabled
}
void UIController::load(const std::string& file)
{
    m_WebView->loadUrl(m_GUIDirectory + file);
    m_WebView->setTransparent(true);
}
void UIController::draw()
{
    m_WebView->draw();
}

/* GETTERS */
he::gfx::WebView* UIController::getWebView() const
{
    return m_WebView;
}

} //end namespace