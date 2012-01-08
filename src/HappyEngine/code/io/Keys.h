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

#include "SDL_keycode.h"

namespace he {
namespace io {

#define MAX_KEYS 512

enum Key
{
    Key_Unkown = SDLK_UNKNOWN,

    Key_Return = SDLK_RETURN,
    Key_Escape = SDLK_ESCAPE,
    Key_Backspace = SDLK_BACKSPACE,
    Key_Tab = SDLK_TAB,
    Key_Space = SDLK_SPACE,
    Key_Exclaim = SDLK_EXCLAIM,
    Key_QuoteDbl = SDLK_QUOTEDBL,
    Key_Hash = SDLK_HASH,
    Key_Percent = SDLK_PERCENT,
    Key_Dollar = SDLK_DOLLAR,
    Key_Ampersand = SDLK_AMPERSAND,
    Key_Quote = SDLK_QUOTE,
    Key_LeftParen = SDLK_LEFTPAREN,
    Key_RightParen = SDLK_RIGHTPAREN,
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
    Key_LeftBracket = SDLK_LEFTBRACKET,
    Key_Backslash = SDLK_BACKSLASH,
    Key_RightBracket = SDLK_RIGHTBRACKET,
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

    Key_PrintScreen = SDLK_PRINTSCREEN,
    Key_ScrollLock = SDLK_SCROLLLOCK,
    Key_Pause = SDLK_PAUSE,
    Key_Insert = SDLK_INSERT,
    Key_Home = SDLK_HOME,
    Key_PageUp = SDLK_PAGEUP,
    Key_Delete = SDLK_DELETE,
    Key_End = SDLK_END,
    Key_PageDown = SDLK_PAGEDOWN,
    Key_Right = SDLK_RIGHT,
    Key_Left = SDLK_LEFT,
    Key_Down = SDLK_DOWN,
    Key_Up = SDLK_UP,

    Key_NumlockClear = SDLK_NUMLOCKCLEAR,
    Key_KP_Divide = SDLK_KP_DIVIDE,
    Key_KP_Multiply = SDLK_KP_MULTIPLY,
    Key_KP_Minus = SDLK_KP_MINUS,
    Key_KP_Plus = SDLK_KP_PLUS,
    Key_KP_Enter = SDLK_KP_ENTER,
    Key_KP_1 = SDLK_KP_1,
    Key_KP_2 = SDLK_KP_2,
    Key_KP_3 = SDLK_KP_3,
    Key_KP_4 = SDLK_KP_4,
    Key_KP_5 = SDLK_KP_5,
    Key_KP_6 = SDLK_KP_6,
    Key_KP_7 = SDLK_KP_7,
    Key_KP_8 = SDLK_KP_8,
    Key_KP_9 = SDLK_KP_9,
    Key_KP_0 = SDLK_KP_0,
    Key_KP_Period = SDLK_KP_PERIOD,

    Key_Application = SDLK_APPLICATION,
    Key_Power = SDLK_POWER,
    Key_KP_Equals = SDLK_KP_EQUALS,
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
    Key_KP_Comma = SDLK_KP_COMMA,
    Key_KP_EqualsAs400 = SDLK_KP_EQUALSAS400,

    Key_AltErase = SDLK_ALTERASE,
    Key_Sysreq = SDLK_SYSREQ,
    Key_Cancel = SDLK_CANCEL,
    Key_Clear = SDLK_CLEAR,
    Key_Prior = SDLK_PRIOR,
    Key_Return2 = SDLK_RETURN2,
    Key_Separator = SDLK_SEPARATOR,
    Key_Out = SDLK_OUT,
    Key_Oper = SDLK_OPER,
    Key_ClearAgain = SDLK_CLEARAGAIN,
    Key_Crsel = SDLK_CRSEL,
    Key_Exsel = SDLK_EXSEL,

    Key_KP_00 = SDLK_KP_00,
    Key_KP_000 = SDLK_KP_000,
    Key_ThousandsSeparator = SDLK_THOUSANDSSEPARATOR,
    Key_DecimalSeparator = SDLK_DECIMALSEPARATOR,
    Key_CurrencyUnit = SDLK_CURRENCYUNIT,
    Key_CurrencySubUnit = SDLK_CURRENCYSUBUNIT,
    Key_KP_LeftParen = SDLK_KP_LEFTPAREN,
    Key_KP_RightParen = SDLK_KP_RIGHTPAREN,
    Key_KP_LeftBrace = SDLK_KP_LEFTBRACE,
    Key_KP_RightBrace = SDLK_KP_RIGHTBRACE,
    Key_KP_Tab = SDLK_KP_TAB,
    Key_KP_Backspace = SDLK_KP_BACKSPACE,
    Key_KP_A = SDLK_KP_A,
    Key_KP_B = SDLK_KP_B,
    Key_KP_C = SDLK_KP_C,
    Key_KP_D = SDLK_KP_D,
    Key_KP_E = SDLK_KP_E,
    Key_KP_F = SDLK_KP_F,
    Key_KP_Xor = SDLK_KP_XOR,
    Key_KP_Power = SDLK_KP_POWER,
    Key_KP_Percent = SDLK_KP_PERCENT,
    Key_KP_Less = SDLK_KP_LESS,
    Key_KP_Greater = SDLK_KP_GREATER,
    Key_KP_Ampersand = SDLK_KP_AMPERSAND,
    Key_KP_DblAmpersand = SDLK_KP_DBLAMPERSAND,
    Key_KP_VerticalBar = SDLK_KP_VERTICALBAR,
    Key_KP_DblVerticalBar = SDLK_KP_DBLVERTICALBAR,
    Key_KP_Colon = SDLK_KP_COLON,
    Key_KP_Hash = SDLK_KP_HASH,
    Key_KP_Space = SDLK_KP_SPACE,
    Key_KP_At = SDLK_KP_AT,
    Key_KP_Exclam = SDLK_KP_EXCLAM,
    Key_KP_MemStore = SDLK_KP_MEMSTORE,
    Key_KP_MemRecall = SDLK_KP_MEMRECALL,
    Key_KP_MemClear = SDLK_KP_MEMCLEAR,
    Key_KP_MemAdd = SDLK_KP_MEMADD,
    Key_KP_MemSubtract = SDLK_KP_MEMSUBTRACT,
    Key_KP_MemMultiply = SDLK_KP_MEMMULTIPLY,
    Key_KP_MemDivide = SDLK_KP_MEMDIVIDE,
    Key_KP_PlusMinus = SDLK_KP_PLUSMINUS,
    Key_KP_Clear = SDLK_KP_CLEAR,
    Key_KP_Clearentry = SDLK_KP_CLEARENTRY,
    Key_KP_Binary = SDLK_KP_BINARY,
    Key_KP_Octal = SDLK_KP_OCTAL,
    Key_KP_Decimal = SDLK_KP_DECIMAL,
    Key_KP_Hexadecimal = SDLK_KP_HEXADECIMAL,

    Key_Lctrl = SDLK_LCTRL,
    Key_Lshift = SDLK_LSHIFT,
    Key_Lalt = SDLK_LALT,
    Key_Lgui = SDLK_LGUI,
    Key_Rctrl = SDLK_RCTRL,
    Key_Rshift = SDLK_RSHIFT,
    Key_Ralt = SDLK_RALT,
    Key_Rgui = SDLK_RGUI,

    Key_Mode = SDLK_MODE,

    Key_AudioNext = SDLK_AUDIONEXT,
    Key_AudioPrev = SDLK_AUDIOPREV,
    Key_AudioStop = SDLK_AUDIOSTOP,
    Key_AudioPlay = SDLK_AUDIOPLAY,
    Key_AudioMute = SDLK_AUDIOMUTE,
    Key_MediaSelect = SDLK_MEDIASELECT,
    Key_WWW = SDLK_WWW,
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

    Key_BrightnessDown = SDLK_BRIGHTNESSDOWN,
    Key_BrightnessUp = SDLK_BRIGHTNESSUP,
    Key_DisplayDwitch = SDLK_DISPLAYSWITCH,
    Key_KbdIllumToggle = SDLK_KBDILLUMTOGGLE,
    Key_KbdIllumDown = SDLK_KBDILLUMDOWN,
    Key_KbdIllumUp = SDLK_KBDILLUMUP,
    Key_Eject = SDLK_EJECT,
    Key_Sleep = SDLK_SLEEP,
};

enum KeyScanCode
{

};

} } //end namespace

#endif
