//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#ifndef _ASSERT_H_
#define _ASSERT_H_
#pragma once

#include <string>

namespace happyengine {
namespace error {

#ifdef _DEBUG
#define ASSERT happyengine::error::details::happyAssert
#else
#define ASSERT(...) {}
#endif

namespace details {

#ifdef _DEBUG
void happyAssert(bool isOk, const std::string& message);
void happyAssert(const std::string& message);
void happyAssert(bool isOk);
void happyAssert(int isOk); //for boost
void happyAssert(void* isOk); //for boost
void happyAssert();
#endif

} } } //end namespace


#endif
