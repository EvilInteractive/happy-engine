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
//Author:  Bastian Damman
//Created: 17/11/2012

#ifndef _HE_ShaderGeneratorVariableFactory_H_
#define _HE_ShaderGeneratorVariableFactory_H_
#pragma once

#include "ShaderGeneratorVariable.h"
#include "Singleton.h"

namespace he {
namespace ct {

class ShaderGeneratorVariableFactory : public ObjectFactory<ShaderGeneratorVariable>, public Singleton<ShaderGeneratorVariableFactory>
{
public:
    ShaderGeneratorVariableFactory();
    virtual ~ShaderGeneratorVariableFactory();

private:

    //Disable default copy constructor and default assignment operator
    ShaderGeneratorVariableFactory(const ShaderGeneratorVariableFactory&);
    ShaderGeneratorVariableFactory& operator=(const ShaderGeneratorVariableFactory&);
};

} } //end namespace

#endif
