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
//Created: 27/11/2011

#ifndef _HE_FX_RANDOM_FLOAT_H_
#define _HE_FX_RANDOM_FLOAT_H_
#pragma once

#include "IFxVariable.h"
#include "Random.h"

namespace he {
namespace gfx {

class FxRandomFloat : public IFxVariable<float>
{
public:
    FxRandomFloat();
    FxRandomFloat(float min, float max);
    virtual ~FxRandomFloat();

    typedef boost::shared_ptr<FxRandomFloat> pointer;
    //////////////////////////////////////////////////////////////////////////
    ///                             IFxVariable                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual float getValue(float normTime) const;

    //////////////////////////////////////////////////////////////////////////
    ///                             Properties                             ///
    //////////////////////////////////////////////////////////////////////////
    void setMin(float min);
    void setMax(float max);
    void setSeed(uint seed);

private:

    float m_Min, m_Max;
    Random m_Random;

    //Disable default copy constructor and default assignment operator
    FxRandomFloat(const FxRandomFloat&);
    FxRandomFloat& operator=(const FxRandomFloat&);
};
class FxRandomVec3 : public IFxVariable<vec3>
{
public:
    FxRandomVec3();
    FxRandomVec3(const vec3& min, const vec3& max);
    virtual ~FxRandomVec3();

    typedef boost::shared_ptr<FxRandomVec3> pointer;
    //////////////////////////////////////////////////////////////////////////
    ///                             IFxVariable                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual vec3 getValue(float normTime) const;

    //////////////////////////////////////////////////////////////////////////
    ///                             Properties                             ///
    //////////////////////////////////////////////////////////////////////////
    void setMin(vec3 min);
    void setMax(vec3 max);
    void setSeed(uint seed);

private:

    vec3 m_Min, m_Max;
    Random m_Random;

    //Disable default copy constructor and default assignment operator
    FxRandomVec3(const FxRandomVec3&);
    FxRandomVec3& operator=(const FxRandomVec3&);
};

} } //end namespace

#endif
