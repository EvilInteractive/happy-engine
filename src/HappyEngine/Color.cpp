//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 05/08/2011
#include "Color.h"

namespace happyengine {

Color::Color()
{
}
Color::Color(float red, float green, float blue, float alpha): r(red), g(green), b(blue), a(alpha)
{
}
Color::Color(byte red, byte green, byte blue, byte alpha) : r(red / 255.0f), g(green / 255.0f), 
                                                   b(blue / 255.0f), a(alpha / 255.0f)
{
}


Color::~Color()
{
}

} //end namespace