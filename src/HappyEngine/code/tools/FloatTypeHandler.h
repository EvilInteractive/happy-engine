//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#ifndef _HE_FLOAT_TYPE_HANDLER_H_
#define _HE_FLOAT_TYPE_HANDLER_H_
#pragma once

#include "ITypeHandler.h"

namespace he {
namespace tools {

class FloatTypeHandler : public ITypeHandler
{
public:

	FloatTypeHandler() {}

	virtual ~FloatTypeHandler() {}

	bool parse(const he::String& values, boost::any& pValueToAssign) const
	{
		float i;

		if (sscanf(values.c_str(), "%f", &i) != 1)
			return false;
	
		float& pF = *boost::any_cast<float*>(pValueToAssign);

		pF = i;

		return true;
	}

	he::String getType() const
	{
		return typeid(float).name();
	}

    //Disable default copy constructor and default assignment operator
    FloatTypeHandler(const FloatTypeHandler&);
    FloatTypeHandler& operator=(const FloatTypeHandler&);
};

} } //end namespace

#endif
