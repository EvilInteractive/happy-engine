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

class IFxComponent
{
public:
    virtual ~IFxComponent() {};
    
    virtual void setStartTime(float startTime) = 0;
    virtual void setEndTime(float endTime) = 0;
    virtual float getStartTime() const = 0;
    virtual float getEndTime() const = 0;

    virtual void start() = 0;

    virtual void tick(float currentTime, float dTime) = 0;

};

} } //end namespace

#endif
