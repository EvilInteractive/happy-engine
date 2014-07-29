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

#ifndef _HT_UnitTestFixedStrings_H_
#define _HT_UnitTestFixedStrings_H_
#pragma once

namespace hut {

class HTFS 
{
public:
#define _DEF_FS(name, string) static he::FixedString name;
#define DEF_FS(string) _DEF_FS(str##string, #string);
#include "UnitTestFixedStringsTable.h"
#undef _DEF_FS
#undef DEF_FS

    static void sdmInit();
    static void sdmDestroy();
};

} //end namespace

#endif
