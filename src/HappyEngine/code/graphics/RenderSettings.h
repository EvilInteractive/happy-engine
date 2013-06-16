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
namespace io {
    class StructuredVisitor;
}
namespace gfx {
 
struct PostSettings
{
    struct HdrSettings
    {
        float exposureSpeed;

        HdrSettings(): exposureSpeed(1.0f) {}

        void visit(io::StructuredVisitor* const visitor);
    };
    struct AOSettings
    {
        float radius;
        float maxDistance;

        AOSettings(): radius(1.0f), maxDistance(2.0f) {}

        void visit(io::StructuredVisitor* const visitor);
    };
    struct ShaderSettings
    {
        ShaderSettings(): enableHDR(true), enableAO(false), enableBloom(true), enableNormalEdgeDetect(false),
            enableDepthEdgeDetect(false), enableVignette(true), enableFog(true) {}

        bool enableHDR;
        bool enableAO;
        bool enableBloom;
        bool enableNormalEdgeDetect;
        bool enableDepthEdgeDetect;
        bool enableVignette;
        bool enableFog;

        bool operator==(const ShaderSettings& other) const
        {
            return enableHDR == other.enableHDR && 
                   enableAO == other.enableAO && 
                   enableBloom == other.enableBloom && 
                   enableNormalEdgeDetect == other.enableNormalEdgeDetect && 
                   enableDepthEdgeDetect == other.enableDepthEdgeDetect && 
                   enableVignette == other.enableVignette && 
                   enableFog == other.enableFog;
        }
        bool operator!=(const ShaderSettings& other) const
        {
            return !(*this == other);
        }

        void visit(io::StructuredVisitor* const visitor);
    };

    HdrSettings hdrSettings;
    ShaderSettings shaderSettings;
    AOSettings aoSettings;

    void visit(io::StructuredVisitor* const visitor);
};  
struct ShadowSettings
{
    ShadowSettings(): shadowMult(1) {}
    uint8 shadowMult;

    void visit(io::StructuredVisitor* const visitor);
};
struct LightingSettings
{
    LightingSettings(): enableLighting(true), enableSpecular(true), enableNormalMap(true), enableShadows(true) {}

    bool enableLighting;
    bool enableSpecular;
    bool enableNormalMap;
    bool enableShadows;

    bool operator==(const LightingSettings& other) const
    {
        return enableLighting == other.enableLighting && 
               enableSpecular == other.enableSpecular && 
               enableNormalMap == other.enableNormalMap && 
               enableShadows == other.enableShadows;
    }
    bool operator!=(const LightingSettings& other) const
    {
        return !(*this == other);
    }

    void visit(io::StructuredVisitor* const visitor);
};
enum StereoSetting
{
    StereoSetting_None,
    StereoSetting_OculusRift
}; 

struct RenderSettings
{
public:
    RenderSettings(): enableDeferred(true), enablePost(true), stereoSetting(StereoSetting_None)
    {}

    bool enableDeferred;
    bool enablePost; 
    StereoSetting stereoSetting;
        
    LightingSettings lightingSettings;
    PostSettings postSettings;
    ShadowSettings shadowSettings;

    void visit(io::StructuredVisitor* const visitor);
};

} } //end namespace

#endif
