//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
#include "HappyPCH.h"
#include "RenderSettings.h"

#include "StructuredVisitor.h"

namespace he {
namespace gfx {

void PostSettings::HdrSettings::visit( io::StructuredVisitor* const visitor )
{
    if (visitor->enterNode(HEFS::strHDRSettings))
    {
        visitor->visit(HEFS::strExposureSpeed, exposureSpeed);

        visitor->exitNode(HEFS::strHDRSettings);
    }
}

void PostSettings::AOSettings::visit( io::StructuredVisitor* const visitor )
{
    if (visitor->enterNode(HEFS::strAOSettings))
    {
        visitor->visit(HEFS::strRadius, radius);
        visitor->visit(HEFS::strMaxDistance, maxDistance);

        visitor->exitNode(HEFS::strAOSettings);
    }
}

void PostSettings::ShaderSettings::visit( io::StructuredVisitor* const visitor )
{
    if (visitor->enterNode(HEFS::strShaderSettings))
    {
        visitor->visit(HEFS::strEnableHDR, enableHDR);
        visitor->visit(HEFS::strEnableAO, enableAO);
        visitor->visit(HEFS::strEnableBloom, enableBloom);
        visitor->visit(HEFS::strEnableNormalEdgeDetect, enableNormalEdgeDetect);
        visitor->visit(HEFS::strEnableDepthEdgeDetect, enableDepthEdgeDetect);
        visitor->visit(HEFS::strEnableVignette, enableVignette);
        visitor->visit(HEFS::strEnableFog, enableFog);

        visitor->exitNode(HEFS::strShaderSettings);
    }
}

void PostSettings::visit( io::StructuredVisitor* const visitor )
{
    if (visitor->enterNode(HEFS::strPostSettings))
    {
        hdrSettings.visit(visitor);
        shaderSettings.visit(visitor);
        aoSettings.visit(visitor);

        visitor->exitNode(HEFS::strPostSettings);
    }
}

void ShadowSettings::visit( io::StructuredVisitor* const visitor )
{
    if (visitor->enterNode(HEFS::strShadowSettings))
    {
        visitor->visit(HEFS::strShadowQuality, shadowMult);

        visitor->exitNode(HEFS::strShadowSettings);
    }
}

void LightingSettings::visit( io::StructuredVisitor* const visitor )
{
    if (visitor->enterNode(HEFS::strLightingSettings))
    {
        visitor->visit(HEFS::strEnableLighting, enableLighting);
        visitor->visit(HEFS::strEnableSpecular, enableSpecular);
        visitor->visit(HEFS::strEnableNormalMaps, enableNormalMap);
        visitor->visit(HEFS::strEnableShadows, enableShadows);

        visitor->exitNode(HEFS::strLightingSettings);
    }
}


void RenderSettings::visit( io::StructuredVisitor* const visitor )
{
    if (visitor->enterNode(HEFS::strRenderSettings))
    {
        visitor->visit(HEFS::strEnableDeferredRendering, enableDeferred);
        visitor->visit(HEFS::strEnablePostEffects, enablePost);

        bool enableOculusRift(stereoSetting == StereoSetting_OculusRift);
        visitor->visit(HEFS::strEnableOculusRift, enableOculusRift);
        stereoSetting = enableOculusRift? StereoSetting_OculusRift : StereoSetting_None;

        lightingSettings.visit(visitor);
        postSettings.visit(visitor);
        shadowSettings.visit(visitor);

        visitor->exitNode(HEFS::strRenderSettings);
    }
}

} } //end namespace
