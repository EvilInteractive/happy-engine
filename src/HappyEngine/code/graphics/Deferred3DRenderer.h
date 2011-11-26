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
//Created: 13/08/2011
//Added multiple lights: 18/08/2011

#ifndef _HE_DEFERRED_3D_RENDERER_H_
#define _HE_DEFERRED_3D_RENDERER_H_
#pragma once

#include "HappyTypes.h"
#include "Shader.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

class LightManager;
class Camera;
class Bloom;
class AutoExposure;
class DrawSettings;

class Deferred3DRenderer
{
private:
    struct PostPointLightData
    {
        //Buffer
        UniformBuffer::pointer pLightBuffer;

        //LightBuffer
        ShaderVariable<vec3> position;
        ShaderVariable<float> multiplier;
        ShaderVariable<vec3> color;
        ShaderVariable<float> beginAttenuation;
        ShaderVariable<float> endAttenuation;

        //No Buffer
        uint colorIllMap, normalMap, sgMap, depthMap;
        uint wvp;
    };
    struct PostSpotLightData
    {
        //Buffer
        UniformBuffer::pointer pLightBuffer;

        //LightBuffer
        ShaderVariable<vec3> position;
        ShaderVariable<float> multiplier;
        ShaderVariable<vec3> direction;
        ShaderVariable<float> beginAttenuation;
        ShaderVariable<vec3> color;
        ShaderVariable<float> endAttenuation;
        ShaderVariable<float> cosCutOff;

        //No Buffer
        uint colorIllMap, normalMap, sgMap, depthMap;
        uint wvp;
    };
    struct PostAmbIllLightData
    {
        //Buffer
        UniformBuffer::pointer pLightBuffer;
        UniformBuffer::pointer pPerFrameBuffer;

        //LightBuffer
        ShaderVariable<vec4> ambColor;
        ShaderVariable<vec4> dirColor;
        ShaderVariable<vec3> dirDirection;

        //PerFrameBuffer
        ShaderVariable<mat44> mtxDirLight0;
        ShaderVariable<mat44> mtxDirLight1;
        ShaderVariable<mat44> mtxDirLight2;
        ShaderVariable<mat44> mtxDirLight3;

        //No Buffer
        uint shadowMap0, shadowMap1, shadowMap2, shadowMap3;
        uint colorIllMap, normalMap, sgMap, depthMap;
    };
    struct PostSharedData
    {
        UniformBuffer::pointer pSharedBuffer;
        // SharedBuffer
        ShaderVariable<vec4> projParams;
    };

public:
    Deferred3DRenderer(const DrawSettings& settings);
    struct SSAOSettings
    {
        float radius;
        float intensity;
        float scale;
        float bias;
        uint minIterations;
        uint maxIterations;
        uint passes;
    };
    virtual ~Deferred3DRenderer();

    void begin(const Camera* pCamera);
    void end();

    void resized();

    LightManager* getLightManager() const;
    const Texture2D::pointer& getDepthTexture() const;

    static const VertexLayout& getVertexLayoutLightVolume();

    void enableSSAO(bool enable);
    void setSSAOSettings(const SSAOSettings& settings);

private:
    static VertexLayout s_VertexLayoutFullscreenQuad;

    void postAmbIllLight();
    void postPointLights();
    void postSpotLights();
    void postToneMap();
    void calculateExposure();

    void initToneMapShader(const std::string& folder, const ShaderLayout& layout);

    //FBO 1
    uint m_CollectionFboId;
    // Textures:
    //      - Color.rgb, ill       GL_RGBA8
    //      - spec, gloss,         GL_RGBA8
    //      - Normal.xy            GL_R16G16
    //      - Depth                GL_DEPTH24_STENCIL8
    static const int TEXTURES = 4;
    static const int TEXTURE_FORMAT[TEXTURES];
    static const int TEXTURE_INTERNALFORMAT[TEXTURES];
    static const int TEXTURE_ATTACHMENT[TEXTURES];
    uint m_TextureId[TEXTURES];
    Texture2D::pointer m_pTexture[TEXTURES];

    //FBO 2
    uint m_RenderFboId;
    uint m_RenderTextureId;
    Texture2D::pointer m_pRenderTexture;

    //SHARED FBO
    uint m_DepthBufferId;


    //SHADERS
    PostSharedData m_SharedShaderData;

    Shader* m_pPointLightShader;
    PostPointLightData m_PointLightData;

    Shader* m_pSpotLightShader;
    PostSpotLightData m_SpotLightData;
    
    Shader* m_pAmbIllShader;
    PostAmbIllLightData m_AmbIllLightData;

    Shader* m_pToneMapShader;
    uint m_ToneMapShaderPos[8]; //8 values
    float m_Exposure;
    float m_Gamma;

    ModelMesh::pointer m_pQuad;
    LightManager* m_pLightManager;   
    const Camera* m_pCamera;
    Bloom* m_pBloom;
    AutoExposure* m_pAutoExposure;

    bool m_ShowDebugTextures;
    bool m_Bloom;

    bool m_bSSAO;
    uint m_ShaderSSAOPos[11];
    Texture2D::pointer m_pRandomNormals;
    SSAOSettings m_SSAOSettings;
    
    //Disable default copy constructor and default assignment operator
    Deferred3DRenderer(const Deferred3DRenderer&);
    Deferred3DRenderer& operator=(const Deferred3DRenderer&);
};

} } //end namespace

#endif
