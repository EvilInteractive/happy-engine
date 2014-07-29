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
//Created: 13/03/2013
#include "HappyPCH.h" 

#include "HeFixedStrings.h"

namespace he {

#define _DEF_FS(name, string) he::FixedString HEFS::name;
#define DEF_FS(string) _DEF_FS(str##string, #string);
#include "HeFixedStringTable.h"
#undef _DEF_FS
#undef DEF_FS


void HEFS::sdmInit()
{
#define _DEF_FS(name, string) name = he::FixedString::fromString(string, sizeof(string));
#define DEF_FS(string) _DEF_FS(str##string, #string);
#include "HeFixedStringTable.h"
#undef _DEF_FS
#undef DEF_FS
}

void HEFS::sdmDestroy()
{
#define _DEF_FS(name, string) name = he::FixedString();
#define DEF_FS(string) _DEF_FS(str##string, #string);
#include "HeFixedStringTable.h"
#undef _DEF_FS
#undef DEF_FS
}

} //end namespace
