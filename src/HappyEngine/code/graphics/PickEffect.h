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
//Author:  Sebastiaan Sprengers
//Created: 11/11/2011

#ifndef _HE_PICK_EFFECT_H_
#define _HE_PICK_EFFECT_H_
#pragma once

#include "mat44.h"
#include "Shader.h"
#include "HappyTypes.h"

namespace he {
namespace gfx {

class PickEffect
{
public:

    /* CONSTRUCTOR - DESCTRUCTOR */
    PickEffect();
    virtual ~PickEffect();

    /* GENERAL */
    void load();
    void begin() const;
    void end() const;

    /* SETTERS */
    void setViewProjection(const mat44& mat);
    void setWorld(const mat44& mat);
    void setID(const vec4& id);

private:

    /* DATAMEMBERS */
    Shader* m_pShader;

    uint m_ShaderVPPos;
    uint m_ShaderWPos;
    uint m_ShaderIDPos;

    /* DEFAULT COPY & ASSIGNMENT */
    PickEffect(const PickEffect&);
    PickEffect& operator=(const PickEffect&);
};

} } //end namespace

#endif
