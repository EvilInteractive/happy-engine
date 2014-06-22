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
//Author:
//Created: //

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
