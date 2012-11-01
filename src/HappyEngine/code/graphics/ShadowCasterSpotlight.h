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
//Created: 13/10/2012

#ifndef _HE_SHADOW_CASTER_SPOTLIGHT_H_
#define _HE_SHADOW_CASTER_SPOTLIGHT_H_
#pragma once

#include "Light.h"

namespace he {
namespace gfx {

class SpotLight;
class Material;
class RenderTarget;
class View;
class Scene;
class ModelMesh;
class IDrawable;

class ShadowCasterSpotLight
{
public:
    ShadowCasterSpotLight();
    virtual ~ShadowCasterSpotLight();
        
    void init(View* view, ShadowResolution resolution);
    void render(Scene* scene, SpotLight* spotlight);

private:
    RenderTarget* m_RenderTarget;
        
    Material* m_MatSingle;
    Material* m_MatSkinned;
    Material* m_MatInstanced;
    
    ShadowResolution m_Resolution;

    std::vector<IDrawable*> m_SingleDrawables;
    std::vector<IDrawable*> m_SkinnedDrawables;
    std::vector<IDrawable*> m_InstancedDrawables;

    //Disable default copy constructor and default assignment operator
    ShadowCasterSpotLight(const ShadowCasterSpotLight&);
    ShadowCasterSpotLight& operator=(const ShadowCasterSpotLight&);
};

} } //end namespace

#endif