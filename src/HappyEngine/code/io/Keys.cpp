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
//Author:  Sebastiaan Sprengers
//Created: 15/04/2012

#include "HappyPCH.h" 

#include "Keys.h"

namespace he {
namespace io {

int getWebKeyFromKey(Key key)
{
    switch (key)
    {
        mapWebKey(0, 0)
        mapWebKey(1, 1)
        mapWebKey(2, 2)
        mapWebKey(3, 3)
        mapWebKey(4, 4)
        mapWebKey(5, 5)
        mapWebKey(6, 6)
        mapWebKey(7, 7)
        mapWebKey(8, 8)
        mapWebKey(9, 9)
        mapWebKey(A, A)
        mapWebKey(B, B)
        mapWebKey(C, C)
        mapWebKey(D, D)
        mapWebKey(E, E)
        mapWebKey(F, F)
        mapWebKey(G, G)
        mapWebKey(H, H)
        mapWebKey(I, I)
        mapWebKey(J, J)
        mapWebKey(K, K)
        mapWebKey(L, L)
        mapWebKey(M, M)
        mapWebKey(N, N)
        mapWebKey(O, O)
        mapWebKey(P, P)
        mapWebKey(Q, Q)
        mapWebKey(R, R)
        mapWebKey(S, S)
        mapWebKey(T, T)
        mapWebKey(U, U)
        mapWebKey(V, V)
        mapWebKey(W, W)
        mapWebKey(X, X)
        mapWebKey(Y, Y)
        mapWebKey(Z, Z)
        mapWebKey(Up, UP)
        mapWebKey(Down, DOWN)
        mapWebKey(Right, RIGHT)
        mapWebKey(Left, LEFT)
        mapWebKey(Return, RETURN)
        mapWebKey(Escape, ESCAPE)
        mapWebKey(Delete, DELETE)
        mapWebKey(Space, SPACE)
        mapWebKey(Backspace, BACK)
        mapWebKey(Tab, TAB)
        mapWebKey(Kp0, NUMPAD0)
        mapWebKey(Kp1, NUMPAD1)
        mapWebKey(Kp2, NUMPAD2)
        mapWebKey(Kp3, NUMPAD3)
        mapWebKey(Kp4, NUMPAD4)
        mapWebKey(Kp5, NUMPAD5)
        mapWebKey(Kp6, NUMPAD6)
        mapWebKey(Kp7, NUMPAD7)
        mapWebKey(Kp8, NUMPAD8)
        mapWebKey(Kp9, NUMPAD9)
        mapWebKey(Rshift, RSHIFT)
        mapWebKey(Lshift, LSHIFT)
        mapWebKey(Rctrl, RCONTROL)
        mapWebKey(Lctrl, LCONTROL)
        mapWebKey(Ralt, RMENU)
        mapWebKey(Lalt, LMENU)
        mapWebKey(F1, F1)
        mapWebKey(F2, F2)
        mapWebKey(F3, F3)
        mapWebKey(F4, F4)
        mapWebKey(F5, F5)
        mapWebKey(F6, F6)
        mapWebKey(F7, F7)
        mapWebKey(F8, F8)
        mapWebKey(F9, F9)
        mapWebKey(F10, F10)
        mapWebKey(F11, F11)
        mapWebKey(F12, F12)
    default:
        return Awesomium::KeyCodes::AK_UNKNOWN;
    }
}

} } //end namespace