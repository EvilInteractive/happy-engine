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

#include "Assert.h"
#include <iostream>

namespace happyengine {
namespace error {
namespace details {

void happyAssert(bool isOk, const char* message)
{
    if (!isOk)
    {
        std::cout << "**ASSERTION FAILURE!**\n";
        std::cout << message << "\n";
        __debugbreak();
    }
}
void happyAssert(const char* message)
{
    std::cout << "**ASSERTION FAILURE!**\n";
    std::cout << message << "\n";
     __debugbreak();
}
void happyAssert()
{
     __debugbreak();
}

} } } //end namespace