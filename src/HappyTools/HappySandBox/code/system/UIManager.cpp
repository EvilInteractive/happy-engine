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
    m_DebugUI->unload();
    m_MainUI->unload();

    delete m_MainUI;
    m_MainUI = nullptr;
    delete m_DebugUI;
    m_DebugUI = nullptr;
}

void UIManager::showEditorUI()
{
    m_MainUI->show();
    // + show all other UI's (toolbars / panels (make good system))
}

void UIManager::hideEditorUI()
{
    m_MainUI->hide();
}

void UIManager::showDebugUI()
{
    m_DebugUI->show();
}

void UIManager::hideDebugUI()
{
    m_DebugUI->hide();
}


} //end namespace
