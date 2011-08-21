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

#include "ExternalError.h"
#include <sstream>

namespace happyengine {
namespace error {

HappyPhysicsErrorCallback::HappyPhysicsErrorCallback() {}
HappyPhysicsErrorCallback::~HappyPhysicsErrorCallback(){}

void HappyPhysicsErrorCallback::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    std::stringstream error;
    error << "physicsEngine '";

    switch (code)
    {
        case PxErrorCode::eINVALID_PARAMETER:
            error << "invalid parameter";
            break;
        case PxErrorCode::eINVALID_OPERATION:
            error << "invalid operation";
            break;
        case PxErrorCode::eOUT_OF_MEMORY:
            error << "out of memory";
            break;
        case PxErrorCode::eDEBUG_INFO:
            error << "info";
            break;
        case PxErrorCode::eDEBUG_WARNING:
            error << "warning";
            break;
        default:
            error << "unknown error";
            break;
    }

    error << "': " << message << " [" << file << "(" << line << ")] \n";
}

} } //end namespace