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
//Author:  Sebastiaan Sprengers
//Created: 27/11/2012

#ifndef _HE_NINEPATCHEFFECT_H_
#define _HE_NINEPATCHEFFECT_H_
#pragma once

namespace he {
namespace gfx {
class Shader;
class Texture2D;

class NinePatchEffect
{
public:
    NinePatchEffect();
    virtual ~NinePatchEffect();

    void load();
    void begin() const;
    void end() const;

    void setWorldMatrix(const he::mat44& mat) const;
    void setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const;
    void setCenter(const RectF& center) const;
    void setSize(const vec2& size) const;
    void setOriginalSize(const vec2& origSize) const;
    void setDepth(float depth) const;

private:
    gfx::Shader* m_Shader;

    uint32 m_ShaderWVPPos;
    uint32 m_ShaderDiffTexPos;
    uint32 m_ShaderCenterPos;
    uint32 m_ShaderSizePos;
    uint32 m_ShaderOrigSizePos;
    uint32 m_ShaderDepthPos;

    //Disable default copy constructor and default assignment operator
    NinePatchEffect(const NinePatchEffect&);
    NinePatchEffect& operator=(const NinePatchEffect&);
};

} } //end namespace

#endif
