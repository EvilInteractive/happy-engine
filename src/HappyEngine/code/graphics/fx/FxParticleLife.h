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
//Created: 19/01/2012

#ifndef _HE_FX_PARTICLE_LIFE_H_
#define _HE_FX_PARTICLE_LIFE_H_
#pragma once

#include "IFxParticleInitComponent.h"
#include "IFxVariable.h"
#include "vec3.h"

namespace he {
namespace gfx {

class FxParticleLife : public IFxParticleInitComponent
{
public:
    FxParticleLife();
    virtual ~FxParticleLife();

    virtual void init(FxParticle* pParticle, const mat44& parentWorld);

    void setValue(const IFxVariable<float>::pointer& value);
    const IFxVariable<float>::pointer& getValue() const;

private:
    IFxVariable<float>::pointer m_Life;

    //Disable default copy constructor and default assignment operator
    FxParticleLife(const FxParticleLife&);
    FxParticleLife& operator=(const FxParticleLife&);
};

} } //end namespace

#endif
