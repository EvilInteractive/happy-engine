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
    m_FrameBuffer.init(m_BufferCounter++, PerFrameBuffer());
    m_CameraBuffer.init(m_BufferCounter++, PerCameraBuffer());
}

void ShaderUniformBufferManager::updateFrameBuffer(const Scene* const scene)
{
    PerFrameBuffer buffer;

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


    m_FrameBuffer.updateData(buffer);
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

    m_CameraBuffer.updateData(buffer);
}

} }
