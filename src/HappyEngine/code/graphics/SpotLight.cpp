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
#include "HappyPCH.h" 

#include "SpotLight.h"

#include "ContentManager.h"
#include "ModelMesh.h"

#include "Texture2D.h"

#include "GraphicsEngine.h"

namespace he {
namespace gfx {

#define MIN_BEGIN_ATTENUATION 0.1f

SpotLight::SpotLight()
    : m_Multiplier(1.0f)
    , m_LocalDirection(0, -1, 0)
    , m_WorldDirection(0, -1, 0)
    , m_Attenuation(0.0f, 10.0f)
    , m_ScaledAttenuation(MIN_BEGIN_ATTENUATION, 10.0f)
    , m_Color(1.0f, 1.0f, 1.0f)
    , m_CosCutoff(0.5f)
    , m_Material(nullptr)
    , m_Model(nullptr)
    , m_LightVolume(nullptr)
    , m_ShadowResolution(ShadowResolution_None)
    , m_ShadowMap(nullptr)
    , m_ShadowLensDirty(true)
    , m_ShadowLookDirty(true)
{
    m_Material = ResourceFactory<Material>::getInstance()->get(CONTENT->loadMaterial("engine/light/debuglight.material"));
    BufferLayout vertexLayout;
    vertexLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));
    //m_pLightVolume = CONTENT->asyncLoadModelMesh("engine/lightvolume/spotLight.binobj", "M_SpotLight", vertexLayout); //HACK: wrong volume
    m_LightVolume = CONTENT->asyncLoadModelMesh("engine/lightvolume/pointLight.binobj", "M_PointLight", vertexLayout); //HACK: wrong volume

    ResourceFactory<gfx::ModelMesh>::getInstance()->instantiate(m_LightVolume->getHandle());
    m_Model = m_LightVolume;
}

SpotLight::~SpotLight()
{   
    if (m_Model != nullptr)
        m_Model->release();
    if (m_LightVolume != nullptr)
        m_LightVolume->release();
    if (m_Material != nullptr)
        m_Material->release();
    if (m_ShadowMap != nullptr)
        m_ShadowMap->release();
}

void SpotLight::setMultiplier(float multiplier)
{
    m_Multiplier = multiplier;
}
void SpotLight::setDirection(const vec3& direction)
{
    if (m_LocalDirection != direction)
    {
        m_LocalDirection = direction;
        setWorldMatrixDirty(Object3D::DirtyFlag_Rotate);
    }
}
void SpotLight::setAttenuation(float begin, float end)
{
    if (m_Attenuation.x != begin || m_Attenuation.y != end)
    {
        m_Attenuation.x = begin;
        m_Attenuation.y = end;
        setLocalScale(vec3(end, end, end));
        setWorldMatrixDirty(Object3D::DirtyFlag_Scale);
    }
}
void SpotLight::setColor(const vec3& color)
{
    m_Color = color;
}
void SpotLight::setColor(const Color& color)
{
    m_Color = color.rgb();
}
void SpotLight::setFov(float angle)
{
    m_CosCutoff = cosf(angle/2.0f);
    m_ShadowLensDirty = true;
}

float SpotLight::getMultiplier() const
{
    return m_Multiplier;
}
const vec3& SpotLight::getLocalDirection() const
{
    return m_LocalDirection;
}
const vec3& SpotLight::getWorldDirection() const
{
    return m_WorldDirection;
}
float SpotLight::getBeginAttenuation() const
{
    return m_Attenuation.x;
}
float SpotLight::getEndAttenuation() const
{
    return m_Attenuation.y;
}
float SpotLight::getScaledBeginAttenuation() const
{
    return m_ScaledAttenuation.x;
}
float SpotLight::getScaledEndAttenuation() const
{
    return m_ScaledAttenuation.y;
}

const vec3& SpotLight::getColor() const
{
    return m_Color;
}
float SpotLight::getCosCutoff() const
{
    return m_CosCutoff;
}
 
const ModelMesh* SpotLight::getLightVolume() const
{
    return m_LightVolume;
}
const Material* SpotLight::getMaterial() const
{
    return m_Material;
}
const ModelMesh* SpotLight::getModelMesh() const
{
    return m_Model;
}

float SpotLight::getFov() const
{
    return acosf(m_CosCutoff) * 2.0f;
}

void SpotLight::calculateWorldMatrix()
{
    DefaultSingleDrawable::calculateWorldMatrix();
    vec4 direction(m_LocalDirection, 0);
    m_WorldDirection = normalize((m_WorldMatrix * direction).xyz());
    float scale(length(vec4(m_WorldMatrix(0, 0), m_WorldMatrix(1, 0), m_WorldMatrix(2, 0), m_WorldMatrix(3, 0)))); // takes x as uniform scale
    m_ScaledAttenuation = m_Attenuation * (scale / m_Attenuation.y);
    m_ScaledAttenuation.x = std::max(m_Attenuation.x, MIN_BEGIN_ATTENUATION);
}

void SpotLight::setWorldMatrixDirty( uint8 cause )
{
    DefaultSingleDrawable::setWorldMatrixDirty(cause);
    if ((cause & DirtyFlag_Scale) != 0)
        m_ShadowLensDirty = true;
    if ((cause & (~DirtyFlag_Scale)) != 0)
        m_ShadowLookDirty = true;
}

void SpotLight::prepareShadowCamera()
{
    if (m_ShadowLookDirty == true)
    {
        vec3 position;
        getWorldMatrix().getTranslationComponent(position);

        const vec3& shadowLook(getWorldDirection());
        m_ShadowCamera.lookAt(position, position + shadowLook, dot(vec3::up, shadowLook) < 0.1? vec3::forward : vec3::up);

        m_ShadowLookDirty = false;
    }
    if (m_ShadowLensDirty == true)
    {
        m_ShadowCamera.setLens(1.0f, getFov(), 1.0f, getScaledEndAttenuation());
        m_ShadowLensDirty = false;
    }
    m_ShadowCamera.prepareForRendering();
}

void SpotLight::setShadowResolution( const ShadowResolution& resolution )
{
    if (m_ShadowResolution != resolution)
    {
        m_ShadowResolution = resolution;
        if (m_ShadowMap != nullptr && resolution == ShadowResolution_None)
        {
            m_ShadowMap->release();
            m_ShadowMap = nullptr;
        }
        if (resolution != ShadowResolution_None)
        {
            if (m_ShadowMap == nullptr)
            {
                m_ShadowMap = ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create());
                m_ShadowMap->setName("Spotlight shadowmap");
                m_ShadowMap->init(TextureWrapType_Clamp, TextureFilterType_Linear, TextureFormat_R16, false);
            }
            uint16 res(gfx::GraphicsEngine::getShadowMapSize(resolution));
            m_ShadowMap->setData(res, res, 0, TextureBufferLayout_R, TextureBufferType_Float);
        }
    }
}

} } //end namespace