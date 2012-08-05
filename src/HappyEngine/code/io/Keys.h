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
//Created: 08/08/2011

#ifndef _HE_KEYS_H_
#define _HE_KEYS_H_
#pragma once

#include "Awesomium/WebKeyboardCodes.h"

namespace he {
namespace io {

#define MAX_KEYS 512
#define mapWebKey(a, b) case Key_##a: return Awesomium::KeyCodes::AK_##b;

enum Key
{
    Key_Return = sf::Keyboard::Return,
    Key_Escape = sf::Keyboard::Escape,
    Key_Backspace = sf::Keyboard::BackSpace,
    Key_Tab = sf::Keyboard::Tab,
    Key_Space = sf::Keyboard::Space,
    //Key_Exclaim = sf::Keyboard::E,
    //Key_QuoteDbl = sf::Keyboard::A,
    //Key_Hash = sf::Keyboard::,
    //Key_Percent = sf::Keyboard::A,
    //Key_Dollar = sf::Keyboard::A,
    /*Key_Ampersand = sf::Keyboard::A,
    Key_Quote = sf::Keyboard::A,
    Key_LeftParen = sf::Keyboard::A,
    Key_RightParen = sf::Keyboard::A,
    Key_Asterisk = sf::Keyboard::A,
    Key_Plus = sf::Keyboard::A,
    Key_Comma = sf::Keyboard::A,
    Key_Minus = sf::Keyboard::A,
    Key_Period = sf::Keyboard::A,
    Key_Slash = sf::Keyboard::A,*/
    Key_0 = sf::Keyboard::Num0,
    Key_1 = sf::Keyboard::Num1,
    Key_2 = sf::Keyboard::Num2,
    Key_3 = sf::Keyboard::Num3,
    Key_4 = sf::Keyboard::Num4,
    Key_5 = sf::Keyboard::Num5,
    Key_6 = sf::Keyboard::Num6,
    Key_7 = sf::Keyboard::Num7,
    Key_8 = sf::Keyboard::Num8,
    Key_9 = sf::Keyboard::Num9,
    /*Key_Colon = sf::Keyboard::,
    Key_Semicolon = sf::Keyboard::A,
    Key_Less = sf::Keyboard::A,
    Key_Equals = sf::Keyboard::A,
    Key_Greater = sf::Keyboard::A,
    Key_Question = sf::Keyboard::A,
    Key_At = sf::Keyboard::A,*/
    /* 
       Skip uppercase letters
     */
    Key_LeftBracket = sf::Keyboard::LBracket,
    Key_Backslash = sf::Keyboard::BackSlash,
    Key_RightBracket = sf::Keyboard::RBracket,
    /*Key_Caret = sf::Keyboard::,
    Key_Underscore = sf::Keyboard::A,
    Key_Backquote = sf::Keyboard::A,*/
    Key_A = sf::Keyboard::A,
    Key_B = sf::Keyboard::B,
    Key_C = sf::Keyboard::C,
    Key_D = sf::Keyboard::D,
    Key_E = sf::Keyboard::E,
    Key_F = sf::Keyboard::F,
    Key_G = sf::Keyboard::G,
    Key_H = sf::Keyboard::H,
    Key_I = sf::Keyboard::I,
    Key_J = sf::Keyboard::J,
    Key_K = sf::Keyboard::K,
    Key_L = sf::Keyboard::L,
    Key_M = sf::Keyboard::M,
    Key_N = sf::Keyboard::N,
    Key_O = sf::Keyboard::O,
    Key_P = sf::Keyboard::P,
    Key_Q = sf::Keyboard::Q,
    Key_R = sf::Keyboard::R,
    Key_S = sf::Keyboard::S,
    Key_T = sf::Keyboard::T,
    Key_U = sf::Keyboard::U,
    Key_V = sf::Keyboard::V,
    Key_W = sf::Keyboard::W,
    Key_X = sf::Keyboard::X,
    Key_Y = sf::Keyboard::Y,
    Key_Z = sf::Keyboard::Z,

    //Key_Capslock = sf::Keyboard::Caps,

    Key_F1 = sf::Keyboard::F1,
    Key_F2 = sf::Keyboard::F2,
    Key_F3 = sf::Keyboard::F3,
    Key_F4 = sf::Keyboard::F4,
    Key_F5 = sf::Keyboard::F5,
    Key_F6 = sf::Keyboard::F6,
    Key_F7 = sf::Keyboard::F7,
    Key_F8 = sf::Keyboard::F8,
    Key_F9 = sf::Keyboard::F9,
    Key_F10 = sf::Keyboard::F10,
    Key_F11 = sf::Keyboard::F11,
    Key_F12 = sf::Keyboard::F12,

    /*Key_PrintScreen = sf::Keyboard::A,
    Key_ScrollLock = sf::Keyboard::A,
    //Key_Pause = sf::Keyboard::A,
    Key_Insert = sf::Keyboard::A,
    Key_Home = sf::Keyboard::A,
    Key_PageUp = sf::Keyboard::A,*/
    Key_Delete = sf::Keyboard::Delete,
    /*Key_End = sf::Keyboard::A,
    Key_PageDown = sf::Keyboard::A,*/
    Key_Right = sf::Keyboard::Right,
    Key_Left = sf::Keyboard::Left,
    Key_Down = sf::Keyboard::Down,
    Key_Up = sf::Keyboard::Up,

    /*Key_NumlockClear = sf::Keyboard::A,
    Key_KP_Divide = sf::Keyboard::A,
    Key_KP_Multiply = sf::Keyboard::A,
    Key_KP_Minus = sf::Keyboard::A,
    Key_KP_Plus = sf::Keyboard::A,
    Key_KP_Enter = sf::Keyboard::A,*/
    Key_KP_1 = sf::Keyboard::Numpad1,
    Key_KP_2 = sf::Keyboard::Numpad2,
    Key_KP_3 = sf::Keyboard::Numpad3,
    Key_KP_4 = sf::Keyboard::Numpad4,
    Key_KP_5 = sf::Keyboard::Numpad5,
    Key_KP_6 = sf::Keyboard::Numpad6,
    Key_KP_7 = sf::Keyboard::Numpad7,
    Key_KP_8 = sf::Keyboard::Numpad8,
    Key_KP_9 = sf::Keyboard::Numpad9,
    Key_KP_0 = sf::Keyboard::Numpad0,
    //Key_KP_Period = sf::Keyboard::A,

    /*Key_Application = sf::Keyboard::A,
    Key_Power = sf::Keyboard::A,
    Key_KP_Equals = sf::Keyboard::A,
    Key_F13 = sf::Keyboard::A,
    Key_F14 = sf::Keyboard::A,
    Key_F15 = sf::Keyboard::A,
    Key_F16 = sf::Keyboard::A,
    Key_F17 = sf::Keyboard::A,
    Key_F18 = sf::Keyboard::A,
    Key_F19 = sf::Keyboard::A,
    Key_F20 = sf::Keyboard::A,
    Key_F21 = sf::Keyboard::A,
    Key_F22 = sf::Keyboard::A,
    Key_F23 = sf::Keyboard::A,
    Key_F24 = sf::Keyboard::A,
    Key_Execute = sf::Keyboard::A,
    Key_Help = sf::Keyboard::A,
    Key_Menu = sf::Keyboard::A,
    Key_Select = sf::Keyboard::A,
    Key_Stop = sf::Keyboard::A,
    Key_Again = sf::Keyboard::A,
    Key_Undo = sf::Keyboard::A,
    Key_Cut = sf::Keyboard::A,
    Key_Copy = sf::Keyboard::A,
    Key_Paste = sf::Keyboard::A,
    Key_Find = sf::Keyboard::A,
    Key_Mute = sf::Keyboard::A,
    Key_Volumeup = sf::Keyboard::A,
    Key_Volumedown = sf::Keyboard::A,
    Key_KP_Comma = sf::Keyboard::A,
    Key_KP_EqualsAs400 = sf::Keyboard::A,

    Key_AltErase = sf::Keyboard::A,
    Key_Sysreq = sf::Keyboard::A,
    Key_Cancel = sf::Keyboard::A,
    Key_Clear = sf::Keyboard::A,
    Key_Prior = sf::Keyboard::A,
    Key_Return2 = sf::Keyboard::A,
    Key_Separator = sf::Keyboard::A,
    Key_Out = sf::Keyboard::A,
    Key_Oper = sf::Keyboard::A,
    Key_ClearAgain = sf::Keyboard::A,
    Key_Crsel = sf::Keyboard::A,
    Key_Exsel = sf::Keyboard::A,

    Key_KP_00 = sf::Keyboard::A,
    Key_KP_000 = sf::Keyboard::A,
    Key_ThousandsSeparator = sf::Keyboard::A,
    Key_DecimalSeparator = sf::Keyboard::A,
    Key_CurrencyUnit = sf::Keyboard::A,
    Key_CurrencySubUnit = sf::Keyboard::A,
    Key_KP_LeftParen = sf::Keyboard::A,
    Key_KP_RightParen = sf::Keyboard::A,
    Key_KP_LeftBrace = sf::Keyboard::A,
    Key_KP_RightBrace = sf::Keyboard::A,
    Key_KP_Tab = sf::Keyboard::A,
    Key_KP_Backspace = sf::Keyboard::A,
    Key_KP_A = sf::Keyboard::A,
    Key_KP_B = sf::Keyboard::A,
    Key_KP_C = sf::Keyboard::A,
    Key_KP_D = sf::Keyboard::A,
    Key_KP_E = sf::Keyboard::A,
    Key_KP_F = sf::Keyboard::A,
    Key_KP_Xor = sf::Keyboard::A,
    Key_KP_Power = sf::Keyboard::A,
    Key_KP_Percent = sf::Keyboard::A,
    Key_KP_Less = sf::Keyboard::A,
    Key_KP_Greater = sf::Keyboard::A,
    Key_KP_Ampersand = sf::Keyboard::A,
    Key_KP_DblAmpersand = sf::Keyboard::A,
    Key_KP_VerticalBar = sf::Keyboard::A,
    Key_KP_DblVerticalBar = sf::Keyboard::A,
    Key_KP_Colon = sf::Keyboard::A,
    Key_KP_Hash = sf::Keyboard::A,
    Key_KP_Space = sf::Keyboard::A,
    Key_KP_At = sf::Keyboard::A,
    Key_KP_Exclam = sf::Keyboard::A,
    Key_KP_MemStore = sf::Keyboard::A,
    Key_KP_MemRecall = sf::Keyboard::A,
    Key_KP_MemClear = sf::Keyboard::A,
    Key_KP_MemAdd = sf::Keyboard::A,
    Key_KP_MemSubtract = sf::Keyboard::A,
    Key_KP_MemMultiply = sf::Keyboard::A,
    Key_KP_MemDivide = sf::Keyboard::A,
    Key_KP_PlusMinus = sf::Keyboard::A,
    Key_KP_Clear = sf::Keyboard::A,
    Key_KP_Clearentry = sf::Keyboard::A,
    Key_KP_Binary = sf::Keyboard::A,
    Key_KP_Octal = sf::Keyboard::A,
    Key_KP_Decimal = sf::Keyboard::A,
    Key_KP_Hexadecimal = sf::Keyboard::A,*/

    Key_Lctrl = sf::Keyboard::LControl,
    Key_Lshift = sf::Keyboard::LShift,
    Key_Lalt = sf::Keyboard::LAlt,
    //Key_Lgui = sf::Keyboard::A,
    Key_Rctrl = sf::Keyboard::RControl,
    Key_Rshift = sf::Keyboard::RShift,
    Key_Ralt = sf::Keyboard::RAlt,
    //Key_Rgui = sf::Keyboard::A,

    /*Key_Mode = sf::Keyboard::A,

    Key_AudioNext = sf::Keyboard::A,
    Key_AudioPrev = sf::Keyboard::A,
    Key_AudioStop = sf::Keyboard::A,
    Key_AudioPlay = sf::Keyboard::A,
    Key_AudioMute = sf::Keyboard::A,
    Key_MediaSelect = sf::Keyboard::A,
    Key_WWW = sf::Keyboard::A,
    Key_Mail = sf::Keyboard::A,
    Key_Calculator = sf::Keyboard::A,
    Key_Computer = sf::Keyboard::A,
    Key_Ac_Search = sf::Keyboard::A,
    Key_Ac_Home = sf::Keyboard::A,
    Key_Ac_Back = sf::Keyboard::A,
    Key_Ac_Forward = sf::Keyboard::A,
    Key_Ac_Stop = sf::Keyboard::A,
    Key_Ac_Refresh = sf::Keyboard::A,
    Key_Ac_Bookmarks = sf::Keyboard::A,

    Key_BrightnessDown = sf::Keyboard::A,
    Key_BrightnessUp = sf::Keyboard::A,
    Key_DisplayDwitch = sf::Keyboard::A,
    Key_KbdIllumToggle = sf::Keyboard::A,
    Key_KbdIllumDown = sf::Keyboard::A,
    Key_KbdIllumUp = sf::Keyboard::A,
    Key_Eject = sf::Keyboard::A,
    Key_Sleep = sf::Keyboard::A,*/

    Key_MAX = sf::Keyboard::KeyCount
};

enum KeyScanCode
{

};

int getWebKeyFromKey(Key key);

} } //end namespace

#endif
