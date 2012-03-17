//HappyEngine Copyright (C) m_KeyLookup[2011] = //  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version m_KeyLookup[3] = // of the License, or
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
#include "HappyPCH.h" 


#if HE_ENABLE_QT

#include "QtKeyboard.h"
#include "HappyNew.h"
#include "SDL.h"

#include <sstream>
#include <iostream>

#pragma warning(disable:4127)
#include <QKeyEvent>
#pragma warning(default:4127)

namespace he {
namespace io {

QtKeyboard::QtKeyboard(): m_NextKeyState(MAX_KEYS, 0), m_CurrentKeyState(MAX_KEYS, 0), m_PrevKeyState(MAX_KEYS, 0)
    //m_iniWriter()
    
{
    //m_iniWriter.open("keys.ini");
    m_KeyLookup[30] = SDL_SCANCODE_A; // A
    m_KeyLookup[48] = SDL_SCANCODE_B; // B
    m_KeyLookup[46] = SDL_SCANCODE_C; //C
    m_KeyLookup[32] = SDL_SCANCODE_D; //D
    m_KeyLookup[18] = SDL_SCANCODE_E; //E
    m_KeyLookup[33] = SDL_SCANCODE_F; //F
    m_KeyLookup[34] = SDL_SCANCODE_G; //G
    m_KeyLookup[35] = SDL_SCANCODE_H; //H
    m_KeyLookup[23] = SDL_SCANCODE_I; //I
    m_KeyLookup[36] = SDL_SCANCODE_J; //J
    m_KeyLookup[37] = SDL_SCANCODE_K; //K
    m_KeyLookup[38] = SDL_SCANCODE_L; //L
    m_KeyLookup[50] = SDL_SCANCODE_M; //M
    m_KeyLookup[49] = SDL_SCANCODE_N; //N
    m_KeyLookup[24] = SDL_SCANCODE_O; //O
    m_KeyLookup[25] = SDL_SCANCODE_P; //P
    m_KeyLookup[16] = SDL_SCANCODE_Q; //Q
    m_KeyLookup[19] = SDL_SCANCODE_R; //R
    m_KeyLookup[31] = SDL_SCANCODE_S; //S
    m_KeyLookup[20] = SDL_SCANCODE_T; //T
    m_KeyLookup[22] = SDL_SCANCODE_U; //U
    m_KeyLookup[47] = SDL_SCANCODE_V; //V
    m_KeyLookup[17] = SDL_SCANCODE_W; //W
    m_KeyLookup[45] = SDL_SCANCODE_X; //X
    m_KeyLookup[21] = SDL_SCANCODE_Y; //Y
    m_KeyLookup[44] = SDL_SCANCODE_Z; //Z
	
    m_KeyLookup[11] = SDL_SCANCODE_0; //0
    m_KeyLookup[2] = SDL_SCANCODE_1; //1
    m_KeyLookup[3] = SDL_SCANCODE_2; //2
    m_KeyLookup[4] = SDL_SCANCODE_3; //3
    m_KeyLookup[5] = SDL_SCANCODE_4; //4
    m_KeyLookup[6] = SDL_SCANCODE_5; //5
    m_KeyLookup[7] = SDL_SCANCODE_6; //6
    m_KeyLookup[8] = SDL_SCANCODE_7; //7
    m_KeyLookup[9] = SDL_SCANCODE_8; //8
    m_KeyLookup[10] = SDL_SCANCODE_9; //9
    m_KeyLookup[41] = SDL_SCANCODE_GRAVE; //Tilde
    m_KeyLookup[13] = SDL_SCANCODE_MINUS; //Minus
    m_KeyLookup[13] = SDL_SCANCODE_EQUALS; //Equals
    m_KeyLookup[26] = SDL_SCANCODE_LEFTBRACKET; //LeftBracket
    m_KeyLookup[27] = SDL_SCANCODE_RIGHTBRACKET; //RightBracket
    m_KeyLookup[39] = SDL_SCANCODE_SEMICOLON; //SemiColon
    m_KeyLookup[40] = SDL_SCANCODE_APOSTROPHE; //Quote
    m_KeyLookup[51] = SDL_SCANCODE_COMMA; //Comma
    m_KeyLookup[52] = SDL_SCANCODE_PERIOD; //Period
    m_KeyLookup[53] = SDL_SCANCODE_SLASH; //Slash
    m_KeyLookup[43] = SDL_SCANCODE_BACKSLASH; //BackSlash
	
    m_KeyLookup[1] = SDL_SCANCODE_ESCAPE; //Escape
    m_KeyLookup[15] = SDL_SCANCODE_TAB; //Tab
    m_KeyLookup[58] = SDL_SCANCODE_CAPSLOCK; //CapsLock
    m_KeyLookup[42] = SDL_SCANCODE_LSHIFT; //LeftShift
    m_KeyLookup[29] = SDL_SCANCODE_LCTRL; //LeftControl
    m_KeyLookup[347] = SDL_SCANCODE_LGUI; //LeftOsKey
    m_KeyLookup[56] = SDL_SCANCODE_LALT; //LeftAlt
    m_KeyLookup[57] = SDL_SCANCODE_SPACE; //Space
    m_KeyLookup[312] = SDL_SCANCODE_RALT; //RightAlt
    m_KeyLookup[348] = SDL_SCANCODE_RGUI; //RightOsKey
    m_KeyLookup[349] = SDL_SCANCODE_MENU; //Context
    m_KeyLookup[285] = SDL_SCANCODE_RCTRL; //RightControl
    m_KeyLookup[14] = SDL_SCANCODE_BACKSPACE; //Backspace
    m_KeyLookup[28] = SDL_SCANCODE_RETURN; //Return
    m_KeyLookup[54] = SDL_SCANCODE_RSHIFT; //RightShift
    m_KeyLookup[338] = SDL_SCANCODE_INSERT; //Insert
    m_KeyLookup[327] = SDL_SCANCODE_HOME; //Home
    m_KeyLookup[329] = SDL_SCANCODE_PAGEUP; //PageUp
    m_KeyLookup[339] = SDL_SCANCODE_DELETE; //Delete
    m_KeyLookup[335] = SDL_SCANCODE_END; //End
    m_KeyLookup[337] = SDL_SCANCODE_PAGEDOWN; //PageDown
    m_KeyLookup[70] = SDL_SCANCODE_SCROLLLOCK; //ScrollLock
    m_KeyLookup[69] = SDL_SCANCODE_PAUSE; //Pause
	
    m_KeyLookup[325] = SDL_SCANCODE_NUMLOCKCLEAR; //NumLock
    m_KeyLookup[309] = SDL_SCANCODE_KP_DIVIDE; //KP_Slash
    m_KeyLookup[55] = SDL_SCANCODE_KP_MULTIPLY; //KP_Asterisk
    m_KeyLookup[74] = SDL_SCANCODE_KP_MINUS; //KP_Minus
    m_KeyLookup[78] = SDL_SCANCODE_KP_PLUS; //KP_Plus
    m_KeyLookup[82] = SDL_SCANCODE_KP_0; //KP_0
    m_KeyLookup[79] = SDL_SCANCODE_KP_1; //KP_1
    m_KeyLookup[80] = SDL_SCANCODE_KP_2; //KP_2
    m_KeyLookup[81] = SDL_SCANCODE_KP_3; //KP_3
    m_KeyLookup[75] = SDL_SCANCODE_KP_4; //KP_4
    m_KeyLookup[76] = SDL_SCANCODE_KP_5; //KP_5
    m_KeyLookup[77] = SDL_SCANCODE_KP_6; //KP_6
    m_KeyLookup[71] = SDL_SCANCODE_KP_7; //KP_7
    m_KeyLookup[72] = SDL_SCANCODE_KP_8; //KP_8
    m_KeyLookup[73] = SDL_SCANCODE_KP_9; //KP_9
    m_KeyLookup[284] = SDL_SCANCODE_KP_ENTER; //KP_Return
    m_KeyLookup[83] = SDL_SCANCODE_KP_PERIOD; //KP_Period
	
    m_KeyLookup[59] = SDL_SCANCODE_F1; //F1
    m_KeyLookup[60] = SDL_SCANCODE_F2; //F2
    m_KeyLookup[61] = SDL_SCANCODE_F3; //F3
    m_KeyLookup[62] = SDL_SCANCODE_F4; //F4
    m_KeyLookup[63] = SDL_SCANCODE_F5; //F5
    m_KeyLookup[64] = SDL_SCANCODE_F6; //F6
    m_KeyLookup[65] = SDL_SCANCODE_F7; //F7
    m_KeyLookup[66] = SDL_SCANCODE_F8; //F8
    m_KeyLookup[67] = SDL_SCANCODE_F9; //F9
    m_KeyLookup[68] = SDL_SCANCODE_F10; //F10

    m_KeyLookup[331] = SDL_SCANCODE_LEFT; //Up
    m_KeyLookup[333] = SDL_SCANCODE_RIGHT; //Right
    m_KeyLookup[328] = SDL_SCANCODE_UP; //Left
    m_KeyLookup[336] = SDL_SCANCODE_DOWN; //Down

}

QtKeyboard::~QtKeyboard()
{
   // m_iniWriter.close();
}


void QtKeyboard::keyPressEvent(QKeyEvent* event)
{
    //static int i(0);
    //std::wstringstream stream;
    //std::string str(event->text().toStdString());
    //stream << L"Key_" << " <" << event->nativeVirtualKey() << ">" << 
    //    " [" << std::wstring(str.cbegin(), str.cend()) << "]";
    ////m_iniWriter.writeInt(L"Keys", stream.str(), (int)event->nativeScanCode());
    //std::wcout << stream.str() << " = " << (int)event->nativeScanCode() << "\n";
    std::map<uint, SDL_Scancode>::const_iterator code(m_KeyLookup.find(event->nativeScanCode()));
    if (code != m_KeyLookup.cend())
        m_NextKeyState[code->second] = 1;
}
void QtKeyboard::keyReleaseEvent (QKeyEvent* event)
{
    std::map<uint, SDL_Scancode>::const_iterator code(m_KeyLookup.find(event->nativeScanCode()));
    if (code != m_KeyLookup.cend())
        m_NextKeyState[code->second] = 0;
}

void QtKeyboard::tick()
{   
    m_PrevKeyState.swap(m_CurrentKeyState);
    m_CurrentKeyState = m_NextKeyState;
}

bool QtKeyboard::isKeyUp(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] == 0;
}
bool QtKeyboard::isKeyDown(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] != 0;
}

bool QtKeyboard::isKeyPressed(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] != 0 && m_PrevKeyState[SDL_GetScancodeFromKey(key)] == 0;
}
bool QtKeyboard::isKeyPressed(KeyScanCode code) const
{
    HE_ASSERT(false, "not implemented");
	return m_CurrentKeyState[code] != 0 && m_PrevKeyState[code] == 0;
}
bool QtKeyboard::isKeyReleased(Key key) const
{
    return m_CurrentKeyState[SDL_GetScancodeFromKey(key)] == 0 && m_PrevKeyState[SDL_GetScancodeFromKey(key)] != 0;
}
bool QtKeyboard::isKeyReleased(KeyScanCode code) const
{
    HE_ASSERT(false, "not implemented");
	return m_CurrentKeyState[code] == 0 && m_PrevKeyState[code] != 0;
}

} } //end namespace

#endif
