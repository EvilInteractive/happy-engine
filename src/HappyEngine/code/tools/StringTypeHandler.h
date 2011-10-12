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

	StringTypeHandler()
	{
		m_InputTypes.push_back(typeid(std::string).name());
	}

	virtual ~StringTypeHandler() {}

	bool parse(const std::string& values, boost::any& pValueToAssign) const
	{
		char i[256];

		if (sscanf(values.c_str(), "%s", i) != static_cast<int>(m_InputTypes.size()))
			return false;
	
		std::string& pF = *boost::any_cast<std::string*>(pValueToAssign);

		pF = std::string(i);

		return true;
	}

	const std::vector<std::string>& getInputTypes() const
	{
		return m_InputTypes;
	}

	std::string getType() const
	{
		return typeid(std::string).name();
	}

private:

	std::vector<std::string> m_InputTypes;

    //Disable default copy constructor and default assignment operator
    StringTypeHandler(const StringTypeHandler&);
    StringTypeHandler& operator=(const StringTypeHandler&);
};

} } //end namespace

#endif
