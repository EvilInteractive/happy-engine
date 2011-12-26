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
//Created: 17/12/2011

#ifndef _HE_IRENDERER_H_
#define _HE_IRENDERER_H_
#pragma once

#include "DrawListContainer.h"
#include "RenderSettings.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

class LightManager;

class IRenderer
{
public:
    virtual ~IRenderer() {}

    virtual void init(const RenderSettings& settings, 
        const Texture2D::pointer& pOutTarget, const Texture2D::pointer& pOutNormalTarget, const Texture2D::pointer& pOutDepthTarget) = 0;
    
    virtual void setRenderSettings(const RenderSettings& settings) = 0;
    virtual void onScreenResized() = 0;

    virtual void draw(const DrawListContainer& drawList, uint renderFlags) = 0;

    virtual void clear(bool color, bool normal, bool depth) = 0;

    virtual bool getSupportsTranslucency() const = 0;
};


} } //end namespace

#endif
