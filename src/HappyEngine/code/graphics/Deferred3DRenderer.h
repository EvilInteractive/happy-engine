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
//Created: 13/08/2011
//Added multiple lights: 18/08/2011

#ifndef _HE_DEFERRED_3D_RENDERER_H_
#define _HE_DEFERRED_3D_RENDERER_H_
#pragma once

#include "Shader.h"
#include "IRenderer.h"
#include "DrawListContainer.h"
#include "BufferLayout.h"
#include "RenderSettings.h"
#include "IDrawable2D.h"

namespace he {
namespace gfx {

class LightManager;
class Bloom;
class AutoExposure;
class DrawSettings;
class Texture2D;
class ModelMesh;
class RenderTarget;
class Scene;

class Deferred3DRenderer : public IRenderer, public IDrawable2D
{
private:
    struct PostPointLightData
    {
        //LightBuffer
        uint position;
        uint multiplier;
        uint color;
        uint beginAttenuation;
        uint endAttenuation;

        //No Buffer
        uint colorIllMap, normalDepthMap, sgMap;
        uint wvp;
    };
    struct PostSpotLightData
    {
        //LightBuffer
        uint position;
        uint multiplier;
        uint direction;
        uint beginAttenuation;
        uint color;
        uint endAttenuation;
        uint cosCutOff;

        //No Buffer
        uint colorIllMap, normalDepthMap, sgMap;
        uint wvp;
    };
    struct PostShadowSpotLightData
    {
        //LightBuffer
        uint position;
        uint multiplier;
        uint direction;
        uint beginAttenuation;
        uint color;
        uint endAttenuation;
        uint cosCutOff;

        //No Buffer
        uint colorIllMap, normalDepthMap, sgMap;
        uint wvp;

        // Shadow
        uint shadowMap;
        uint shadowMatrix;
    };
    struct PostAmbDirIllLightData
    {
        uint ambColor;
        uint dirColor;
        uint dirDirection;
        uint dirPosition;
        uint dirNearFar;

        //No Buffer
        uint shadowMap0, shadowMap1, shadowMap2, shadowMap3;
        uint colorIllMap, normalDepthMap, sgMap;
    };
    struct PostSharedData
    {
        UniformBuffer* pSharedBuffer;
        // SharedBuffer
        ShaderVariable<vec4> projParams;
    };

public:
    Deferred3DRenderer();
    virtual ~Deferred3DRenderer();

    virtual void init(View* view, const RenderTarget* target);

    virtual void draw();
    virtual void draw2D(Canvas2D* canvas);

private:
    static BufferLayout s_VertexLayoutFullscreenQuad;

    void onViewResized();
    void onSettingChanged();
    void compileShaders();

    void postAmbDirIllLight(const Scene* scene);
    void postPointLights(const Scene* scene);
    void postSpotLights(const Scene* scene);



    //////////////////////////////////////////////////////////////////////////
    ///                              Draw Data                             ///
    //////////////////////////////////////////////////////////////////////////
    // Collection FBO
    RenderTarget* m_CollectionRenderTarget;
    Texture2D* m_ColorIllTexture;
    Texture2D* m_SGTexture;
    const Texture2D* m_NormalDepthTexture;

    // Render FBO
    View* m_View;
    const RenderTarget* m_OutputRenderTarget;
    
    //////////////////////////////////////////////////////////////////////////
    ///                              SHADERS                               ///
    //////////////////////////////////////////////////////////////////////////
    const Texture2D* m_pColorRampTex;
    PostSharedData m_SharedShaderData;

    //Point light
    Shader* m_PointLightShader;
    PostPointLightData m_PointLightData;

    //Spot light
    Shader* m_SpotLightShader, *m_ShadowSpotLightShader;
    PostSpotLightData m_SpotLightData;
    PostShadowSpotLightData m_ShadowSpotLightData;

    //Amb&Dir&Ill light
    Shader* m_AmbDirIllShader;
    PostAmbDirIllLightData m_AmbDirIllLightData;

    //////////////////////////////////////////////////////////////////////////
    ///                              Data                                  ///
    ////////////////////////////////////////////////////////////////////////// 
    ModelMesh* m_pQuad;

    //////////////////////////////////////////////////////////////////////////
    ///                              Settings                              ///
    ////////////////////////////////////////////////////////////////////////// 
    LightingSettings m_Settings;
    bool m_ShowDebugTextures;

    
    //Disable default copy constructor and default assignment operator
    Deferred3DRenderer(const Deferred3DRenderer&);
    Deferred3DRenderer& operator=(const Deferred3DRenderer&);
};

} } //end namespace

#endif
