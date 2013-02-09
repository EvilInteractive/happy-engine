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
//Created: 17/08/2011

#ifndef _HE_ILIGHT_H_
#define _HE_ILIGHT_H_
#pragma once

namespace he {
namespace gfx {
    
ENUM(ShadowResolution, uint8) // note this is Normal quality, can be scaled up or down for other qualities
{
    ShadowResolution_None,
    ShadowResolution_32,
    ShadowResolution_64,  
    ShadowResolution_128,  
    ShadowResolution_256, 
    ShadowResolution_512,  
    ShadowResolution_1024,

    ShadowResolution_MAX
};
enum LightType
{
    LightType_Point,
    LightType_Spot,
    LightType_Unkown
};

class Light
{
DECLARE_OBJECT(Light)
public:
    Light(): m_VisibleLastFrame(false) {}
    virtual ~Light() {}
    virtual LightType getType() const { return LightType_Unkown; }

    inline bool getVisibleLastFrame() const { return m_VisibleLastFrame; }
    inline void setVisibleLastFrame(bool visible) { m_VisibleLastFrame = visible; }

private:
    bool m_VisibleLastFrame;
};


} } //end namespace

#endif
