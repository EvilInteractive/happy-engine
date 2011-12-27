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

#ifndef _HE_IFX_COMPONENT_H_
#define _HE_IFX_COMPONENT_H_
#pragma once

namespace he {
namespace gfx {

enum FxType
{
    FxType_ParticleSystem = 0,
    FxType_CameraEffect
};
class IFxComponent
{
public:

    virtual ~IFxComponent() {};
    
    virtual FxType getType() const = 0;

    virtual void setStartTime(float startTime) { m_StartTime = startTime; }
    virtual void setEndTime(float endTime) { m_EndTime = endTime; } 
    virtual float getStartTime() const { return m_StartTime; }
    virtual float getEndTime() const { return m_EndTime; }

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void tick(float currentTime, float dTime) = 0;

private:
    float m_StartTime, m_EndTime;
};

} } //end namespace

#endif
