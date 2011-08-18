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

#ifndef _LIGHT_H_
#define _LIGHT_H_
#pragma once

#include "Vector3.h"
#include "boost/shared_ptr.hpp"

namespace happyengine {
namespace graphics {

class AmbientLight
{
public:

    math::Vector3 position;
    float multiplier;
    math::Vector3 color;
    float range;

    typedef boost::shared_ptr<AmbientLight> pointer;
};
class PointLight
{
public:

    math::Vector3 position;
    float multiplier;
    math::Vector3 color;
    float beginAttenuation;
    float endAttenuation;

    typedef boost::shared_ptr<PointLight> pointer;
};
class SpotLight
{
public:

    math::Vector3 position;
    float multiplier;
    math::Vector3 direction;
    float beginAttenuation;
    math::Vector3 color;
    float endAttenuation;
    float cosCutoff;

    typedef boost::shared_ptr<SpotLight> pointer;
};
class DirectionalLight
{
public:

    math::Vector3 direction;
    math::Vector3 color;
    float multiplier;

    typedef boost::shared_ptr<DirectionalLight> pointer;
};

} } //end namespace

#endif
