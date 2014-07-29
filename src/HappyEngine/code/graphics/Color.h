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

#ifndef _HE_COLOR_H_
#define _HE_COLOR_H_
#pragma once

#include "Color.h"
#include "HappyTypes.h"
#include "vec3.h"
#include "vec4.h"

namespace he {
    
struct HAPPY_ENTRY Color
{
private:
    vec3 m_rgb;
    float m_a;

public:
    Color();
    Color(float r, float g, float b, float a = 1.0f);
    Color(uint8 r, uint8 g, uint8 b, uint8 a = 255);
    Color(const vec4& col);
    ~Color();
    //Default copy constructor and assignment operator are fine

    static Color fromHSB(float hue, float saturation, float brightness);

    //-----------------------------------------//
    //                GETTERS                  // 
    //-----------------------------------------//
    float r() const;
    float g() const;
    float b() const;
    float a() const;
    const vec3& rgb() const;
    vec4 rgba() const;

    uint8 rByte() const;
    uint8 gByte() const;
    uint8 bByte() const;
    uint8 aByte() const;

    char r16() const;
    char g16() const;
    char b16() const;
    char a16() const;

    //-----------------------------------------//
    //                SETTERS                  //
    //-----------------------------------------//
    void r(float r);
    void g(float g);
    void b(float b);
    void a(float a);
    void rgb(const vec3& rgb);
    void rgba(const vec4& rgba);

    void r(uint8 r);
    void g(uint8 g);
    void b(uint8 b);
    void a(uint8 a);


    //-----------------------------------------//
    //                Operators                //
    //-----------------------------------------//
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;
};

} //end namespace

#endif
