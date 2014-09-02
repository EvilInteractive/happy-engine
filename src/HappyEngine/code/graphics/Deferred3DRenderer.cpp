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
//Revising RenderEngine: 17/12/2011
//Revising Materials: 31/07/2014
#include "HappyPCH.h" 

#include "Deferred3DRenderer.h"
#include "Renderer2D.h"
#include "VertexLayout.h"
#include "Vertex.h"

#include "LightManager.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

#include "GraphicsEngine.h"
#include "ContentManager.h"

#include "CameraManager.h"
#include "ICamera.h"
#include "CameraBound.h"
#include "LightFactory.h"

#include "RenderTarget.h"
#include "Scene.h"
#include "View.h"
#include "Window.h"

#include "Texture2D.h"
#include "ModelMesh.h"
#include "MaterialInstance.h"
#include "Canvas2D.h"
#include "GlobalSettings.h"
#include "MaterialInstance.h"
#include "ShaderUniformBufferManager.h"

#include "Drawable.h"
#include "DrawContext.h"

namespace he {
namespace gfx {
    
Deferred3DRenderer::Deferred3DRenderer(): 
            m_Quad(nullptr), 
            m_ShowDebugTextures(false),
            m_PointLightVolume(nullptr),
            m_PointLightMaterial(nullptr),
            m_SpotLightVolume(nullptr),
            m_SpotLightMaterial(nullptr),
            m_ShadowSpotLightMaterial(nullptr),
            m_AmbDirIllMaterial(nullptr),
            m_CollectionRenderTarget(nullptr),
            m_OutputRenderTarget(nullptr),
            m_View(nullptr),
            m_Scene(nullptr),
            m_NormalDepthTexture(nullptr),
            m_SpecularEnabled(false)
{
    ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
    m_ColorIllTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_ColorIllTexture->setName("Deferred3DRenderer::m_ColorIllTexture");
    m_ColorIllTexture->init(TextureWrapType_Clamp, TextureFilterType_Nearest, 
        TextureFormat_RGBA8, false);

    handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_SGTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_SGTexture->setName("Deferred3DRenderer::m_SGTexture");
    m_SGTexture->init(TextureWrapType_Clamp, TextureFilterType_Nearest, 
        TextureFormat_RGBA8, false);

}
void Deferred3DRenderer::init( View* view, const RenderTarget* target )
{
    HE_ASSERT(m_View == nullptr, "Deferred3DRenderer inited twice!");
    
    m_View = view;
    m_CollectionRenderTarget = NEW RenderTarget(m_View->getWindow()->getContext());
    m_OutputRenderTarget = target;

    m_NormalDepthTexture = target->getTextureTarget(1);

    eventCallback0<void> viewportSizeChangedHandler(std::bind(&Deferred3DRenderer::onViewResized, this));
    m_View->ViewportSizeChanged += viewportSizeChangedHandler;

    m_Quad = CONTENT->getFullscreenQuad();

    loadMaterials();
    onViewResized();

    //FBO Collection
    m_CollectionRenderTarget->removeAllTargets();
    m_CollectionRenderTarget->addTextureTarget(m_ColorIllTexture);
    m_CollectionRenderTarget->addTextureTarget(m_SGTexture);
    m_CollectionRenderTarget->addTextureTarget(m_NormalDepthTexture);
    m_CollectionRenderTarget->setDepthTarget(target->getDepthBuffer());
    m_CollectionRenderTarget->init();
}


Deferred3DRenderer::~Deferred3DRenderer()
{
    m_ColorIllTexture->release();
    m_SGTexture->release();

    delete m_CollectionRenderTarget;
    
    m_Quad->release();

    if (m_PointLightVolume)
    {
        m_PointLightVolume->release();
        m_PointLightVolume = nullptr;
    }
    delete m_PointLightMaterial;    

    if (m_SpotLightVolume)
    {
        m_SpotLightVolume->release();
        m_SpotLightVolume = nullptr;
    }
    delete m_SpotLightMaterial;    
    delete m_ShadowSpotLightMaterial;
    delete m_AmbDirIllMaterial;
}
void Deferred3DRenderer::loadMaterials()
{
    const RenderSettings& settings(GlobalSettings::getInstance()->getRenderSettings());

    //////////////////////////////////////////////////////////////////////////
    ///                                 CLEAN                              ///
    //////////////////////////////////////////////////////////////////////////
    if (m_PointLightVolume)
    {
        m_PointLightVolume->release();
        m_PointLightVolume = nullptr;
    }
    delete m_PointLightMaterial;    
    m_PointLightMaterial = nullptr;
    if (m_SpotLightVolume)
    {
        m_SpotLightVolume->release();
        m_SpotLightVolume = nullptr;
    }
    delete m_SpotLightMaterial;     
    m_SpotLightMaterial = nullptr;
    delete m_ShadowSpotLightMaterial; 
    m_ShadowSpotLightMaterial = nullptr;
    delete m_AmbDirIllMaterial; 
    m_AmbDirIllMaterial = nullptr;
    
    //////////////////////////////////////////////////////////////////////////
    ///                                 Load                               ///
    //////////////////////////////////////////////////////////////////////////
    he::ObjectList<he::String> shaderDefines;
    m_SpecularEnabled = settings.lightingSettings.enableSpecular;

    // Pointlight
    {
        // Load material
        {
            const Material* const pointLightMaterial(CONTENT->loadMaterial("engine/deferred/pointLight.hm"));
            m_PointLightMaterial = pointLightMaterial->createMaterialInstance(eShaderRenderType_Normal);
            pointLightMaterial->release();
        }
        // Load mesh
        {
            m_PointLightVolume = CONTENT->asyncLoadModelMesh("engine/lightvolume/pointlight.binobj", "M_PointLight");
            m_PointLightVolume->callbackOnceIfLoaded(this, [this](const ELoadResult result)
            {
                if (result == eLoadResult_Success)
                {
                    if (m_PointLightMaterial->getLoadResult() == eLoadResult_Unloaded)
                    {
                        m_PointLightMaterial->callbackOnceIfLoaded(this, [this](const ELoadResult /*result*/)
                        {
                            m_PointLightMaterial->calculateMaterialLayout(m_PointLightVolume->getVertexLayout());
                        });
                    }
                    else
                    {
                        m_PointLightMaterial->calculateMaterialLayout(m_PointLightVolume->getVertexLayout());
                    }
                }
                else
                {
                    HAPPYENGINE->quit(); // fatal if this happen
                }
            });
        }

        m_PointLightData.position = m_PointLightMaterial->findParameter(HEFS::strlight_position);
        m_PointLightData.multiplier = m_PointLightMaterial->findParameter(HEFS::strlight_multiplier);
        m_PointLightData.color = m_PointLightMaterial->findParameter(HEFS::strlight_color);
        m_PointLightData.beginAttenuation = m_PointLightMaterial->findParameter(HEFS::strlight_beginAttenuation);
        m_PointLightData.endAttenuation = m_PointLightMaterial->findParameter(HEFS::strlight_endAttenuation);
        m_PointLightData.colorIllMap = m_PointLightMaterial->findParameter(HEFS::strcolorIllMap);
        m_PointLightData.normalDepthMap = m_PointLightMaterial->findParameter(HEFS::strnormalDepthMap);
        if (m_SpecularEnabled)
            m_PointLightData.sgMap = m_PointLightMaterial->findParameter(HEFS::strsgMap);
        m_PointLightData.wvp = m_PointLightMaterial->findParameter(HEFS::strmtxWVP);
    }

    // SpotLight
    {
        // Load material
        {
            const Material* const spotLightMaterial(CONTENT->loadMaterial("engine/deferred/spotLight.hm"));
            m_SpotLightMaterial = spotLightMaterial->createMaterialInstance(eShaderRenderType_Normal);
            spotLightMaterial->release();
        }
        // Load mesh
        {
            m_SpotLightVolume = CONTENT->asyncLoadModelMesh("engine/lightvolume/pointlight.binobj", "M_PointLight");
            m_SpotLightVolume->callbackOnceIfLoaded(this, [this](const ELoadResult result)
            {
                if (result == eLoadResult_Success)
                {
                    if (m_SpotLightMaterial->getLoadResult() == eLoadResult_Unloaded)
                    {
                        m_SpotLightMaterial->callbackOnceIfLoaded(this, [this](const ELoadResult /*result*/)
                        {
                            m_SpotLightMaterial->calculateMaterialLayout(m_SpotLightVolume->getVertexLayout());
                        });
                    }
                    else
                    {
                        m_SpotLightMaterial->calculateMaterialLayout(m_SpotLightVolume->getVertexLayout());
                    }
                }
                else
                {
                    HAPPYENGINE->quit(); // fatal if this happen
                }
            });
        }

        m_SpotLightData.position = m_SpotLightMaterial->findParameter(HEFS::strlight_position);
        m_SpotLightData.multiplier = m_SpotLightMaterial->findParameter(HEFS::strlight_multiplier);
        m_SpotLightData.direction = m_SpotLightMaterial->findParameter(HEFS::strlight_direction);
        m_SpotLightData.beginAttenuation = m_SpotLightMaterial->findParameter(HEFS::strlight_beginAttenuation);
        m_SpotLightData.color = m_SpotLightMaterial->findParameter(HEFS::strlight_color);
        m_SpotLightData.endAttenuation = m_SpotLightMaterial->findParameter(HEFS::strlight_endAttenuation);
        m_SpotLightData.cosCutOff = m_SpotLightMaterial->findParameter(HEFS::strlight_cosCutoff);
        m_SpotLightData.colorIllMap = m_SpotLightMaterial->findParameter(HEFS::strcolorIllMap);
        m_SpotLightData.normalDepthMap = m_SpotLightMaterial->findParameter(HEFS::strnormalDepthMap);
        if (settings.lightingSettings.enableSpecular)
            m_SpotLightData.sgMap = m_SpotLightMaterial->findParameter(HEFS::strsgMap);
        m_SpotLightData.wvp = m_SpotLightMaterial->findParameter(HEFS::strmtxWVP);
    }

    // Shadow SpotLight
    {
        // Load material
        {
            const Material* const shadowSpotLightMaterial(CONTENT->loadMaterial("engine/deferred/spotLightShadow.hm"));
            m_ShadowSpotLightMaterial = shadowSpotLightMaterial->createMaterialInstance(eShaderRenderType_Normal);
            shadowSpotLightMaterial->release();
        }
        m_ShadowSpotLightMaterial->calculateMaterialLayout(m_Quad->getVertexLayout());

        m_ShadowSpotLightData.position = m_ShadowSpotLightMaterial->findParameter(HEFS::strlight_position);
        m_ShadowSpotLightData.multiplier = m_ShadowSpotLightMaterial->findParameter(HEFS::strlight_multiplier);
        m_ShadowSpotLightData.direction = m_ShadowSpotLightMaterial->findParameter(HEFS::strlight_direction);
        m_ShadowSpotLightData.beginAttenuation = m_ShadowSpotLightMaterial->findParameter(HEFS::strlight_beginAttenuation);
        m_ShadowSpotLightData.color = m_ShadowSpotLightMaterial->findParameter(HEFS::strlight_color);
        m_ShadowSpotLightData.endAttenuation = m_ShadowSpotLightMaterial->findParameter(HEFS::strlight_endAttenuation);
        m_ShadowSpotLightData.cosCutOff = m_ShadowSpotLightMaterial->findParameter(HEFS::strlight_cosCutoff);
        m_ShadowSpotLightData.colorIllMap = m_ShadowSpotLightMaterial->findParameter(HEFS::strcolorIllMap);
        m_ShadowSpotLightData.normalDepthMap = m_ShadowSpotLightMaterial->findParameter(HEFS::strnormalDepthMap);
        if (settings.lightingSettings.enableSpecular)
            m_ShadowSpotLightData.sgMap = m_ShadowSpotLightMaterial->findParameter(HEFS::strsgMap);
        m_ShadowSpotLightData.wvp = m_ShadowSpotLightMaterial->findParameter(HEFS::strmtxWVP);
        m_ShadowSpotLightData.shadowMap = m_ShadowSpotLightMaterial->findParameter(HEFS::strshadowMap);
        m_ShadowSpotLightData.shadowMatrix = m_ShadowSpotLightMaterial->findParameter(HEFS::strshadowMatrix);
        m_ShadowSpotLightData.shadowInvSize = m_ShadowSpotLightMaterial->findParameter(HEFS::strshadowMapInvSize);
    }

    // AmbDirLight
    {
        // Load material
        {
            const Material* const ambDirLightMaterial(CONTENT->loadMaterial("engine/deferred/ambDirLight.hm"));
            m_AmbDirIllMaterial = ambDirLightMaterial->createMaterialInstance(eShaderRenderType_Normal);
            ambDirLightMaterial->release();
        }
        m_AmbDirIllMaterial->calculateMaterialLayout(m_Quad->getVertexLayout());

        m_AmbDirIllLightData.ambColor = m_AmbDirIllMaterial->findParameter(HEFS::strambLight_color);
        m_AmbDirIllLightData.dirColor = m_AmbDirIllMaterial->findParameter(HEFS::strdirLight_color);
        m_AmbDirIllLightData.dirDirection = m_AmbDirIllMaterial->findParameter(HEFS::strdirLight_direction);

        m_AmbDirIllLightData.colorIllMap  = m_AmbDirIllMaterial->findParameter(HEFS::strcolorIllMap);
        m_AmbDirIllLightData.normalDepthMap  = m_AmbDirIllMaterial->findParameter(HEFS::strnormalDepthMap);
        if (settings.lightingSettings.enableSpecular)
            m_AmbDirIllLightData.sgMap  = m_AmbDirIllMaterial->findParameter(HEFS::strsgMap);
    }
}

void Deferred3DRenderer::onViewResized()
{
    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    uint32 width(m_View->getViewport().width); 
    uint32 height(m_View->getViewport().height);

    //Collection Textures - just SGI and color others are shared
    // Color
    m_ColorIllTexture->setData(width, height, 0, 
        TextureBufferLayout_BGRA, TextureBufferType_Byte, 0 );

    // SG
    m_SGTexture->setData(width, height, 0, 
        TextureBufferLayout_BGRA, TextureBufferType_Byte, 0 );

    m_CollectionRenderTarget->resizeDepthBuffer(width, height);

}

void Deferred3DRenderer::render()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    HE_ASSERT(m_Scene != nullptr, "Scene is nullptr, assign a scene first!");
    const ICamera* const camera(m_View->getCamera());
    if (camera != nullptr)
    {
        //////////////////////////////////////////////////////////////////////////
        ///                             BEGIN                                  ///
        //////////////////////////////////////////////////////////////////////////
        m_CollectionRenderTarget->prepareForRendering();
        GL::heSetViewport(RectI(0, 0, m_View->getViewport().width, m_View->getViewport().height));
        m_CollectionRenderTarget->clear(he::Color(0.0f, 1, 0, 0));
        GRAPHICS->getShaderUniformBufferManager()->updateSceneBuffer(m_Scene);
        GRAPHICS->getShaderUniformBufferManager()->bind();


        //////////////////////////////////////////////////////////////////////////
        ///                             DRAW                                   ///
        //////////////////////////////////////////////////////////////////////////
        DrawContext context;
        context.m_Camera = camera;
        m_Scene->getDrawList().draw(DrawListContainer::BlendFilter_Opac, camera, [&context](Drawable* d)
        {
            ModelMesh* const mesh(d->getModelMesh());
            context.m_VBO = mesh->getVBO();
            context.m_IBO = mesh->getIBO();
            context.m_WorldMatrix = d->getWorldMatrix();
            d->getMaterial()->apply(context);
            d->getModelMesh()->draw();
        });


        //////////////////////////////////////////////////////////////////////////
        ///                             POST                                   ///
        //////////////////////////////////////////////////////////////////////////
         m_OutputRenderTarget->prepareForRendering(1);
                 
         postPointLights();           
         postSpotLights();
         postAmbDirIllLight();
    }
}
void Deferred3DRenderer::draw2D(gui::Canvas2D* canvas)
{
    //if (m_ShowDebugTextures)
    {
        canvas->blitImage(m_ColorIllTexture, vec2(12 * 1 + 256 * 0, 12), false, vec2(256, 144));
        canvas->blitImage(m_SGTexture,       vec2(12 * 2 + 256 * 1, 12), false, vec2(256, 144));
        canvas->blitImage(m_NormalDepthTexture,   vec2(12 * 3 + 256 * 2, 12), false, vec2(256, 144));
        canvas->blitImage(m_OutputRenderTarget->getTextureTarget(0), vec2(12 * 1 + 256 * 0, 12 * 2 + 1 * 144), false, vec2(256, 144));
    }
}

void Deferred3DRenderer::postAmbDirIllLight()
{
    LightManager* lightManager(m_Scene->getLightManager());

    const AmbientLight* ambLight(lightManager->getAmbientLight());
    const DirectionalLight* dirLight(lightManager->getDirectionalLight());

    m_AmbDirIllMaterial->getParameter(m_AmbDirIllLightData.ambColor).setFloat4(vec4(ambLight->color, ambLight->multiplier));
    m_AmbDirIllMaterial->getParameter(m_AmbDirIllLightData.dirColor).setFloat4(vec4(dirLight->getColor(), dirLight->getMultiplier()));
    m_AmbDirIllMaterial->getParameter(m_AmbDirIllLightData.dirDirection).setFloat3(normalize((m_View->getCamera()->getView() * vec4(dirLight->getDirection(), 0.0f)).xyz()));
  
    m_AmbDirIllMaterial->getParameter(m_AmbDirIllLightData.colorIllMap).setTexture2D(m_ColorIllTexture);
    if (m_SpecularEnabled)
        m_AmbDirIllMaterial->getParameter(m_AmbDirIllLightData.sgMap).setTexture2D(m_SGTexture);
    m_AmbDirIllMaterial->getParameter(m_AmbDirIllLightData.normalDepthMap).setTexture2D(m_NormalDepthTexture);

    DrawContext context;
    context.m_VBO = m_Quad->getVBO();
    context.m_IBO = m_Quad->getIBO();
    m_AmbDirIllMaterial->apply(context);
    m_Quad->draw();
}
void Deferred3DRenderer::postPointLights()
{
    LightManager* lightManager(m_Scene->getLightManager());
    const he::ObjectList<ObjectHandle>& lights(lightManager->getPointLights());

    const LightFactory* lightFactory(LightFactory::getInstance());

    if (lights.size() == 0 || m_PointLightVolume->isLoaded() == false)
        return;

    m_PointLightMaterial->getParameter(m_PointLightData.colorIllMap).setTexture2D(m_ColorIllTexture);
    if (m_SpecularEnabled)
        m_PointLightMaterial->getParameter(m_PointLightData.sgMap).setTexture2D(m_SGTexture);
    m_PointLightMaterial->getParameter(m_PointLightData.normalDepthMap).setTexture2D(m_NormalDepthTexture);

    const ICamera& camera(*m_View->getCamera());
    vec3 position;
    lights.forEach([&](const ObjectHandle& lightHandle)
    {
        PointLight* light(lightFactory->getPointLight(lightHandle));

        light->getWorldMatrix().getTranslationComponent(position);
        Sphere sphere(position, light->getScaledEndAttenuation());
        if (camera.intersect(sphere) != IntersectResult_Outside)  
        {
            if (lengthSqr(position - camera.getPosition()) < sqr(light->getScaledEndAttenuation()) + 2*light->getScaledEndAttenuation()*camera.getNearClip() + sqr(camera.getNearClip()))
            {
                m_PointLightMaterial->setCullFrontFace(true);
                m_PointLightMaterial->setDepthFunc(DepthFunc_GeaterOrEqual);
            }
            else
            {
                m_PointLightMaterial->setCullFrontFace(false);
                m_PointLightMaterial->setDepthFunc(DepthFunc_LessOrEqual);
            }

            m_PointLightMaterial->getParameter(m_PointLightData.position).setFloat3(camera.getView() * position);
            m_PointLightMaterial->getParameter(m_PointLightData.multiplier).setFloat(light->getMultiplier());
            m_PointLightMaterial->getParameter(m_PointLightData.color).setFloat3(light->getColor());
            m_PointLightMaterial->getParameter(m_PointLightData.beginAttenuation).setFloat(light->getScaledBeginAttenuation());
            m_PointLightMaterial->getParameter(m_PointLightData.endAttenuation).setFloat(light->getScaledEndAttenuation());
            m_PointLightMaterial->getParameter(m_PointLightData.wvp).setFloat44(camera.getViewProjection() * light->getWorldMatrix());

            DrawContext context;
            context.m_VBO = m_PointLightVolume->getVBO();
            context.m_IBO = m_PointLightVolume->getIBO();
            m_PointLightMaterial->apply(context);
            m_PointLightVolume->draw();
        }
    });
}

void Deferred3DRenderer::postSpotLights()
{
    LightManager* pLightManager(m_Scene->getLightManager());
    const he::ObjectList<ObjectHandle>& lights(pLightManager->getSpotLights());

    const LightFactory* lightFactory(LightFactory::getInstance());

    if (lights.size() == 0 || m_SpotLightVolume->isLoaded() == false)
        return;

    const ICamera& camera(*m_View->getCamera());
    vec3 position;

    bool shadowLights(false);
    for (size_t i(0); i < 2; ++i)
    {
        MaterialInstance* material(nullptr);
        if (i == 0)
        {
            material = m_SpotLightMaterial;
            m_SpotLightMaterial->getParameter(m_SpotLightData.colorIllMap).setTexture2D(m_ColorIllTexture);
            if (m_SpecularEnabled)
                m_SpotLightMaterial->getParameter(m_SpotLightData.sgMap).setTexture2D(m_SGTexture);
            m_SpotLightMaterial->getParameter(m_SpotLightData.normalDepthMap).setTexture2D(m_NormalDepthTexture);
        }
        else
        {
            material = m_ShadowSpotLightMaterial;
            m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.colorIllMap).setTexture2D(m_ColorIllTexture);
            if (m_SpecularEnabled)
                m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.sgMap).setTexture2D(m_SGTexture);
            m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.normalDepthMap).setTexture2D(m_NormalDepthTexture);
        }

        lights.forEach([&](const ObjectHandle handle)
        {
            SpotLight* light(lightFactory->getSpotLight(handle));

            bool isShadowed(false);
            if (light->getShadowResolution() != ShadowResolution_None)
            {
                shadowLights = true;
                isShadowed = true;
                if (i == 0) return; // Wait for next pass
            }
            else if (i != 0)
            {
                return; // Already rendered
            }

            light->getWorldMatrix().getTranslationComponent(position);
            Sphere sphere(position, light->getScaledEndAttenuation());      /// BAAAD

            bool inView(camera.intersect(sphere) != IntersectResult_Outside);
            light->setVisibleLastFrame(inView);
            if (inView)  
            {
                if (lengthSqr(position - camera.getPosition()) < sqr(light->getScaledEndAttenuation()) + 2*light->getScaledEndAttenuation()*camera.getNearClip() + sqr(camera.getNearClip()))
                {
                    material->setCullFrontFace(true);
                    material->setDepthFunc(DepthFunc_GeaterOrEqual);
                }
                else
                {
                    material->setCullFrontFace(false);
                    material->setDepthFunc(DepthFunc_LessOrEqual);
                }

                if (isShadowed)
                {
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.wvp).setFloat44(camera.getViewProjection() * light->getWorldMatrix());
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.position).setFloat3(camera.getView() * position);
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.multiplier).setFloat(light->getMultiplier());
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.direction).setFloat3(normalize((camera.getView() * vec4(light->getWorldDirection(), 0)).xyz()));
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.beginAttenuation).setFloat(light->getScaledBeginAttenuation());
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.color).setFloat3(light->getColor());
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.endAttenuation).setFloat(light->getScaledEndAttenuation());
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.cosCutOff).setFloat(light->getCosCutoff());

                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.shadowMatrix).setFloat44(light->getShadowCamera().getViewProjection() * camera.getView().inverse());
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.shadowMap).setTexture2D(light->getShadowMap());
                    m_ShadowSpotLightMaterial->getParameter(m_ShadowSpotLightData.shadowInvSize).setFloat2(vec2(1.0f / gfx::GraphicsEngine::getShadowMapSize(light->getShadowResolution())));
                }
                else
                {
                    m_SpotLightMaterial->getParameter(m_SpotLightData.wvp).setFloat44(camera.getViewProjection() * light->getWorldMatrix());
                    m_SpotLightMaterial->getParameter(m_SpotLightData.position).setFloat3(camera.getView() * position);
                    m_SpotLightMaterial->getParameter(m_SpotLightData.multiplier).setFloat(light->getMultiplier());
                    m_SpotLightMaterial->getParameter(m_SpotLightData.direction).setFloat3(normalize((camera.getView() * vec4(light->getWorldDirection(), 0)).xyz()));
                    m_SpotLightMaterial->getParameter(m_SpotLightData.beginAttenuation).setFloat(light->getScaledBeginAttenuation());
                    m_SpotLightMaterial->getParameter(m_SpotLightData.color).setFloat3(light->getColor());
                    m_SpotLightMaterial->getParameter(m_SpotLightData.endAttenuation).setFloat(light->getScaledEndAttenuation());
                    m_SpotLightMaterial->getParameter(m_SpotLightData.cosCutOff).setFloat(light->getCosCutoff());
                }

                DrawContext context;
                context.m_VBO = m_SpotLightVolume->getVBO();
                context.m_IBO = m_SpotLightVolume->getIBO();
                material->apply(context);
                m_SpotLightVolume->draw();
            }
        });
        if (shadowLights == false)
            break;
    }
}

} } //end namespace
