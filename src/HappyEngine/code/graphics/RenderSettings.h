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
//Created: 17/12/2011

#ifndef _HE_RENDER_SETTINGS_H_
#define _HE_RENDER_SETTINGS_H_
#pragma once

#include "HappyTypes.h"

namespace he {
namespace gfx {

struct AOSettings
{
    float radius;
    float maxDistance;
};  

struct RenderSettings
{
public:
    bool isFullscreen;
    bool enableDeferred;

    bool enableHDR;
    bool enableAO;
    bool enableBloom;
    bool enableShadows;
    
    bool enableLighting;
    bool enableSpecular;
    bool enableNormalMap;

    bool enableNormalEdgeDetect;
    bool enableDepthEdgeDetect;
    bool enableVignette;
    bool enableFog;

    AOSettings aoSettings;

    byte shadowMult;

    float exposureSpeed;
};

} } //end namespace

#endif
