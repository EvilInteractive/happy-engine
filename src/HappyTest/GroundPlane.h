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

#ifndef _GROUND_PLANE_H_
#define _GROUND_PLANE_H_
#pragma once

#include "Camera.h"
#include "Entity.h"

namespace happytest {

class GroundPlane : public happyengine::game::Entity
{
public:
	GroundPlane(const happyengine::game::EntityDesc& desc);
    virtual ~GroundPlane();

private:
    
    //Disable default copy constructor and default assignment operator
    GroundPlane(const GroundPlane&);
    GroundPlane& operator=(const GroundPlane&);
};

} //end namespace

#endif
