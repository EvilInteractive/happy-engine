//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "HappySandBoxPCH.h"
#include "UIManager.h"

#include "ui/UIMain.h"
#include "ui/UIDebug.h"

namespace hs {

UIManager::UIManager()
    : m_MainUI(nullptr)
    , m_DebugUI(nullptr)
{
    
}

UIManager::~UIManager()
{
    HE_ASSERT(m_MainUI == nullptr, "UIManager is not unloaded!");
}

void UIManager::load()
{
    m_MainUI = NEW UIMain();
    m_DebugUI = NEW UIDebug();

    m_MainUI->load();
    m_DebugUI->load();
}

void UIManager::unload()
{
    hideDebugUI();
    m_DebugUI->unload();
    hideEditorUI();
    m_MainUI->unload();

    delete m_MainUI;
    m_MainUI = nullptr;
    delete m_DebugUI;
    m_DebugUI = nullptr;
}

void UIManager::showEditorUI()
{
    if (m_MainUI->isVisible() == false)
    {
        m_MainUI->show();
    }
    // + show all other UI's (toolbars / panels (make good system))
}

void UIManager::hideEditorUI()
{
    if (m_MainUI->isVisible() == true)
    {
        m_MainUI->hide();
    }
}

void UIManager::showDebugUI()
{
    if (m_DebugUI->isVisible() == false)
    {
        m_DebugUI->show();
    }
}

void UIManager::hideDebugUI()
{
    if (m_DebugUI->isVisible() == true)
    {
        m_DebugUI->hide();
    }
}


} //end namespace
