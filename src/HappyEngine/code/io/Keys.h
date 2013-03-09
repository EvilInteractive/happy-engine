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
#include "SFML/Window/Keyboard.hpp"

namespace he {
namespace io {

#define MAX_KEYS 512
#define mapWebKey(a, b) case Key_##a: return Awesomium::KeyCodes::AK_##b;

enum Key
{
    // Numbers
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

    // Letters
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

    // F's
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

    // Special
    Key_Escape = sf::Keyboard::Escape,
    Key_LSystem = sf::Keyboard::LSystem,
    Key_RSystem = sf::Keyboard::RSystem,
    Key_Menu = sf::Keyboard::Menu,
    Key_Return = sf::Keyboard::Return,
    Key_BackSpace = sf::Keyboard::BackSpace,
    Key_Tab = sf::Keyboard::Tab,
    Key_PageUp = sf::Keyboard::PageUp,
    Key_PageDown = sf::Keyboard::PageDown,
    Key_End = sf::Keyboard::End,
    Key_Home = sf::Keyboard::Home,
    Key_Insert = sf::Keyboard::Insert,
    Key_Delete = sf::Keyboard::Delete,

    // Signs
    Key_LBracket = sf::Keyboard::LBracket,   
    Key_RBracket = sf::Keyboard::RBracket,   
    Key_SemiColon = sf::Keyboard::SemiColon,  
    Key_Comma = sf::Keyboard::Comma,      
    Key_Period = sf::Keyboard::Period,     
    Key_Quote = sf::Keyboard::Quote,      
    Key_Slash = sf::Keyboard::Slash,      
    Key_BackSlah = sf::Keyboard::BackSlash,  
    Key_Tilde = sf::Keyboard::Tilde,      
    Key_Equal = sf::Keyboard::Equal,      
    Key_Dash = sf::Keyboard::Dash,       
    Key_Space = sf::Keyboard::Space,    
    Key_Add = sf::Keyboard::Add,        
    Key_Subtract = sf::Keyboard::Subtract,   
    Key_Multiply = sf::Keyboard::Multiply,   
    Key_Divide = sf::Keyboard::Divide,  

    // Arrow
    Key_Left = sf::Keyboard::Left,       
    Key_Right = sf::Keyboard::Right,      
    Key_Up = sf::Keyboard::Up,         
    Key_Down = sf::Keyboard::Down,       

    // Keypad
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

    // Mod
    Key_Lctrl = sf::Keyboard::LControl,
    Key_Lshift = sf::Keyboard::LShift,
    Key_Lalt = sf::Keyboard::LAlt,
    Key_Rctrl = sf::Keyboard::RControl,
    Key_Rshift = sf::Keyboard::RShift,
    Key_Ralt = sf::Keyboard::RAlt,


    Key_Ctrl, // merged keys
    Key_Shift,
    Key_Alt,

    Key_MAX = sf::Keyboard::KeyCount
};

enum KeyScanCode
{

};

int getWebKeyFromKey(Key key);

} } //end namespace

#endif
