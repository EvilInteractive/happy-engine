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
//Created: 25/03/2012
#include "HappyPCH.h"

#include "HappyInfo.h"

#pragma warning(disable:4996) // use _s version instead

namespace he {

namespace details {
void HE_INFO( const char* msg, va_list args )
{
    char buff[500];
    he_memset(buff, 0, 500);
    vsnprintf(buff, 500, msg, args);

    std::cout << "Info: " << buff << "\n";
    if (HAPPYENGINE != nullptr) 
        CONSOLE->addMessage(buff, CMSG_TYPE_ENGINE);
}
void HE_ERROR( const char* msg, va_list args )
{
    char buff[500];
    he_memset(buff, 0, 500);
    vsnprintf(buff, 500, msg, args);

    std::cout << "Error: " << buff << "\n";
    if (HAPPYENGINE != nullptr) 
        CONSOLE->addMessage(buff, CMSG_TYPE_ERROR);
}
void HE_WARNING( const char* msg, va_list args )
{
    char buff[500];
    he_memset(buff, 0, 500);
    vsnprintf(buff, 500, msg, args);

    std::cout << "Warning: " << buff << "\n";
    if (HAPPYENGINE != nullptr) 
        CONSOLE->addMessage(buff, CMSG_TYPE_WARNING);
}
} // end namespace details

void HE_INFO( const char* msg, ...) 
{
    va_list arg_list;
    va_start(arg_list, msg);
    details::HE_INFO(msg, arg_list);
    va_end(arg_list);
}
void HE_ERROR( const char* msg, ...) 
{
    va_list arg_list;
    va_start(arg_list, msg);
    details::HE_ERROR(msg, arg_list);
    va_end(arg_list);
}
void HE_WARNING( const char* msg, ...) 
{
    va_list arg_list;
    va_start(arg_list, msg);
    details::HE_WARNING(msg, arg_list);
    va_end(arg_list);
}

#pragma warning(default:4996) // use _s version instead

}
