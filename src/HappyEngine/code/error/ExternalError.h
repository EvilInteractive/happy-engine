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

#ifndef _HE_EXTRERNAL_ERROR_H_
#define _HE_EXTRERNAL_ERROR_H_
#pragma once

#include "GL/glew.h"
#include "SDL.h"
#include <iostream>
#include "PxPhysicsAPI.h"
#include "HappyEngine.h"
#include "Console.h"

namespace he {
namespace err {

inline void glHandleError(GLenum err)
{
    if (err != GL_NO_ERROR)
    {
        std::cout << "***GL err*** " << glewGetErrorString(err) << "\n";
    }
}
inline void glCheckForErrors(bool postErrors = true)
{
    GLenum err = glGetError();
    while (err != GL_NO_ERROR)
    {
        if (postErrors)
            glHandleError(err);
        err = glGetError();
    }
}
inline void sdlHandleError(int err)
{
    if (err != 0)
    {
        std::cout << "***SDL err*** " << SDL_GetError() << "\n";
    }
}
inline void checkFboStatus(const std::string& name)
{
    GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (err != GL_FRAMEBUFFER_COMPLETE)
    {
        CONSOLE->addMessage("Woops something went wrong with the " + name + " framebuffer", CMSG_TYPE_ERROR);
        switch (err)
        {
            case GL_FRAMEBUFFER_UNDEFINED: CONSOLE->addMessage("GL_FRAMEBUFFER_UNDEFINED", CMSG_TYPE_ERROR); break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: CONSOLE->addMessage("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT", CMSG_TYPE_ERROR); break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: CONSOLE->addMessage("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", CMSG_TYPE_ERROR); break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: CONSOLE->addMessage("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER", CMSG_TYPE_ERROR); break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: CONSOLE->addMessage("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER", CMSG_TYPE_ERROR); break;
            case GL_FRAMEBUFFER_UNSUPPORTED: CONSOLE->addMessage("GL_FRAMEBUFFER_UNSUPPORTED", CMSG_TYPE_ERROR); break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: CONSOLE->addMessage("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE", CMSG_TYPE_ERROR); break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: CONSOLE->addMessage("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS", CMSG_TYPE_ERROR); break;
            default: CONSOLE->addMessage("GL_UNKOWN_ERROR", CMSG_TYPE_ERROR); break;
        }
    }
}

class HappyPhysicsErrorCallback : public physx::PxErrorCallback
{
public:
    HappyPhysicsErrorCallback();
    ~HappyPhysicsErrorCallback();

    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
};

} } //end namespace
#endif