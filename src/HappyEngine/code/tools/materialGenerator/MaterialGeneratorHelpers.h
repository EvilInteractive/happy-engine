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
//Created: 13/11/2012

#ifndef _HE_MaterialGeneratorNodeTypes_H_
#define _HE_MaterialGeneratorNodeTypes_H_
#pragma once

namespace he {
namespace tools {

template<typename T>
inline T getValueFromBuffer(const void* buffer, const size_t offsetInBytes = 0)
{
    return *static_cast<const T*>(static_cast<const char*>(buffer) + offsetInBytes);
}

} } //end namespace

#endif
