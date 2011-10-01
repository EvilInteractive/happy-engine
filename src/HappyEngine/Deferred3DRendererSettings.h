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
//Created: 28/09/2011

#ifndef _HE_DEFERRED_3D_RENDERER_SETTINGS_H_
#define _HE_DEFERRED_3D_RENDERER_SETTINGS_H_
#pragma once

#include "HappyTypes.h"

namespace happyengine {
namespace graphics {

enum BloomQuality
{
    BloomQuality_None,   //No Bloom
    BloomQuality_Low,    //Simple bloom (1 blur)
    BloomQuality_High    //Advanced bloom (multiple blurs)
};
enum ShadowQuality
{
    ShadowQuality_None,
    ShadowQuality_LowNoDynamic,    //(static only low quality)
    ShadowQuality_Low,             //static+dynamic low quality
    ShadowQuality_MediumNoDynamic, //(static only medium quality)
    ShadowQuality_Medium,          //static+dynamic medium quality
    ShadowQuality_HighNoDynamic,   //static high quality
    ShadowQuality_High             //static+dynamic high quality
};
enum ModelQuality
{
    ModelQuality_Low,        //min lod2
    ModelQuality_Medium,     //min lod1
    ModelQuality_High        //min lod0
};

enum ViewDistanceQuality
{
    ViewDistanceQuality_Low,        //fog from .. to ..
    ViewDistanceQuality_Medium,     //fog from .. to ..
    ViewDistanceQuality_High        //fog from .. to ..
};

class Deferred3DRendererSettings
{
public:
	Deferred3DRendererSettings();
    virtual ~Deferred3DRendererSettings();

    void setViewDistanceQuality(ViewDistanceQuality quality);
    float getFogStart() const;
    float getFogEnd() const;

    void setShadowQuality(ShadowQuality quality) const;
    bool getShadowsEnabled() const;
    bool getDynamicShadowsEnabled() const;
    byte getShadowMapMultiplier() const;

private:

    //ViewDistanceQuality
    float m_FogStart, m_FogEnd;

    //ShadowQuality
    byte m_ShadowMapSizeMuliplier;
    bool m_EnableShadows;
    bool m_EnableDynamicShadows;

    //Disable default copy constructor and default assignment operator
    Deferred3DRendererSettings(const Deferred3DRendererSettings&);
    Deferred3DRendererSettings& operator=(const Deferred3DRendererSettings&);
};

} } //end namespace

#endif
