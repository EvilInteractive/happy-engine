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
#include "Model.h"
#include "ModelMesh.h"
#include "IRenderer.h"
#include "DrawListContainer.h"

namespace he {
namespace gfx {

class LightManager;
class Bloom;
class AutoExposure;
class DrawSettings;
class Texture2D;

class Deferred3DRenderer : public IRenderer
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
    struct PostAmbDirIllLightData
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
        uint colorIllMap, normalMap, sgMap, depthMap, colorRamp;
    };
    struct PostSharedData
    {
        UniformBuffer::pointer pSharedBuffer;
        // SharedBuffer
        ShaderVariable<vec4> projParams;
    };

public:
    Deferred3DRenderer();
    virtual ~Deferred3DRenderer();

    virtual void init(const RenderSettings& settings, 
        const Texture2D* pOutTarget, const Texture2D* pOutNormalTarget, const Texture2D* pOutDepthTarget);

    virtual void setRenderSettings(const RenderSettings& settings);
    virtual void onScreenResized();

    virtual void draw(const DrawListContainer& drawList, uint renderFlags);

    virtual void clear(bool color, bool normal, bool depth);

    virtual bool getSupportsTranslucency() const { return false; }

private:
    static BufferLayout s_VertexLayoutFullscreenQuad;

    void compileShaders();

    void postAmbDirIllLight();
    void postPointLights();
    void postSpotLights();

    void drawDebugTextures() const;

    //////////////////////////////////////////////////////////////////////////
    ///                              FBO                                   ///
    //////////////////////////////////////////////////////////////////////////
    // Collection FBO
    uint m_CollectionFboId;
    Texture2D* m_pColorIllTexture;
    Texture2D* m_pSGTexture;
    const Texture2D* m_pNormalTexture;
    const Texture2D* m_pDepthTexture;

    // Render FBO
    uint m_RenderFboId;
    const Texture2D* m_pOutTexture;
    
    //////////////////////////////////////////////////////////////////////////
    ///                              SHADERS                               ///
    //////////////////////////////////////////////////////////////////////////
    const Texture2D* m_pColorRampTex;
    PostSharedData m_SharedShaderData;

    //Point light
    Shader* m_pPointLightShader;
    PostPointLightData m_PointLightData;

    //Spot light
    Shader* m_pSpotLightShader;
    PostSpotLightData m_SpotLightData;

    //Amb&Dir&Ill light
    Shader* m_pAmbDirIllShader;
    PostAmbDirIllLightData m_AmbDirIllLightData;

    //////////////////////////////////////////////////////////////////////////
    ///                              Data                                  ///
    ////////////////////////////////////////////////////////////////////////// 
    ModelMesh::pointer m_pQuad;

    //////////////////////////////////////////////////////////////////////////
    ///                              Settings                              ///
    ////////////////////////////////////////////////////////////////////////// 
    RenderSettings m_RenderSettings;
    bool m_ShowDebugTextures;

    
    //Disable default copy constructor and default assignment operator
    Deferred3DRenderer(const Deferred3DRenderer&);
    Deferred3DRenderer& operator=(const Deferred3DRenderer&);
};

} } //end namespace

#endif
