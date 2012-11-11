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
//Created: 18/12/2011

#ifndef _HE_POST_PROCESSER_H_
#define _HE_POST_PROCESSER_H_
#pragma once

#include "RenderSettings.h"
#include "IDrawable2D.h"

namespace he {
namespace gfx {

class Shader;
class Bloom;
class AutoExposure;
class Texture2D;
class ModelMesh;
class View;
class RenderTarget;
class UniformBuffer;

struct ToneMapData
{
    ToneMapData()
    : shoulderStrength(0.15f)
    , linearStrength(0.50f)
    , linearAngle(0.10f)
    , toeStrength(0.20f)
    , toeNumerator(0.02f)
    , toeDenominator(0.30f)
    , exposureBias(3.0f)
    {}

    float shoulderStrength;
    float linearStrength;
    float linearAngle;
    float toeStrength;
    float toeNumerator;
    float toeDenominator;
    float exposureBias;
};

class PostProcesser : public IDrawable2D
{
public:
    PostProcesser();
    virtual ~PostProcesser();

    void init(View* view, const RenderTarget* writeTarget, const RenderTarget* readTarget);

    void setFogColor(const he::vec3& color);
    const ToneMapData& getToneMapData() const { return m_ToneMapData; }
    ToneMapData& getToneMapDataForEdit() { return m_ToneMapData; }

    void setDebugRenderer(Renderer2D* renderer);

    void draw();
    virtual void draw2D(Canvas2D* canvas);

private:
    void onSettingsChanged(const RenderSettings& settings, bool force = false);
    void compileShader();

    enum PostShaderVar
    {
        PV_Bloom0 = 0,
        PV_Bloom1,
        PV_Bloom2,
        PV_Bloom3,

        PV_SSAORandomNormals,
        PV_SSAORadius,
        PV_SSAOIntensity,
        PV_SSAOScale,
        PV_SSAOBias,

        PV_ProjParams,
        PV_ViewPortSize,

        PV_LumMap,
        PV_NormalDepthMap,
        PV_ColorMap,

        PV_FogColor,
        PV_ToneMapData,

        MAX_POST_SHADER_VARS
    };

    View* m_View;
    const RenderTarget* m_WriteRenderTarget;
    const RenderTarget* m_ReadRenderTarget;
    RenderSettings m_Settings;

    Renderer2D* m_DebugRenderer;

    Bloom* m_Bloom;
    AutoExposure* m_AutoExposure;
    UniformBuffer* m_ToneMapUniformBuffer;

    Shader* m_PostShader;
    uint32 m_PostShaderVars[MAX_POST_SHADER_VARS];

    const Texture2D* m_RandomNormals;

    bool m_ShowDebugTextures;

    ModelMesh* m_Quad;

    vec3 m_FogColor;
    ToneMapData m_ToneMapData;

    //Disable default copy constructor and default assignment operator
    PostProcesser(const PostProcesser&);
    PostProcesser& operator=(const PostProcesser&);
};

} } //end namespace

#endif
