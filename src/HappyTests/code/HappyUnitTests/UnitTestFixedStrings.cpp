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
//Created: 13/03/2013
#include "HappyUnitTestsPCH.h" 

#include "UnitTestFixedStrings.h"

namespace hut {

#define _DEF_FS(name, string) he::FixedString HTFS::name;
#define DEF_FS(string) _DEF_FS(str##string, #string);
#include "UnitTestFixedStringsTable.h"
#undef _DEF_FS
#undef DEF_FS


void HTFS::sdmInit()
{
#define _DEF_FS(name, string) name = he::FixedString::fromString(string, sizeof(string));
#define DEF_FS(string) _DEF_FS(str##string, #string);
#include "UnitTestFixedStringsTable.h"
#undef _DEF_FS
#undef DEF_FS
}

void HTFS::sdmDestroy()
{
#define _DEF_FS(name, string) name = he::FixedString();
#define DEF_FS(string) _DEF_FS(str##string, #string);
#include "UnitTestFixedStringsTable.h"
#undef _DEF_FS
#undef DEF_FS
}

} //end namespace
