//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/09/28
#include "HappyPCH.h" 

#include "ShaderUniformBufferManager.h"

#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "ICamera.h"
#include "LightManager.h"
#include "Scene.h"

namespace he {
namespace gfx {

ShaderUniformBufferManager::ShaderUniformBufferManager()
    : m_BufferCounter(0)
{
}


ShaderUniformBufferManager::~ShaderUniformBufferManager()
{
}

void ShaderUniformBufferManager::init()
{
    m_SceneBuffer.init(m_BufferCounter++, PerSceneBuffer(), true);
    m_CameraBuffer.init(m_BufferCounter++, PerCameraBuffer(), true);
    m_SpotLightBuffer.init(m_BufferCounter++, SpotLightUniformBuffer(), true);
    m_PointLightBuffer.init(m_BufferCounter++, PointLightUniformBuffer(), true);
}

void ShaderUniformBufferManager::updateSceneBuffer(const Scene* const scene)
{
    PerSceneBuffer buffer;

    const LightManager* const lightMan(scene->getLightManager());

    const ToneMapData& toneMapData(lightMan->getToneMapData());
    buffer.m_HDRShoulderStrength = toneMapData.shoulderStrength;
    buffer.m_HDRLinearStrength = toneMapData.linearStrength;
    buffer.m_HDRLinearAngle = toneMapData.linearAngle;
    buffer.m_HDRToeStrength = toneMapData.toeStrength;
    buffer.m_HDRToeNumerator = toneMapData.toeNumerator;
    buffer.m_HDRToeDenominator = toneMapData.toeDenominator;
    buffer.m_HDRExposureBias = toneMapData.exposureBias;

    const AmbientLight* const ambLight(lightMan->getAmbientLight());
    buffer.m_AmbientLightColor = vec4(ambLight->color, ambLight->multiplier);

    const DirectionalLight* const dirLight(lightMan->getDirectionalLight());
    buffer.m_DirectionalLightColor = vec4(dirLight->getColor(), dirLight->getMultiplier());
    buffer.m_DirectionalLightDirection = dirLight->getDirection();


    m_SceneBuffer.updateData(buffer);
}

void ShaderUniformBufferManager::updateCameraBuffer(const ICamera* const camera)
{
    PerCameraBuffer buffer;

    buffer.m_View = camera->getView();
    buffer.m_Projection = camera->getProjection();
    buffer.m_ViewProjection = camera->getViewProjection();
    buffer.m_CameraPosition = camera->getPosition();
    buffer.m_NearFar.x = camera->getNearClip();
    buffer.m_NearFar.y = camera->getFarClip();
    const RectI& viewport(GL::heGetViewport());
    buffer.m_ViewSize.x = static_cast<float>(viewport.width);
    buffer.m_ViewSize.y = static_cast<float>(viewport.height);
    buffer.m_ProjParams = vec4(camera->getProjection()(0, 0), camera->getProjection()(1, 1), camera->getNearClip(), camera->getFarClip());

    m_CameraBuffer.updateData(buffer);
}

void ShaderUniformBufferManager::updateSpotLightBuffer( const SpotLight* const light, const ICamera* const camera )
{
    SpotLightUniformBuffer buffer;
    buffer.m_WVP = camera->getViewProjection() * light->getWorldMatrix();
    buffer.m_ColorMult = vec4(light->getColor(), light->getMultiplier());
    buffer.m_DirectionCosCutOff = vec4(normalize((camera->getView() * vec4(light->getWorldDirection(), 0)).xyz()), light->getCosCutoff());
    vec3 position;
    light->getWorldMatrix().getTranslationComponent(position);
    buffer.m_ViewPosition = camera->getView() * position;
    buffer.m_Attenuation = vec2(light->getScaledBeginAttenuation(), light->getScaledEndAttenuation());

    m_SpotLightBuffer.updateData(buffer);
}

void ShaderUniformBufferManager::updatePointLightBuffer( const PointLight* const light, const ICamera* const camera )
{
    PointLightUniformBuffer buffer;
    buffer.m_WVP = camera->getViewProjection() * light->getWorldMatrix();
    buffer.m_ColorMult = vec4(light->getColor(), light->getMultiplier());
    vec3 position;
    light->getWorldMatrix().getTranslationComponent(position);
    buffer.m_ViewPosition = camera->getView() * position;
    buffer.m_Attenuation = vec2(light->getScaledBeginAttenuation(), light->getScaledEndAttenuation());

    m_PointLightBuffer.updateData(buffer);
}

he::uint32 ShaderUniformBufferManager::findLink( const he::FixedString& name ) const
{
    if (name == HEFS::strSharedPerCameraUniformBuffer)
    {
        return m_CameraBuffer.getBufferID();
    }
    else if (name == HEFS::strSharedPerSceneUniformBuffer)
    {
        return m_SceneBuffer.getBufferID();
    }
    else if (name == HEFS::strSharedSpotLightUniformBuffer)
    {
        return m_SpotLightBuffer.getBufferID();
    }
    else if (name == HEFS::strSharedPointLightUniformBuffer)
    {
        return m_PointLightBuffer.getBufferID();
    }
    return UINT32_MAX;
}

void ShaderUniformBufferManager::bind()
{
    m_SceneBuffer.bindBuffer();
    m_CameraBuffer.bindBuffer();
    m_PointLightBuffer.bindBuffer();
    m_SpotLightBuffer.bindBuffer();
}


} }
