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
//Author:  Sebastiaan Sprengers
//Created: 11/10/2011

#ifndef _HE_STRING_TYPE_HANDLER_H_
#define _HE_STRING_TYPE_HANDLER_H_
#pragma once

#include "ITypeHandler.h"
#include <string>

namespace he {
namespace tools {

class StringTypeHandler : public ITypeHandler
{
public:

	StringTypeHandler() {}

	virtual ~StringTypeHandler() {}

	bool parse(const he::String& values, boost::any& pValueToAssign) const
	{
		he::String& pF = *boost::any_cast<he::String*>(pValueToAssign);

		he::String s(values);

		size_t firstQ(s.find("\""));
        size_t secondQ(s.find("\"", firstQ + 1));

		if (firstQ != -1 && secondQ != -1 && secondQ != firstQ + 1)
		{
			s = s.substr(firstQ + 1, (secondQ - firstQ) - 1);

			pF = s;
		}
		else
			return false;
	
		return true;
	}

	he::String getType() const
	{
		return typeid(he::String).name();
	}

    //Disable default copy constructor and default assignment operator
    StringTypeHandler(const StringTypeHandler&);
    StringTypeHandler& operator=(const StringTypeHandler&);
};

} } //end namespace

#endif
