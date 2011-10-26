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
#include "LightManager.h"
#include "Camera.h"
#include "Bloom.h"
#include "AutoExposure.h"
#include "DrawSettings.h"

namespace he {
namespace gfx {

class Deferred3DRenderer
{
public:
	Deferred3DRenderer(const DrawSettings& settings);
    virtual ~Deferred3DRenderer();
        
    virtual void draw(const Model::pointer& pModel);
    virtual void draw(const ModelMesh::pointer& pModel);

    void begin(const Camera* pCamera);
    void end();

    LightManager* getLightManager() const;

    static const VertexLayout& getVertexLayoutLightVolume();

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

    enum LightVolumeType
    {
        LightVolumeType_PointLight = 0,
        LightVolumeType_SpotLight = 1
    };

    static const int LIGHTVOLUME_SHADERS = 2;
    Shader* m_pPostLightVolumeShader[LIGHTVOLUME_SHADERS];
    uint m_ShaderLVColMapPos[LIGHTVOLUME_SHADERS], 
         m_ShaderLVNormalMapPos[LIGHTVOLUME_SHADERS], 
         m_ShaderLVSGMapPos[LIGHTVOLUME_SHADERS],
         m_ShaderLVDepthMapPos[LIGHTVOLUME_SHADERS];
    uint m_ShaderLVPLPos[5]; //5 values
    uint m_ShaderLVSLPos[7]; //7 values
    uint m_ShaderLVProjParams[LIGHTVOLUME_SHADERS],
         m_ShaderLVWVP[LIGHTVOLUME_SHADERS];
    
    Shader* m_pAmbIllShader;
    uint m_ShaderAmbIllPos[17]; //17 values

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
    
    //Disable default copy constructor and default assignment operator
    Deferred3DRenderer(const Deferred3DRenderer&);
    Deferred3DRenderer& operator=(const Deferred3DRenderer&);
};

} } //end namespace

#endif
