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

#define HE_CONDITIONAL_INFO(check, msg, ...) if (check) { he::heInfo(msg, ##__VA_ARGS__); }
#define HE_CONDITIONAL_ERROR(check, msg, ...) if (check) { he::heError(msg, ##__VA_ARGS__); }
#define HE_CONDITIONAL_WARNING(check, msg, ...) if (check) { he::heWarning(msg, ##__VA_ARGS__); }

} //end namespace

#endif
