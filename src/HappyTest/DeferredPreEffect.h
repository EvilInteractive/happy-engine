//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _DEFERRED_PRE_EFFECT_H_
#define _DEFERRED_PRE_EFFECT_H_
#pragma once
#include "HappyTypes.h"
#include "Shader.h"

#include "Matrix.h"
#include "Vector3.h"
#include "Texture2D.h"

namespace happytest {

class DeferredPreEffect
{
public:
	DeferredPreEffect();
    virtual ~DeferredPreEffect();

    void load();
    void begin() const;
    void end() const;

    void setWVP(const happyengine::math::Matrix& wvp) const;
    void setWorld(const happyengine::math::Matrix& wvp) const;
    void setDiffuseMap(const happyengine::graphics::Texture2D::pointer& diffuseMap) const;

private:
    happyengine::graphics::Shader* m_pShader;

    happyengine::uint m_ShaderWVPpos;
    happyengine::uint m_ShaderWorldPos;
    happyengine::uint m_ShaderDiffTexPos;

    //Disable default copy constructor and default assignment operator
    DeferredPreEffect(const DeferredPreEffect&);
    DeferredPreEffect& operator=(const DeferredPreEffect&);
};

} //end namespace

#endif
