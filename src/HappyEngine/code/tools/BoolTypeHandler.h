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

#ifndef _HE_BOOL_TYPE_HANDLER_H_
#define _HE_BOOL_TYPE_HANDLER_H_
#pragma once

#include "ITypeHandler.h"
#include <string>
#include <algorithm>

namespace he {
namespace tools {

class BoolTypeHandler :	public ITypeHandler
{
public:

	BoolTypeHandler() {}

	virtual ~BoolTypeHandler() {}

	bool parse(const he::String& values, boost::any& pValueToAssign) const
	{
		bool& pB = *boost::any_cast<bool*>(pValueToAssign);

		he::String s(values);

		//s.erase(std::remove_if(s.begin(), s.end(), std::isspace), s.end());
		std::transform(s.begin(), s.end(), s.begin(),tolower);

		if (s == "true")
			pB = true;
		else if (s == "false")
			pB = false;
		else
		{
			int i;

			if (sscanf(values.c_str(), "%d", &i) != 1)
				return false;

			if (i == 0)
				pB = false;
			else if (i > 0)
				pB = true;
		}

		return true;
	}

	he::String getType() const
	{
		return typeid(bool).name();
	}

    //Disable default copy constructor and default assignment operator
    BoolTypeHandler(const BoolTypeHandler&);
    BoolTypeHandler& operator=(const BoolTypeHandler&);
};

} } //end namespace

#endif
