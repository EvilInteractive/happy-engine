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

#ifndef _HE_VEC3_TYPE_HANDLER_H_
#define _HE_VEC3_TYPE_HANDLER_H_
#pragma once

#include "ITypeHandler.h"
#include "vec3.h"

namespace he {
namespace tools {

class Vec3TypeHandler : public ITypeHandler
{
public:

	Vec3TypeHandler()
	{
		m_InputTypes.push_back(typeid(float).name());
		m_InputTypes.push_back(typeid(float).name());
		m_InputTypes.push_back(typeid(float).name());
	}

	virtual ~Vec3TypeHandler() {}

	bool parse(const std::string& values, boost::any& pValueToAssign) const
	{
		float i[3];

		if (sscanf(values.c_str(), "%f,%f,%f", &i[0], &i[1], &i[2]) != static_cast<int>(m_InputTypes.size()))
			return false;

		vec3 v(i[0],i[1],i[2]);
	
		vec3& pV = *boost::any_cast<vec3*>(pValueToAssign);

		pV = v;

		return true;
	}

	const std::vector<std::string>& getInputTypes() const
	{
		return m_InputTypes;
	}

	std::string getType() const
	{
		return typeid(vec3).name();
	}

private:

	std::vector<std::string> m_InputTypes;

    //Disable default copy constructor and default assignment operator
    Vec3TypeHandler(const Vec3TypeHandler&);
    Vec3TypeHandler& operator=(const Vec3TypeHandler&);
};

} } //end namespace

#endif
