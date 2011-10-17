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
//Created: 17/10/2011

#ifndef _HE_DIRECTIONAL_LIGHT_H_
#define _HE_DIRECTIONAL_LIGHT_H_
#pragma once

#include "vec3.h"
#include "mat44.h"
#include "Color.h"
#include "Model.h"

#include "boost/shared_ptr.hpp"

#include "IDrawable.h"

namespace he {
namespace gfx {

class DirectionalLight
{
private:
    float m_Multiplier;
    vec3 m_Color;
	vec3 m_Direction;

public:
    DirectionalLight();
    ~DirectionalLight();
    //default copy constructor and assignment operator are fine

    void setMultiplier(float multiplier);
    void setColor(const vec3& color);
    void setColor(const Color& color);
	void setDirection(const vec3& direction);

	void makeAwesomeGame(); 

    float getMultiplier() const;
    const vec3& getColor() const;
	const vec3& getDirection() const;

	typedef boost::shared_ptr<DirectionalLight> pointer;
    
};

} } //end namespace

#endif
