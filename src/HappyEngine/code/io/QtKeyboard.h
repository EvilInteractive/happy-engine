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
//Author:  Bastian Damman
//Created: 10/11/2011

#ifndef _HE_QTKEYBOARD_H_
#define _HE_QTKEYBOARD_H_
#pragma once


#if HE_ENABLE_QT

#include "HappyTypes.h"
#include "Keys.h"
#include <vector>
#include <map>
#include "IKeyboard.h"
#include "SDL.h"
//#include "IniWriter.h"

class QKeyEvent;

namespace he {
namespace io {

class QtKeyboard : public IKeyboard
{
public:
	QtKeyboard();
    virtual ~QtKeyboard();

    void tick();

    bool isKeyUp(Key key) const;
    bool isKeyDown(Key key) const;

    bool isKeyPressed(Key key) const;  //true when state goes from up to down
	bool isKeyPressed(KeyScanCode code) const;
    bool isKeyReleased(Key key) const; //true when state goes from down to up
	bool isKeyReleased(KeyScanCode code) const;

    
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent (QKeyEvent* event);

private:
    std::vector<byte> m_NextKeyState;       //1 = pressed
    std::vector<byte> m_CurrentKeyState;
    std::vector<byte> m_PrevKeyState;
    //IniWriter m_iniWriter;

    std::map<uint, SDL_Scancode> m_KeyLookup;
    
    //Disable default copy constructor and default assignment operator
    QtKeyboard(const QtKeyboard&);
    QtKeyboard& operator=(const QtKeyboard&);

};

} } //end namespace

#endif

#endif
