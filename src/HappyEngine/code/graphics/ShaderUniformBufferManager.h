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

#ifndef _HE_ShaderUniformBufferManager_H_
#define _HE_ShaderUniformBufferManager_H_
#pragma once

#include "ShaderUniformBuffer.h"

namespace he {
namespace gfx {

class Scene;
class ICamera;

class ShaderUniformBufferManager
{
    UNIFORM_STRUCT(PerSceneBuffer)
        UNIFORM_FLOAT_FLOAT_FLOAT_FLOAT(m_HDRShoulderStrength, m_HDRLinearStrength, m_HDRLinearAngle, m_HDRToeStrength);
        UNIFORM_FLOAT_FLOAT_FLOAT_FLOAT(m_HDRToeNumerator, m_HDRToeDenominator, m_HDRExposureBias, __pad_HDR);
        UNIFORM_VEC4(m_AmbientLightColor);
        UNIFORM_VEC4(m_DirectionalLightColor);
        UNIFORM_VEC3(m_DirectionalLightDirection);

        PerSceneBuffer()
            : m_AmbientLightColor(1.0f, 1.0f, 1.0f, 0.5f)
            , m_DirectionalLightColor(1.0f, 1.0f, 1.0f, 2.0f)
            , m_DirectionalLightDirection(0.0f, -1.0f, 0.0f)
        {}
    UNIFORM_STRUCT_END(PerSceneBuffer)

    UNIFORM_STRUCT(PerCameraBuffer)
        UNIFORM_MAT4(m_View);
        UNIFORM_MAT4(m_Projection);
        UNIFORM_MAT4(m_ViewProjection);
        UNIFORM_VEC3(m_CameraPosition); // isn't this in the view matrix? it is.. but for simplicity
        UNIFORM_VEC2_VEC2(m_NearFar, m_ViewSize);
        UNIFORM_VEC4(m_ProjParams);

        PerCameraBuffer()
            : m_View(mat44::Identity)
            , m_Projection(mat44::Identity)
            , m_ViewProjection(mat44::Identity)
            , m_CameraPosition(0.0f)
            , m_NearFar(1.0f, 100.0f)
            , m_ViewSize(1280, 720)
        {}
    UNIFORM_STRUCT_END(PerCameraBuffer)

public:
    ShaderUniformBufferManager();
    ~ShaderUniformBufferManager();

    void init();

    void updateSceneBuffer(const Scene* const scene);
    void updateCameraBuffer(const ICamera* const camera);

    uint32 findLink(const he::FixedString& name) const;

private:
    SharedUniformBuffer<PerSceneBuffer> m_FrameBuffer;
    SharedUniformBuffer<PerCameraBuffer> m_CameraBuffer;

    size_t m_BufferCounter;

    //Disable default copy constructor and default assignment operator
    ShaderUniformBufferManager(const ShaderUniformBufferManager&);
    ShaderUniformBufferManager& operator=(const ShaderUniformBufferManager&);
};

} } //end namespace

#endif
