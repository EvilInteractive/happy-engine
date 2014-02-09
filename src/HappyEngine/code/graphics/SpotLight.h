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

#ifndef _HE_SpotLight_H_
#define _HE_SpotLight_H_
#pragma once

#include "Drawable.h"
#include "Light.h"
#include "CameraPerspective.h"
#include "IDrawable2D.h"

namespace he {
namespace gfx {
class Texture2D;

class SpotLight : public Drawable, public Light
{
private:
    float m_Multiplier;

    vec3 m_LocalDirection;
    mutable vec3 m_WorldDirection;

    vec2 m_Attenuation;
    mutable vec2 m_ScaledAttenuation;

    vec3 m_Color;
    float m_CosCutoff;

    ModelMesh* m_LightVolume;

    ShadowResolution m_ShadowResolution;
    Texture2D* m_ShadowMap;
    CameraPerspective m_ShadowCamera;
    bool m_ShadowLookDirty;
    bool m_ShadowLensDirty;

protected:
    virtual void calculateWorldMatrix(); // override Object3D
    virtual void setWorldMatrixDirty(const uint8 cause); // override Object3D

public:
    SpotLight();
    virtual ~SpotLight();

    void setMultiplier(float multiplier);
    void setDirection(const vec3& direction);
    void setAttenuation(float begin, float end);
    void setColor(const vec3& color);
    void setColor(const Color& color);
    void setFov(float angle);

    float getMultiplier() const;

    const vec3& getLocalDirection() const;
    const vec3& getWorldDirection() const;

    float getBeginAttenuation() const;
    float getEndAttenuation() const;
    float getScaledBeginAttenuation() const;
    float getScaledEndAttenuation() const;

    const vec3& getColor() const;
    float getCosCutoff() const;
    float getFov() const;

    void setShadowResolution(const ShadowResolution& resolution);
    const ShadowResolution& getShadowResolution() const { return m_ShadowResolution; }
    Texture2D* getShadowMap() const { return m_ShadowMap; }
    const CameraPerspective& getShadowCamera() { return m_ShadowCamera;}
    void prepareShadowCamera();

    const ModelMesh* getLightVolume() const;

    virtual LightType getType() const { return LightType_Spot; }

private:
    SpotLight(const SpotLight& other);
    SpotLight& operator=(const SpotLight& other);
};

} } //end namespace

#endif
