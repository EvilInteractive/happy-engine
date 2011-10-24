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
//Created: 22/10/2011

#ifndef _HE_SHADOW_CASTER_H_
#define _HE_SHADOW_CASTER_H_
#pragma once

#include "Camera.h"
#include "Light.h"
#include "Texture2D.h"
#include "Shader.h"
#include "DrawManager.h"

namespace he {
namespace gfx {

class ShadowCaster
{
public:
	ShadowCaster();
    virtual ~ShadowCaster();

    void init();
    
    void render(const std::vector<DrawManager::DrawElement>& elements, const Camera* pCamera, const DirectionalLight::pointer& pDirectionalLight);

private:

    uint m_FboId;

    const static int COUNT = DirectionalLight::CASCADES;
    Texture2D::pointer m_pShadowTexture[COUNT];

    Shader::pointer m_pShadowShader;
    uint m_shaderWVPpos;

    ushort m_ShadowSize;

    //Disable default copy constructor and default assignment operator
    ShadowCaster(const ShadowCaster&);
    ShadowCaster& operator=(const ShadowCaster&);
};

} } //end namespace

#endif
