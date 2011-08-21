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
//Created: 05/08/2011

#ifndef _EXTRERNAL_ERROR_H_
#define _EXTRERNAL_ERROR_H_
#pragma once

#include "GL/glew.h"
#include "SDL.h"
#include <iostream>
#include "PxPhysicsAPI.h"

namespace happyengine {
namespace error {

inline void glHandleError(GLenum err)
{
    if (err != GL_NO_ERROR)
    {
        std::cout << "***GL error*** file: " << __FILE__ << " line: " << __LINE__ << "\n";
        std::cout << glewGetErrorString(err);
    }
}
inline void sdlHandleError(int err)
{
    if (err != 0)
    {
        std::cout << "***SDL error***" << __FILE__ << " line: " << __LINE__ << "\n";
        std::cout << SDL_GetError();
    }
}

class HappyPhysicsErrorCallback : public PxErrorCallback
{
public:
    HappyPhysicsErrorCallback();
    ~HappyPhysicsErrorCallback();

    virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
};

} } //end namespace
#endif