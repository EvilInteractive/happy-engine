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
//Created: 13/08/2011
//Added multiple lights: 18/08/2011

#ifndef _HE_DEFERRED_3D_RENDERER_H_
#define _HE_DEFERRED_3D_RENDERER_H_
#pragma once

#include "Shader.h"
#include "IRenderer.h"
#include "DrawListContainer.h"
#include "BufferLayout.h"
#include "IDrawable2D.h"

namespace he {
namespace gui {
    class Canvas2D;
}
namespace gfx {

class LightManager;
class Bloom;
class AutoExposure;
class DrawSettings;
class Texture2D;
class ModelMesh;
class RenderTarget;
class Scene;

class HAPPY_ENTRY Deferred3DRenderer : public IRenderer, public IDrawable2D
{
private:
    struct PostPointLightData
    {
        //LightBuffer
        uint32 position;
        uint32 multiplier;
        uint32 color;
        uint32 beginAttenuation;
        uint32 endAttenuation;

        //No Buffer
        uint32 colorIllMap, normalDepthMap, sgMap;
        uint32 wvp;
    };
    struct PostSpotLightData
    {
        //LightBuffer
        uint32 position;
        uint32 multiplier;
        uint32 direction;
        uint32 beginAttenuation;
        uint32 color;
        uint32 endAttenuation;
        uint32 cosCutOff;

        //No Buffer
        uint32 colorIllMap, normalDepthMap, sgMap;
        uint32 wvp;
    };
    struct PostShadowSpotLightData
    {
        //LightBuffer
        uint32 position;
        uint32 multiplier;
        uint32 direction;
        uint32 beginAttenuation;
        uint32 color;
        uint32 endAttenuation;
        uint32 cosCutOff;

        //No Buffer
        uint32 colorIllMap, normalDepthMap, sgMap;
        uint32 wvp;

        // Shadow
        uint32 shadowMap;
        uint32 shadowMatrix;
        uint32 shadowInvSize;
    };
    struct PostAmbDirIllLightData
    {
        uint32 ambColor;
        uint32 dirColor;
        uint32 dirDirection;
        uint32 dirPosition;
        uint32 dirNearFar;

        //No Buffer
        uint32 shadowMap0, shadowMap1, shadowMap2, shadowMap3;
        uint32 colorIllMap, normalDepthMap, sgMap;
    };
    struct PostSharedData
    {
        PostSharedData(): sharedBuffer(nullptr) {}
        UniformBuffer* sharedBuffer;
        // SharedBuffer
        vec4 projParams;
    };

public:
    Deferred3DRenderer();
    virtual ~Deferred3DRenderer();

    virtual void init(View* view, const RenderTarget* target);

    void setScene(Scene* scene) { m_Scene = scene; }
    Scene* getScene() const { return m_Scene; }

    virtual RenderPass getRenderPass() const { return RenderPass_Opac; }
    virtual uint8 getRenderPriority() const { return 50; }

    virtual void preRender() {}
    virtual void render();
    virtual void draw2D(gui::Canvas2D* canvas);

private:
    static BufferLayout s_VertexLayoutFullscreenQuad;

    void onViewResized();
    void initFromSettings();
    void compileShaders();

    void postAmbDirIllLight();
    void postPointLights();
    void postSpotLights();

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
    Scene* m_Scene;
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
    bool m_SpecularEnabled;
    bool m_ShowDebugTextures;

    
    //Disable default copy constructor and default assignment operator
    Deferred3DRenderer(const Deferred3DRenderer&);
    Deferred3DRenderer& operator=(const Deferred3DRenderer&);
};

} } //end namespace

#endif
