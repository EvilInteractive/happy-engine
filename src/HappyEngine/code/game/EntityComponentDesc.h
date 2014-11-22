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
//Created: 10/03/2013

#ifndef _HE_EntityComponentDesc_H_
#define _HE_EntityComponentDesc_H_
#pragma once

#include <FixedStringMap.h>
#include "PropertyDesc.h"

namespace he {
namespace ge {

struct HAPPY_ENTRY EntityComponentDesc
{
public:
    EntityComponentDesc();
    ~EntityComponentDesc();

    he::FixedString m_ID;
    he::String m_DisplayName;
    he::FixedStringMap<PropertyDesc> m_Properties;

private:
    EntityComponentDesc(const EntityComponentDesc&);
    EntityComponentDesc& operator=(const EntityComponentDesc&);
};

} } //end namespace

#endif
