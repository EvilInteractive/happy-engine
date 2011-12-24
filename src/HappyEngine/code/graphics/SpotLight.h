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
//Created: 07/10/2011

#ifndef _HE_SpotLight_H_
#define _HE_SpotLight_H_
#pragma once


#include "vec3.h"
#include "mat44.h"
#include "Color.h"
#include "ModelMesh.h"

#include "boost/shared_ptr.hpp"

#include "DefaultSingleDrawable.h"

namespace he {
namespace gfx {

class SpotLight : public DefaultSingleDrawable
{
private:
    vec3 m_Position;
    float m_Multiplier;
    vec3 m_Direction;
    float m_BeginAttenuation;
    vec3 m_Color;
    float m_EndAttenuation;
    float m_CosCutoff;

    mat44 m_mtxWorld;
    void calculateWorld();

    ModelMesh::pointer m_pLightVolume;
    ModelMesh::pointer m_pModel;
    Material m_Material;

public:
    SpotLight();
    ~SpotLight();
    //default copy constructor and assignment operator are fine

    void setPosition(const vec3& position);
    void setMultiplier(float multiplier);
    void setDirection(const vec3& direction);
    void setAttenuation(float begin, float end);
    void setColor(const vec3& color);
    void setColor(const Color& color);
    void setFov(float angle);

    const vec3& getPosition() const;
    float getMultiplier() const;
    const vec3& getDirection() const;
    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    const vec3& getColor() const;
    float getCosCutoff() const;
    
    mat44 getWorldMatrix() const;
    const ModelMesh::pointer& getLightVolume() const;

    virtual bool getCastsShadow() const { return false; }
    
    virtual const Material& getMaterial() const;
    virtual const ModelMesh::pointer& getModelMesh() const;
    
    typedef boost::shared_ptr<SpotLight> pointer;
};

} } //end namespace

#endif
