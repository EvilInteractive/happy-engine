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
//Created: 17/08/2011

#ifndef _HE_LIGHT_H_
#define _HE_LIGHT_H_
#pragma once

#include "vec3.h"
#include "boost/shared_ptr.hpp"
#include "Rect.h"
#include "Camera.h"
#include "MathFunctions.h"

#include "SpotLight.h"
#include "PointLight.h"

namespace he {
namespace gfx {

class AmbientLight
{
public:

    vec3 position;
    float multiplier;
    vec3 color;
    float range;

    typedef boost::shared_ptr<AmbientLight> pointer;
};
class DirectionalLight
{
public:

    vec3 direction;
    vec3 color;
    float multiplier;

    typedef boost::shared_ptr<DirectionalLight> pointer;
};

} } //end namespace

#endif
