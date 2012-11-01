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

#ifndef _HE_SHADOW_CASTER_H_
#define _HE_SHADOW_CASTER_H_
#pragma once

#include "ShadowCasterSpotlight.h"
#include "Light.h"

namespace he {
namespace gfx {
class View;
class Scene;

class ShadowCaster
{
public:
    ShadowCaster();
    virtual ~ShadowCaster();

    void init(View* view);

    void render(Scene* scene);

private:
    ShadowCasterSpotLight m_SpotLightShadowRenderers[ShadowResolution_MAX - 1];

    //Disable default copy constructor and default assignment operator
    ShadowCaster(const ShadowCaster&);
    ShadowCaster& operator=(const ShadowCaster&);
};

} } //end namespace

#endif
