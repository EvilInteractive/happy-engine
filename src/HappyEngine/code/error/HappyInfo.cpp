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
//Created: 25/03/2012
#include "HappyPCH.h"

#include "HappyInfo.h"
#include "Logger.h"

#pragma warning(disable:4996) // use _s version instead

namespace he {

void heInfo( const char* msg, ...) 
{
    va_list arg_list;
    va_start(arg_list, msg);
    LOG(he::LogType_Info, msg, arg_list);
    va_end(arg_list);
}
void heError( const char* msg, ...) 
{
    va_list arg_list;
    va_start(arg_list, msg);
    LOG(he::LogType_Error, msg, arg_list);
    va_end(arg_list);
}
void heWarning( const char* msg, ...) 
{
    va_list arg_list;
    va_start(arg_list, msg);
    LOG(he::LogType_Warning, msg, arg_list);
    va_end(arg_list);
}

#pragma warning(default:4996) // use _s version instead

}
