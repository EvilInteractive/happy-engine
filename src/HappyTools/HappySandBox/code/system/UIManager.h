#ifndef _HS_UIManager_H_
#define _HS_UIManager_H_
#pragma once

#include <Singleton.h>

namespace hs {
class UIMain;
class UIDebug;

class UIManager : public he::Singleton<UIManager>
{
public:
    UIManager();
    virtual ~UIManager();
    
    void load();
    void unload();

    void showEditorUI();
    void hideEditorUI();

    void showDebugUI();
    void hideDebugUI();

    
private:    
    UIMain* m_MainUI;
    UIDebug* m_DebugUI;
    
    //Disable default copy constructor and default assignment operator
    UIManager(const UIManager&);
    UIManager& operator=(const UIManager&);
};

} //end namespace

#endif
