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
//Created: 19/08/2011
#include "HappyPCH.h" 

#include "ExternalError.h"

namespace he {
namespace err {

HappyPhysicsErrorCallback::HappyPhysicsErrorCallback() {}
HappyPhysicsErrorCallback::~HappyPhysicsErrorCallback(){}

void HappyPhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    std::stringstream stream;
    stream << "PhysicsEngine '";

    switch (code)
    {
        case physx::PxErrorCode::eINVALID_PARAMETER:
            stream << "invalid parameter";
            break;
        case physx::PxErrorCode::eINVALID_OPERATION:
            stream << "invalid operation";
            break;
        case physx::PxErrorCode::eOUT_OF_MEMORY:
            stream << "out of memory";
            break;
        case physx::PxErrorCode::eDEBUG_INFO:
            stream << "info";
            break;
        case physx::PxErrorCode::eDEBUG_WARNING:
            stream << "warning";
            return;
            break;
        default:
            stream << "unknown err";
            break;
    }

    stream << "': " << message << " [" << file << "(" << line << ")] \n";

    HE_WARNING(stream.str().c_str());
}

void checkFboStatus( const he::String& name )
{
    GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (err != GL_FRAMEBUFFER_COMPLETE)
    {
        HE_ERROR("Woops something went wrong with the %s framebuffer", name.c_str());
        switch (err)
        {
        case GL_FRAMEBUFFER_UNDEFINED:                      HE_ERROR("GL_FRAMEBUFFER_UNDEFINED");                      break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:          HE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");          break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:  HE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");  break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:         HE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");         break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:         HE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");         break;
        case GL_FRAMEBUFFER_UNSUPPORTED:                    HE_ERROR("GL_FRAMEBUFFER_UNSUPPORTED");                    break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:         HE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");         break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:       HE_ERROR("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");       break;
        default:                                            HE_ERROR("GL_UNKOWN_ERROR");                               break;
        }
    }
}

} } //end namespace