//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "IDrawable2D.h"

namespace he {
namespace gfx {

class MaterialInstance;
class Bloom;
class AutoExposure;
class Texture2D;
class ModelMesh;
class View;
class RenderTarget;
class UniformBuffer;

class PostProcesser : public IDrawable2D
{
public:
    PostProcesser();
    ~PostProcesser();

    void init(View* view, const RenderTarget* writeTarget, const RenderTarget* readTarget);

    void setFogColor(const he::vec3& color);

    void setDebugRenderer(Renderer2D* renderer);

    void draw();
    void draw2D(gui::Canvas2D* canvas);

private:
    void initFromSettings();
    void loadMaterial();

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

    Renderer2D* m_DebugRenderer;

    const Texture2D* m_RandomNormals;
    Bloom* m_Bloom;
    AutoExposure* m_AutoExposure;
    ModelMesh* m_Quad;

    MaterialInstance* m_PostMaterial;
    int8 m_PostShaderVars[MAX_POST_SHADER_VARS];


    bool m_ShowDebugTextures;
    bool m_AOEnabled;
    bool m_FogEnabled;
    vec3 m_FogColor;

    //Disable default copy constructor and default assignment operator
    PostProcesser(const PostProcesser&);
    PostProcesser& operator=(const PostProcesser&);
};

} } //end namespace

#endif
