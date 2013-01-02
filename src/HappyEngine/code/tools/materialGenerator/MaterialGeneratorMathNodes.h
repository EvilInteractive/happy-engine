//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 15/11/2012

#ifndef _HE_MaterialGeneratorMathNodes_H_
#define _HE_MaterialGeneratorMathNodes_H_
#pragma once

#include "MaterialGeneratorNodeClassHelper.h"

namespace he {
namespace tools {
    
// One param
MaterialGeneratorNodeClass(Abs)
MaterialGeneratorNodeClass(Cos)
MaterialGeneratorNodeClass(Sin)
MaterialGeneratorNodeClass(Ceil)
MaterialGeneratorNodeClass(Floor)
MaterialGeneratorNodeClass(Frac)
MaterialGeneratorNodeClass(OneMin)
MaterialGeneratorNodeClass(Normalize)
MaterialGeneratorNodeClass(Sign)

// Two params
MaterialGeneratorNodeClass(Add)
MaterialGeneratorNodeClass(Divide)
MaterialGeneratorNodeClass(Min)
MaterialGeneratorNodeClass(Max)
MaterialGeneratorNodeClass(Multiply)
MaterialGeneratorNodeClass(Subtract)
MaterialGeneratorNodeClass(Cross)
MaterialGeneratorNodeClass(Distance)
MaterialGeneratorNodeClass(DistanceSqr)
MaterialGeneratorNodeClass(Dot)
MaterialGeneratorNodeClass(Power)
MaterialGeneratorNodeClass(Reflect)

// Three param
MaterialGeneratorNodeClass(Clamp)
MaterialGeneratorNodeClass(Lerp)

} } //end namespace

#endif
