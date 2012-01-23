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
//Created: 27/12/2011

#ifndef _HE_FX_PARTICLE_MODIFY_COMPONENT_H_
#define _HE_FX_PARTICLE_MODIFY_COMPONENT_H_
#pragma once

namespace he {
namespace gfx {

struct FxParticle;

class IFxParticleModifyComponent
{
public:
    virtual ~IFxParticleModifyComponent() {}

    virtual void transform(FxParticle* pParticle, float normTime, float dTime, const mat44& parentWorld) = 0; //normTime [0, 1]
};

} } //end namespace

#endif
