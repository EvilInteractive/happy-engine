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

#ifndef _HE_I_TYPE_HANDLER_H_
#define _HE_I_TYPE_HANDLER_H_
#pragma once

#include "boost/any.hpp"
#include <typeinfo>
#include <vector>

namespace he {
namespace tools {

class ITypeHandler
{
public:

	virtual ~ITypeHandler() {}

	virtual bool parse(const std::string& values, boost::any& pValueToAssign) const = 0;
	virtual std::string getType() const = 0;
};

} } //end namespace

#endif
