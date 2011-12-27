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
//Created: 27/11/2011

#ifndef _HE_IFX_VARIABLE_H_
#define _HE_IFX_VARIABLE_H_
#pragma once

#include "boost/shared_ptr.hpp"

namespace he {
namespace gfx {

template<typename T>
class IFxVariable
{
public:
    virtual ~IFxVariable() {}

    virtual T getValue(float /*normTime*/) const = 0; //normTime [0, 1]

    typedef boost::shared_ptr<IFxVariable<T>> pointer;
};

} } //end namespace

#endif
