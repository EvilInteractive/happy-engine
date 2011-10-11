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
//Author:  Sebastiaan Sprengers
//Created: 11/10/2011

#ifndef _HE_VEC2_TYPE_HANDLER_H_
#define _HE_VEC2_TYPE_HANDLER_H_
#pragma once

#include "ITypeHandler.h"
#include "vec2.h"

namespace he {
namespace tools {

class Vec2TypeHandler : public ITypeHandler
{
public:

	Vec2TypeHandler()  : ITypeHandler()
	{
		m_InputTypes.push_back(typeid(float).name());
		m_InputTypes.push_back(typeid(float).name());
	}

    virtual ~Vec2TypeHandler();

	void parse(const std::vector<boost::any>& values, boost::any& pValueToAssign) const
	{
		vec2 v(boost::any_cast<float>(values[0]), boost::any_cast<float>(values[1]));
	
		vec2& pV = *boost::any_cast<vec2*>(pValueToAssign);

		pV = v;
	}

	std::string getType() const
	{
		return typeid(vec2).name();
	}

private:

    //Disable default copy constructor and default assignment operator
    Vec2TypeHandler(const Vec2TypeHandler&);
    Vec2TypeHandler& operator=(const Vec2TypeHandler&);
};

} } //end namespace

#endif
