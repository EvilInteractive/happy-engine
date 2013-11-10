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
#include <SDL2/SDL_keycode.h>

namespace he {
namespace io {

#define MAX_KEYS 512
#define mapWebKey(a, b) case Key_##a: return Awesomium::KeyCodes::AK_##b;

enum Key
{
    Key_Unknown = SDLK_UNKNOWN,
    
    Key_Return = SDLK_RETURN,
    Key_Escape = SDLK_ESCAPE,
    Key_Backspace = SDLK_BACKSPACE,
    Key_Tab = SDLK_TAB,
    Key_Space = SDLK_SPACE,
    Key_ExclamationMark = SDLK_EXCLAIM,
    Key_DubbleQuote = SDLK_QUOTEDBL,
    Key_Hash = SDLK_HASH,
    Key_Percent = SDLK_PERCENT,
    Key_Dollar = SDLK_DOLLAR,
    Key_Ampersand = SDLK_AMPERSAND,
    Key_Quote = SDLK_QUOTE,
    Key_Leftparen = SDLK_LEFTPAREN,
    Key_Rightparen = SDLK_RIGHTPAREN,
    Key_Asterisk = SDLK_ASTERISK,
    Key_Plus = SDLK_PLUS,
    Key_Comma = SDLK_COMMA,
    Key_Minus = SDLK_MINUS,
    Key_Period = SDLK_PERIOD,
    Key_Slash = SDLK_SLASH,
    Key_0 = SDLK_0,
    Key_1 = SDLK_1,
    Key_2 = SDLK_2,
    Key_3 = SDLK_3,
    Key_4 = SDLK_4,
    Key_5 = SDLK_5,
    Key_6 = SDLK_6,
    Key_7 = SDLK_7,
    Key_8 = SDLK_8,
    Key_9 = SDLK_9,
    Key_Colon = SDLK_COLON,
    Key_Semicolon = SDLK_SEMICOLON,
    Key_Less = SDLK_LESS,
    Key_Equals = SDLK_EQUALS,
    Key_Greater = SDLK_GREATER,
    Key_Question = SDLK_QUESTION,
    Key_At = SDLK_AT,
    /*
     Skip uppercase letters
     */
    Key_Leftbracket = SDLK_LEFTBRACKET,
    Key_Backslash = SDLK_BACKSLASH,
    Key_Rightbracket = SDLK_RIGHTBRACKET,
    Key_Caret = SDLK_CARET,
    Key_Underscore = SDLK_UNDERSCORE,
    Key_Backquote = SDLK_BACKQUOTE,
    Key_A = SDLK_a,
    Key_B = SDLK_b,
    Key_C = SDLK_c,
    Key_D = SDLK_d,
    Key_E = SDLK_e,
    Key_F = SDLK_f,
    Key_G = SDLK_g,
    Key_H = SDLK_h,
    Key_I = SDLK_i,
    Key_J = SDLK_j,
    Key_K = SDLK_k,
    Key_L = SDLK_l,
    Key_M = SDLK_m,
    Key_N = SDLK_n,
    Key_O = SDLK_o,
    Key_P = SDLK_p,
    Key_Q = SDLK_q,
    Key_R = SDLK_r,
    Key_S = SDLK_s,
    Key_T = SDLK_t,
    Key_U = SDLK_u,
    Key_V = SDLK_v,
    Key_W = SDLK_w,
    Key_X = SDLK_x,
    Key_Y = SDLK_y,
    Key_Z = SDLK_z,
    
    Key_Capslock = SDLK_CAPSLOCK,
    
    Key_F1 = SDLK_F1,
    Key_F2 = SDLK_F2,
    Key_F3 = SDLK_F3,
    Key_F4 = SDLK_F4,
    Key_F5 = SDLK_F5,
    Key_F6 = SDLK_F6,
    Key_F7 = SDLK_F7,
    Key_F8 = SDLK_F8,
    Key_F9 = SDLK_F9,
    Key_F10 = SDLK_F10,
    Key_F11 = SDLK_F11,
    Key_F12 = SDLK_F12,
    
    Key_Printscreen = SDLK_PRINTSCREEN,
    Key_Scrolllock = SDLK_SCROLLLOCK,
    Key_Pause = SDLK_PAUSE,
    Key_Insert = SDLK_INSERT,
    Key_Home = SDLK_HOME,
    Key_Pageup = SDLK_PAGEUP,
    Key_Delete = SDLK_DELETE,
    Key_End = SDLK_END,
    Key_Pagedown = SDLK_PAGEDOWN,
    Key_Right = SDLK_RIGHT,
    Key_Left = SDLK_LEFT,
    Key_Down = SDLK_DOWN,
    Key_Up = SDLK_UP,
    
    Key_Numlockclear = SDLK_NUMLOCKCLEAR,
    Key_Kp_Divide = SDLK_KP_DIVIDE,
    Key_Kp_Multiply = SDLK_KP_MULTIPLY,
    Key_Kp_Minus = SDLK_KP_MINUS,
    Key_Kp_Plus = SDLK_KP_PLUS,
    Key_Kp_Enter = SDLK_KP_ENTER,
    Key_Kp_1 = SDLK_KP_1,
    Key_Kp_2 = SDLK_KP_2,
    Key_Kp_3 = SDLK_KP_3,
    Key_Kp_4 = SDLK_KP_4,
    Key_Kp_5 = SDLK_KP_5,
    Key_Kp_6 = SDLK_KP_6,
    Key_Kp_7 = SDLK_KP_7,
    Key_Kp_8 = SDLK_KP_8,
    Key_Kp_9 = SDLK_KP_9,
    Key_Kp_0 = SDLK_KP_0,
    Key_Kp_Period = SDLK_KP_PERIOD,
    
    Key_Application = SDLK_APPLICATION,
    Key_Power = SDLK_POWER,
    Key_Kp_Equals = SDLK_KP_EQUALS,
    Key_F13 = SDLK_F13,
    Key_F14 = SDLK_F14,
    Key_F15 = SDLK_F15,
    Key_F16 = SDLK_F16,
    Key_F17 = SDLK_F17,
    Key_F18 = SDLK_F18,
    Key_F19 = SDLK_F19,
    Key_F20 = SDLK_F20,
    Key_F21 = SDLK_F21,
    Key_F22 = SDLK_F22,
    Key_F23 = SDLK_F23,
    Key_F24 = SDLK_F24,
    Key_Execute = SDLK_EXECUTE,
    Key_Help = SDLK_HELP,
    Key_Menu = SDLK_MENU,
    Key_Select = SDLK_SELECT,
    Key_Stop = SDLK_STOP,
    Key_Again = SDLK_AGAIN,
    Key_Undo = SDLK_UNDO,
    Key_Cut = SDLK_CUT,
    Key_Copy = SDLK_COPY,
    Key_Paste = SDLK_PASTE,
    Key_Find = SDLK_FIND,
    Key_Mute = SDLK_MUTE,
    Key_Volumeup = SDLK_VOLUMEUP,
    Key_Volumedown = SDLK_VOLUMEDOWN,
    Key_Kp_Comma = SDLK_KP_COMMA,
    Key_Kp_Equalsas400 = SDLK_KP_EQUALSAS400,
    
    Key_Alterase = SDLK_ALTERASE,
    Key_Sysreq = SDLK_SYSREQ,
    Key_Cancel = SDLK_CANCEL,
    Key_Clear = SDLK_CLEAR,
    Key_Prior = SDLK_PRIOR,
    Key_Return2 = SDLK_RETURN2,
    Key_Separator = SDLK_SEPARATOR,
    Key_Out = SDLK_OUT,
    Key_Oper = SDLK_OPER,
    Key_Clearagain = SDLK_CLEARAGAIN,
    Key_Crsel = SDLK_CRSEL,
    Key_Exsel = SDLK_EXSEL,
    
    Key_Kp_00 = SDLK_KP_00,
    Key_Kp_000 = SDLK_KP_000,
    Key_Thousandsseparator = SDLK_THOUSANDSSEPARATOR,
    Key_Decimalseparator = SDLK_DECIMALSEPARATOR,
    Key_Currencyunit = SDLK_CURRENCYUNIT,
    Key_Currencysubunit = SDLK_CURRENCYSUBUNIT,
    Key_Kp_Leftparen = SDLK_KP_LEFTPAREN,
    Key_Kp_Rightparen = SDLK_KP_RIGHTPAREN,
    Key_Kp_Leftbrace = SDLK_KP_LEFTBRACE,
    Key_Kp_Rightbrace = SDLK_KP_RIGHTBRACE,
    Key_Kp_Tab = SDLK_KP_TAB,
    Key_Kp_Backspace = SDLK_KP_BACKSPACE,
    Key_Kp_A = SDLK_KP_A,
    Key_Kp_B = SDLK_KP_B,
    Key_Kp_C = SDLK_KP_C,
    Key_Kp_D = SDLK_KP_D,
    Key_Kp_E = SDLK_KP_E,
    Key_Kp_F = SDLK_KP_F,
    Key_Kp_Xor = SDLK_KP_XOR,
    Key_Kp_Power = SDLK_KP_POWER,
    Key_Kp_Percent = SDLK_KP_PERCENT,
    Key_Kp_Less = SDLK_KP_LESS,
    Key_Kp_Greater = SDLK_KP_GREATER,
    Key_Kp_Ampersand = SDLK_KP_AMPERSAND,
    Key_Kp_Dblampersand = SDLK_KP_DBLAMPERSAND,
    Key_Kp_Verticalbar = SDLK_KP_VERTICALBAR,
    Key_Kp_Dblverticalbar = SDLK_KP_DBLVERTICALBAR,
    Key_Kp_Colon = SDLK_KP_COLON,
    Key_Kp_Hash = SDLK_KP_HASH,
    Key_Kp_Space = SDLK_KP_SPACE,
    Key_Kp_At = SDLK_KP_AT,
    Key_Kp_Exclam = SDLK_KP_EXCLAM,
    Key_Kp_Memstore = SDLK_KP_MEMSTORE,
    Key_Kp_Memrecall = SDLK_KP_MEMRECALL,
    Key_Kp_Memclear = SDLK_KP_MEMCLEAR,
    Key_Kp_Memadd = SDLK_KP_MEMADD,
    Key_Kp_Memsubtract = SDLK_KP_MEMSUBTRACT,
    Key_Kp_Memmultiply = SDLK_KP_MEMMULTIPLY,
    Key_Kp_Memdivide = SDLK_KP_MEMDIVIDE,
    Key_Kp_Plusminus = SDLK_KP_PLUSMINUS,
    Key_Kp_Clear = SDLK_KP_CLEAR,
    Key_Kp_Clearentry = SDLK_KP_CLEARENTRY,
    Key_Kp_Binary = SDLK_KP_BINARY,
    Key_Kp_Octal = SDLK_KP_OCTAL,
    Key_Kp_Decimal = SDLK_KP_DECIMAL,
    Key_Kp_Hexadecimal = SDLK_KP_HEXADECIMAL,
    
    Key_Lctrl = SDLK_LCTRL,
    Key_Lshift = SDLK_LSHIFT,
    Key_Lalt = SDLK_LALT,
    Key_Lgui = SDLK_LGUI,
    Key_Rctrl = SDLK_RCTRL,
    Key_Rshift = SDLK_RSHIFT,
    Key_Ralt = SDLK_RALT,
    Key_Rgui = SDLK_RGUI,
    
    Key_Mode = SDLK_MODE,
    
    Key_Audionext = SDLK_AUDIONEXT,
    Key_Audioprev = SDLK_AUDIOPREV,
    Key_Audiostop = SDLK_AUDIOSTOP,
    Key_Audioplay = SDLK_AUDIOPLAY,
    Key_Audiomute = SDLK_AUDIOMUTE,
    Key_Mediaselect = SDLK_MEDIASELECT,
    Key_Www = SDLK_WWW,
    Key_Mail = SDLK_MAIL,
    Key_Calculator = SDLK_CALCULATOR,
    Key_Computer = SDLK_COMPUTER,
    Key_Ac_Search = SDLK_AC_SEARCH,
    Key_Ac_Home = SDLK_AC_HOME,
    Key_Ac_Back = SDLK_AC_BACK,
    Key_Ac_Forward = SDLK_AC_FORWARD,
    Key_Ac_Stop = SDLK_AC_STOP,
    Key_Ac_Refresh = SDLK_AC_REFRESH,
    Key_Ac_Bookmarks = SDLK_AC_BOOKMARKS,
    
    Key_Brightnessdown = SDLK_BRIGHTNESSDOWN,
    Key_Brightnessup = SDLK_BRIGHTNESSUP,
    Key_Displayswitch = SDLK_DISPLAYSWITCH,
    Key_Kbdillumtoggle = SDLK_KBDILLUMTOGGLE,
    Key_Kbdillumdown = SDLK_KBDILLUMDOWN,
    Key_Kbdillumup = SDLK_KBDILLUMUP,
    Key_Eject = SDLK_EJECT,
    Key_Sleep = SDLK_SLEEP,
    
    Key_MAX,

    Key_Ctrl, // merged keys
    Key_Shift,
    Key_Alt,

};

enum KeyScanCode
{

};

int getWebKeyFromKey(Key key);

} } //end namespace

#endif
