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

#ifndef _HE_VEC4_TYPE_HANDLER_H_
#define _HE_VEC4_TYPE_HANDLER_H_
#pragma once

#include "ITypeHandler.h"
#include "vec4.h"

namespace he {
namespace tools  {

class Vec4TypeHandler : public ITypeHandler
{
public:

	Vec4TypeHandler() : ITypeHandler()
	{
		m_InputTypes.push_back(typeid(float).name());
		m_InputTypes.push_back(typeid(float).name());
		m_InputTypes.push_back(typeid(float).name());
		m_InputTypes.push_back(typeid(float).name());
	}

    virtual ~Vec4TypeHandler();

	void parse(const std::vector<boost::any>& values, boost::any& pValueToAssign) const
	{
		vec4 v(boost::any_cast<float>(values[0]), boost::any_cast<float>(values[1]),
				boost::any_cast<float>(values[2]), boost::any_cast<float>(values[2]));
	
		vec4& pV = *boost::any_cast<vec4*>(pValueToAssign);

		pV = v;
	}

	std::string getType() const
	{
		return typeid(vec4).name();
	}

private:

    //Disable default copy constructor and default assignment operator
    Vec4TypeHandler(const Vec4TypeHandler&);
    Vec4TypeHandler& operator=(const Vec4TypeHandler&);
};

} } //end namespace

#endif
