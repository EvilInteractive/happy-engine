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
//Created: 07/10/2011

#ifndef _HE_PointLight_H_
#define _HE_PointLight_H_
#pragma once

#include "DefaultSingleDrawable.h"
#include "ILight.h"

namespace he {
namespace gfx {

class ModelMesh;

class PointLight : public DefaultSingleDrawable, public ILight
{
private:
    vec3 m_Position;
    float m_Multiplier;
    vec3 m_Color;
    float m_BeginAttenuation;
    float m_EndAttenuation;

    mat44 m_mtxWorld;
    void calculateWorld();

    ModelMesh* m_LightVolume;
    ModelMesh* m_Model;
    Material m_Material;

public:
    PointLight();
    virtual ~PointLight();
    PointLight(const PointLight& other);
    PointLight& operator=(const PointLight& other);

    void setPosition(const vec3& position);
    void setMultiplier(float multiplier);
    void setAttenuation(float begin, float end);
    void setColor(const vec3& color);
    void setColor(const Color& color);

    const vec3& getPosition() const;
    float getMultiplier() const;
    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    const vec3& getColor() const;
    
    mat44 getWorldMatrix() const; 
    const ModelMesh* getLightVolume() const;
    
    virtual bool getCastsShadow() const { return false; }

    virtual const Material& getMaterial() const;
    virtual const ModelMesh* getModelMesh() const;

    virtual LightType getType() const { return LightType_Point; }
};

} } //end namespace

#endif
