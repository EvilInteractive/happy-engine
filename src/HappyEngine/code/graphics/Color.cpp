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
//Author:  Bastian Damman
//Created: 05/08/2011
#include "HappyPCH.h" 

#include "Color.h"
#include "MathFunctions.h"

namespace he {

Color::Color(): m_rgb(1.0f, 1.0f, 1.0f), m_a(1.0f)
{
}
Color::Color(float red, float green, float blue, float alpha): m_rgb(red, green, blue), m_a(alpha)
{
}
Color::Color(uint8 red, uint8 green, uint8 blue, uint8 alpha) : m_rgb(red / 255.0f, green / 255.0f, blue / 255.0f), 
                                                            m_a(alpha / 255.0f)
{
}
Color::Color(const vec4& col): m_rgb(col.xyz()), m_a(col.w)
{
}


Color::~Color()
{
}

//-----------------------------------------//
//                GETTERS                  // 
//-----------------------------------------//
float Color::r() const
{
    return m_rgb.x;
}
float Color::g() const
{
    return m_rgb.y;
}
float Color::b() const
{
    return m_rgb.z;
}
float Color::a() const
{
    return m_a;
}
const vec3& Color::rgb() const
{
    return m_rgb;
}
vec4 Color::rgba() const
{
    return vec4(m_rgb, m_a);
}


uint8 Color::rByte() const
{
    return static_cast<uint8>(clamp(m_rgb.x * 255.0f, 0.0f, 255.0f));
}
uint8 Color::gByte() const
{
    return static_cast<uint8>(clamp(m_rgb.y * 255, 0.0f, 255.0f));
}
uint8 Color::bByte() const
{
    return static_cast<uint8>(clamp(m_rgb.z * 255, 0.0f, 255.0f));
}
uint8 Color::aByte() const
{
    return static_cast<uint8>(clamp(m_a * 255, 0.0f, 255.0f));
}
    
//-----------------------------------------//
//                SETTERS                  //
//-----------------------------------------//
void Color::r(float r)
{
    m_rgb.x = r;
}
void Color::g(float g)
{
    m_rgb.y = g;
}
void Color::b(float b)
{
    m_rgb.z = b;
}
void Color::a(float a)
{
    m_a = a;
}
void Color::rgb(const vec3& rgb)
{
    m_rgb = rgb;
}
void Color::rgba(const vec4& rgba)
{
    m_rgb = rgba.xyz();
    m_a = rgba.w;
}

void Color::r(uint8 r)
{
    m_rgb.x = r / 255.0f;
}
void Color::g(uint8 g)
{
    m_rgb.y = g / 255.0f;
}
void Color::b(uint8 b)
{
    m_rgb.z = b / 255.0f;
}
void Color::a(uint8 a)
{
    m_a = a / 255.0f;
}
char itoax(const uint8 i)
{
    HE_ASSERT(i >= 0 && i < 16, "Invalid range %d", i);
    char result;
    if (i > 9)
        result = (i - 10) + 65;
    else
        result = i + 48;
    return result;
}
char Color::r16() const
{
    return itoax(rByte() / 16);
}
char Color::g16() const
{
    return itoax(gByte() / 16);
}
char Color::b16() const
{
    return itoax(bByte() / 16);
}
char Color::a16() const
{
    return itoax(aByte() / 16);
}
//-----------------------------------------//
//                Operators                //
//-----------------------------------------//
bool Color::operator==(const Color& other) const
{
    return m_a == other.m_a && m_rgb == other.m_rgb;
}
bool Color::operator!=(const Color& other) const
{
    return m_a != other.m_a || m_rgb != other.m_rgb;
}

} //end namespace