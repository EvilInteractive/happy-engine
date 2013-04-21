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
//Created: 2013/04/20

#ifndef _HE_SandboxEntityComponentFactory_H_
#define _HE_SandboxEntityComponentFactory_H_
#pragma once

#include "IEntityComponentFactory.h"

namespace hs {

class SandboxEntityComponentFactory : public he::ge::IEntityComponentFactory
{
public:
    virtual ~SandboxEntityComponentFactory() {}

    he::ge::EntityComponent* createEntityComponent(const he::FixedString& type) const;
    void fillComponentDescList(he::PrimitiveList<he::ge::EntityComponentDesc*>& list) const;
};

} //end namespace

#endif
