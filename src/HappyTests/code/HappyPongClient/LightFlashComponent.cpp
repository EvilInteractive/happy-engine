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
#include "HappyPongClientPCH.h" 

#include "LightFlashComponent.h"
#include "Game.h"

namespace hpc {

LightFlashComponent::LightFlashComponent():
    m_LightFlashMult(50),
    m_LightNormalMult(1),
    m_LightAnimSpeed(5.0f),
    m_LightAnim(0.0f)
{
    GAME->addToTickList(this);
}


LightFlashComponent::~LightFlashComponent()
{
    GAME->removeFromTickList(this);
}

void LightFlashComponent::tick( float dTime )
{
    setMultiplier(he::lerp(m_LightNormalMult, m_LightFlashMult, m_LightAnim));
    if (m_LightAnim > 0.0f)
    {
        m_LightAnim -= m_LightAnimSpeed * dTime;
        if (m_LightAnim < 0.0f)
            m_LightAnim = 0.0f;
    }
}

void LightFlashComponent::setFlashMultiplier( float mult )
{
    m_LightFlashMult = mult;
}

void LightFlashComponent::setNormalMultiplier( float mult )
{
    m_LightNormalMult = mult;
}

void LightFlashComponent::flash()
{
    m_LightAnim = 1.0f;
}

void LightFlashComponent::setFlashDuration( float duration )
{
    m_LightAnimSpeed = 1 / duration;
}

} //end namespace