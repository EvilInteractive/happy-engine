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
//Author:  Sebastiaan Sprengers
//Created: 06/11/2011

#ifndef _HE_SIMPLE_COLOR_EFFECT_H_
#define _HE_SIMPLE_COLOR_EFFECT_H_
#pragma once

namespace he {
namespace gfx {

class Shader;

class SimpleColorEffect
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    SimpleColorEffect();
    virtual ~SimpleColorEffect();

    /* GENERAL */
    void load();
    void begin() const;
    void end() const;

    /* SETTERS */
    void setViewProjection(const mat44& mat);
    void setWorld(const mat44& mat);
    void setColor(const Color& color);

private:

    /* DATAMEMBERS */
    Shader* m_Shader;

    uint32 m_ShaderVPPos;
    uint32 m_ShaderWPos;
    uint32 m_ShaderColorPos;

    /* DEFAULT COPY & ASSIGNMENT */
    SimpleColorEffect(const SimpleColorEffect&);
    SimpleColorEffect& operator=(const SimpleColorEffect&);
};

} } //end namespace

#endif
