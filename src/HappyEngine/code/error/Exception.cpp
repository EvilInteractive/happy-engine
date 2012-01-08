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
#include "HappyPCH.h" 

#include "Exception.h"

namespace he {
namespace err {

Exception::Exception() : m_Msg(L"")
{
}
Exception::Exception(const std::wstring& msg) : m_Msg(msg)
{
}

Exception::~Exception()
{
}

std::wstring Exception::getType() const
{
    return L"Exception";
}
std::wstring Exception::getMsg() const
{
    return m_Msg;
}

} } //end namespace