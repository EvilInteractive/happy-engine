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

#ifndef _HE_UINT_TYPE_HANDLER_H_
#define _HE_UINT_TYPE_HANDLER_H_
#pragma once

#include "ITypeHandler.h"
#include "HappyTypes.h"

namespace he {
namespace tools {

class UIntTypeHandler : public ITypeHandler
{
public:

	UIntTypeHandler() : ITypeHandler()
	{
		m_InputTypes.push_back(typeid(uint).name());
	}

    virtual ~UIntTypeHandler();

	void parse(const std::vector<boost::any>& values, boost::any& pValueToAssign) const
	{
		uint i(boost::any_cast<uint>(values[0]));
	
		uint& pI = *boost::any_cast<uint*>(pValueToAssign);

		pI = i;
	}

	std::string getType() const
	{
		return typeid(uint).name();
	}

private:

    //Disable default copy constructor and default assignment operator
    UIntTypeHandler(const UIntTypeHandler&);
    UIntTypeHandler& operator=(const UIntTypeHandler&);
};

} } //end namespace

#endif
