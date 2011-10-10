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

namespace he {
namespace gfx {

class Deferred3DRenderer
{
public:
	Deferred3DRenderer();
    virtual ~Deferred3DRenderer();
        
    virtual void draw(const Model::pointer& pModel);
    virtual void draw(const ModelMesh::pointer& pModel);

    void begin(const Camera* pCamera);
    void end();

    LightManager* getLightManager() const;

    static const VertexLayout& getVertexLayoutLightVolume();

private:
    static VertexLayout s_VertexLayoutLightVolume;

    void postAmbientLights();
    void postPointLights();
    void postSpotLights();
    void postDirectionalLights();

    uint m_FboId;

    // Textures:
    //      - Color.rgb, ill       GL_RGBA8
    //      - spec, gloss, depth   GL_RGBA8
    //      - Normal.xy            GL_R16G16
    //      - Depth                GL_DEPTH24_STENCIL8
    static const int TEXTURES = 4;
    static const int TEXTURE_FORMAT[TEXTURES];
    static const int TEXTURE_INTERNALFORMAT[TEXTURES];
    static const int TEXTURE_ATTACHMENT[TEXTURES];
    uint m_TextureId[TEXTURES];
    uint m_DepthBufferId;

    enum LightType
    {
        LightType_AmbientLight = 0,
        LightType_PointLight = 1,
        LightType_SpotLight = 2,
        LightType_DirectionalLight = 3
    };

    static const int SHADERS = 4;
    Shader* m_pPostShader[SHADERS];
    uint m_ShaderColMapPos[SHADERS], 
         m_ShaderNormalMapPos[SHADERS], 
         m_ShaderSGIMapPos[SHADERS],
         m_ShaderDepthMapPos[SHADERS];
    uint m_ShaderALPos[2]; //2 values
    uint m_ShaderPLPos[5]; //5 values
    uint m_ShaderSLPos[7]; //7 values
    uint m_ShaderDLPos[3]; //3 values
    uint m_ShaderProjAB[SHADERS],
         m_ShaderWVP[SHADERS],
         m_ShaderWV[SHADERS],
         m_ShaderNearFar[SHADERS],
         m_ShaderTopRight[SHADERS],
         m_ShaderInvMtxProj[SHADERS];

    ModelMesh::pointer m_pModel;

    Texture2D::pointer m_pTexture[TEXTURES];

    LightManager* m_pLightManager;
    
    const Camera* m_pCamera;
    
    //Disable default copy constructor and default assignment operator
    Deferred3DRenderer(const Deferred3DRenderer&);
    Deferred3DRenderer& operator=(const Deferred3DRenderer&);
};

} } //end namespace

#endif
