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
//Created: 25/08/2012
#include "HappyPCH.h"
#include "Light.h"

namespace he {
namespace gfx {

IMPLEMENT_OBJECT(Light);

HAPPY_ENTRY const char* shadowResolutionToGuiString( const ShadowResolution res )
{
    const char* result("**Invalid!**");
    switch (res)
    {
    case ShadowResolution_None: result = "None"; break;
    case ShadowResolution_32: result = "32 * 32px"; break;
    case ShadowResolution_64: result = "64 * 64px"; break;  
    case ShadowResolution_128: result = "128 * 128px"; break;  
    case ShadowResolution_256: result = "256 * 256px"; break; 
    case ShadowResolution_512: result = "512 * 512px"; break;  
    case ShadowResolution_1024: result = "1024 * 1024px"; break;
    default: LOG(LogType_ProgrammerAssert, "Unknown ShadowResolution! %d", res); break;
    }
    return result;
}

HAPPY_ENTRY he::gfx::ShadowResolution shadowResolutionFromGuiString( const char* str )
{
    ShadowResolution result(ShadowResolution_None);
    if (strcmp(str, "None") == 0)
    {
        result = ShadowResolution_None;
    }
    else if (strcmp(str, "32 * 32px") == 0)
    {
        result = ShadowResolution_32;
    }
    else if (strcmp(str, "64 * 64px") == 0)
    {
        result = ShadowResolution_64;
    }
    else if (strcmp(str, "128 * 128px") == 0)
    {
        result = ShadowResolution_128;
    }
    else if (strcmp(str, "256 * 256px") == 0)
    {
        result = ShadowResolution_256;
    }
    else if (strcmp(str, "512 * 512px") == 0)
    {
        result = ShadowResolution_512;
    }
    else if (strcmp(str, "1024 * 1024px") == 0)
    {
        result = ShadowResolution_1024;
    }
    else
    {
        LOG(LogType_ProgrammerAssert, "Unknown ShadowResolution string! %s", str);
    }
    return result;
}

} } //end namespace

