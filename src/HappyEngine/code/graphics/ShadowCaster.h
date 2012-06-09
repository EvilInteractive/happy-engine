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
//Created: 22/10/2011

#ifndef _HE_SHADOW_CASTER_H_
#define _HE_SHADOW_CASTER_H_
#pragma once

#include "RenderSettings.h"
#include "DrawListContainer.h"
#include "DirectionalLight.h"

namespace he {
namespace gfx {

class Shader;
class Texture2D;
class ModelMesh;
class Material;

class ShadowCaster
{
public:
    ShadowCaster();
    virtual ~ShadowCaster();

    void init(const RenderSettings& settings);
    void setSettings(const RenderSettings& settings);
    
    void render(const DrawListContainer& elements, DirectionalLight* directionalLight);

private:

    uint m_FboId;

    const static int COUNT = DirectionalLight::CASCADES + 1;
    Texture2D* m_pShadowTexture[COUNT]; //first = blur temp
    uint m_DepthRenderbuff;
    
    Material* m_MatSingle;
    Material* m_MatSkinned;
    Material* m_MatInstanced;

    const static int s_ShadowBlurPasses = 2;
    Shader* m_pShadowBlurShaderPass[s_ShadowBlurPasses];
    uint m_BlurShaderTexPosPass[s_ShadowBlurPasses];

    ModelMesh* m_pQuad;

    ushort m_ShadowSize;

    bool m_ShowShadowDebug;

    //Disable default copy constructor and default assignment operator
    ShadowCaster(const ShadowCaster&);
    ShadowCaster& operator=(const ShadowCaster&);
};

} } //end namespace

#endif
