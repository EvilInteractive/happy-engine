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

#include "IRenderer.h"
#include "DrawListContainer.h"
#include "VertexLayout.h"
#include "IDrawable2D.h"

namespace he {
namespace gui {
    class Canvas2D;
}
namespace gfx {

class AutoExposure;
class Bloom;
class DrawSettings;
class LightManager;
class MaterialInstance;
class ModelMesh;
class RenderTarget;
class Scene;
class Texture2D;

class HAPPY_ENTRY Deferred3DRenderer : public IRenderer, public IDrawable2D
{
private:
    struct PostPointLightData
    {
        //LightBuffer
        int8 position;
        int8 multiplier;
        int8 color;
        int8 beginAttenuation;
        int8 endAttenuation;

        //No Buffer
        int8 colorIllMap, normalDepthMap, sgMap;
        int8 wvp;
    };
    struct PostSpotLightData
    {
        //LightBuffer
        int8 position;
        int8 multiplier;
        int8 direction;
        int8 beginAttenuation;
        int8 color;
        int8 endAttenuation;
        int8 cosCutOff;

        //No Buffer
        int8 colorIllMap, normalDepthMap, sgMap;
        int8 wvp;
    };
    struct PostShadowSpotLightData
    {
        //LightBuffer
        int8 position;
        int8 multiplier;
        int8 direction;
        int8 beginAttenuation;
        int8 color;
        int8 endAttenuation;
        int8 cosCutOff;

        //No Buffer
        int8 colorIllMap, normalDepthMap, sgMap;
        int8 wvp;

        // Shadow
        int8 shadowMap;
        int8 shadowMatrix;
        int8 shadowInvSize;
    };
    struct PostAmbDirIllLightData
    {
        int8 ambColor;
        int8 dirColor;
        int8 dirDirection;
        int8 dirPosition;
        int8 dirNearFar;

        //No Buffer
        int8 shadowMap0, shadowMap1, shadowMap2, shadowMap3;
        int8 colorIllMap, normalDepthMap, sgMap;
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
    void onViewResized();
    void initFromSettings();
    void loadMaterials();

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
    ///                             Material                               ///
    //////////////////////////////////////////////////////////////////////////
    const Texture2D* m_ColorRampTex;

    //Point light
    ModelMesh* m_PointLightVolume;
    MaterialInstance* m_PointLightMaterial;
    PostPointLightData m_PointLightData;

    //Spot light
    ModelMesh* m_SpotLightVolume;
    MaterialInstance* m_SpotLightMaterial, *m_ShadowSpotLightMaterial;
    PostSpotLightData m_SpotLightData;
    PostShadowSpotLightData m_ShadowSpotLightData;

    //Amb&Dir&Ill light
    MaterialInstance* m_AmbDirIllMaterial;
    PostAmbDirIllLightData m_AmbDirIllLightData;

    //////////////////////////////////////////////////////////////////////////
    ///                              Data                                  ///
    ////////////////////////////////////////////////////////////////////////// 
    ModelMesh* m_Quad;

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
