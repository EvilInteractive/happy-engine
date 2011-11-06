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
//
//Author:  Bastian Damman
//Created: 19/08/2011
#include "StdAfx.h" 

#include "ExternalError.h"
#include <sstream>

namespace he {
namespace err {

HappyPhysicsErrorCallback::HappyPhysicsErrorCallback() {}
HappyPhysicsErrorCallback::~HappyPhysicsErrorCallback(){}

void HappyPhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    std::cout << "physicsEngine '";

    switch (code)
    {
        case physx::PxErrorCode::eINVALID_PARAMETER:
            std::cout << "invalid parameter";
            break;
        case physx::PxErrorCode::eINVALID_OPERATION:
            std::cout << "invalid operation";
            break;
        case physx::PxErrorCode::eOUT_OF_MEMORY:
            std::cout << "out of memory";
            break;
        case physx::PxErrorCode::eDEBUG_INFO:
            std::cout << "info";
            break;
        case physx::PxErrorCode::eDEBUG_WARNING:
            std::cout << "warning";
            break;
        default:
            std::cout << "unknown err";
            break;
    }

    std::cout << "': " << message << " [" << file << "(" << line << ")] \n";
}

} } //end namespace