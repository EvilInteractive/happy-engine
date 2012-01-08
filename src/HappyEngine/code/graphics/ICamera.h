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
//Author:  Bastian Damman
//Created: 18/12/2011

#ifndef _HE_ICAMERA_H_
#define _HE_ICAMERA_H_
#pragma once

namespace he {
class mat44;

namespace gfx {

class ICamera
{
public:
    virtual ~ICamera() {}
    
    virtual const mat44& getView() const = 0;
    virtual const mat44& getProjection() const = 0;
    virtual const mat44& getViewProjection() const = 0;

    virtual float getNearClip() const = 0;
    virtual float getFarClip() const = 0;

    virtual vec3 getPosition() const = 0;
    virtual const vec3& getLook() const = 0;

};  

} } //end namespace

#endif
