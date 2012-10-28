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
//Author:  Bastian Damman
//Created: 28/10/2012

#ifndef _HE_EVENT_H_
#define _HE_EVENT_H_
#pragma once

namespace he {
namespace details {

template<typename T>
bool defaultEventCombiner(T& inoutA, const T& inB)
{
    inoutA = inB;
    return false;
}

}

#define ARGS 0
#include "event_internal.h"

#define ARGS 1
#define TEMPLATE_EXTRA_ARGS typename Arg1Type
#define DECL_PARAMS const Arg1Type& arg1
#define PARAMS arg1
#include "event_internal.h"

#define ARGS 2
#define TEMPLATE_EXTRA_ARGS typename Arg1Type, typename Arg2Type
#define DECL_PARAMS const Arg1Type& arg1, const Arg2Type& arg2
#define PARAMS arg1, arg2
#include "event_internal.h"

}

#endif
