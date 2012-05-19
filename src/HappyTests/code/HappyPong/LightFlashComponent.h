//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 31/03/2012

#ifndef _HT_LightFlashComponent_H_
#define _HT_LightFlashComponent_H_
#pragma once

#include "LightComponent.h"

namespace ht {

class LightFlashComponent : public he::ge::PointLightComponent
{
public:
    LightFlashComponent();
    virtual ~LightFlashComponent();

    virtual void tick(float dTime);

    virtual void setFlashMultiplier(float mult);
    virtual void setNormalMultiplier(float mult);
    virtual void setFlashDuration(float duration);

    virtual void flash();

private:

    float m_LightFlashMult;
    float m_LightNormalMult;
    float m_LightAnimSpeed;
    float m_LightAnim;

    //Disable default copy constructor and default assignment operator
    LightFlashComponent(const LightFlashComponent&);
    LightFlashComponent& operator=(const LightFlashComponent&);
};

} //end namespace

#endif
