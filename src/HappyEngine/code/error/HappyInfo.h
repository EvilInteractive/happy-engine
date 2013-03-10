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
//Created: 23/03/2012

#ifndef _HE_HAPPYINFO_H_
#define _HE_HAPPYINFO_H_
#pragma once

namespace he {

HAPPY_ENTRY void heInfo(const char* msg, ...);
HAPPY_ENTRY void heError(const char* msg, ...);
HAPPY_ENTRY void heWarning(const char* msg, ...);

#define HE_INFO he::heInfo
#define HE_ERROR he::heError
#define HE_WARNING he::heWarning

} //end namespace

#endif
