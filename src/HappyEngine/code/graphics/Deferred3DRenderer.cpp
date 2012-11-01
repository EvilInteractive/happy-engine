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
//Revising RenderEngine: 17/12/2011
#include "HappyPCH.h" 

#include "Deferred3DRenderer.h"
#include "Renderer2D.h"
#include "BufferLayout.h"
#include "Vertex.h"

#include "LightManager.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

#include "GraphicsEngine.h"
#include "ContentManager.h"

#include "CameraManager.h"
#include "CameraPerspective.h"
#include "CameraBound.h"
#include "LightFactory.h"

#include "RenderTarget.h"
#include "Scene.h"
#include "View.h"
#include "Window.h"

#include "Texture2D.h"
#include "ModelMesh.h"
#include "Canvas2D.h"

namespace he {
namespace gfx {

BufferLayout Deferred3DRenderer::s_VertexLayoutFullscreenQuad = BufferLayout();

Deferred3DRenderer::Deferred3DRenderer(): 
            m_pQuad(nullptr), 
            m_ShowDebugTextures(false),
            m_PointLightShader(nullptr),
            m_SpotLightShader(nullptr),
            m_ShadowSpotLightShader(nullptr),
            m_AmbDirIllShader(nullptr),
            m_CollectionRenderTarget(nullptr),
            m_OutputRenderTarget(nullptr),
            m_View(nullptr),
            m_NormalDepthTexture(nullptr)
{
    ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
    m_ColorIllTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_ColorIllTexture->setName("Deferred3DRenderer::m_pColorIllTexture");
    m_ColorIllTexture->init(TextureWrapType_Clamp, TextureFilterType_Nearest, 
        TextureFormat_RGBA8, false);

    handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_SGTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_SGTexture->setName("Deferred3DRenderer::m_pSGTexture");
    m_SGTexture->init(TextureWrapType_Clamp, TextureFilterType_Nearest, 
        TextureFormat_RGBA8, false);

}
void Deferred3DRenderer::init( View3D* view, const RenderTarget* target )
{
    HE_ASSERT(m_View == nullptr, "Deferred3DRenderer inited twice!");
    //CONSOLE->registerVar(&m_ShowDebugTextures, "debugDefTex");
    
    m_View = view;
    m_CollectionRenderTarget = NEW RenderTarget(m_View->getWindow()->getContext());
    m_OutputRenderTarget = target;

    m_NormalDepthTexture = target->getTextureTarget(1);

    eventCallback0<void> settingsChangedHandler(boost::bind(&Deferred3DRenderer::onSettingChanged, this));
    m_View->SettingsChanged += settingsChangedHandler;
    eventCallback0<void> viewportSizeChangedHandler(boost::bind(&Deferred3DRenderer::onViewResized, this));
    m_View->ViewportSizeChanged += viewportSizeChangedHandler;

    m_Settings = m_View->getSettings().lightingSettings;
    compileShaders();
    onViewResized();

    //FBO Collection
    m_CollectionRenderTarget->removeAllTargets();
    m_CollectionRenderTarget->addTextureTarget(m_ColorIllTexture);
    m_CollectionRenderTarget->addTextureTarget(m_SGTexture);
    m_CollectionRenderTarget->addTextureTarget(m_NormalDepthTexture);
    m_CollectionRenderTarget->setDepthTarget(target->getDepthBuffer());
    m_CollectionRenderTarget->init();
    


    //m_View->get2DRenderer()->attachToRender(this);

    m_pQuad = CONTENT->getFullscreenQuad();
}


Deferred3DRenderer::~Deferred3DRenderer()
{
    //m_View->get2DRenderer()->detachFromRender(this);

    m_ColorIllTexture->release();
    m_SGTexture->release();

    delete m_CollectionRenderTarget;
    
    m_pQuad->release();

    if (m_PointLightShader != nullptr)
        m_PointLightShader->release();    
    if (m_SpotLightShader != nullptr)
        m_SpotLightShader->release();    
    if (m_ShadowSpotLightShader != nullptr)
        m_ShadowSpotLightShader->release();
    if (m_AmbDirIllShader != nullptr)
        m_AmbDirIllShader->release();
}
void Deferred3DRenderer::compileShaders()
{
    //////////////////////////////////////////////////////////////////////////
    ///                                 CLEAN                              ///
    //////////////////////////////////////////////////////////////////////////
    if (m_PointLightShader != nullptr)
        m_PointLightShader->release();    
    if (m_SpotLightShader != nullptr)
        m_SpotLightShader->release(); 
    if (m_ShadowSpotLightShader != nullptr)
        m_ShadowSpotLightShader->release();
    if (m_AmbDirIllShader != nullptr)
        m_AmbDirIllShader->release();

    ResourceFactory<Shader>* factory(ResourceFactory<Shader>::getInstance());
    m_PointLightShader =  factory->get(factory->create());
    m_SpotLightShader  =  factory->get(factory->create());
    m_ShadowSpotLightShader  =  factory->get(factory->create());
    m_AmbDirIllShader  =  factory->get(factory->create());

    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    s_VertexLayoutFullscreenQuad.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));

    //////////////////////////////////////////////////////////////////////////
    ///                                 Load                               ///
    //////////////////////////////////////////////////////////////////////////
    const std::string& folder(CONTENT->getShaderFolderPath().str());
    std::set<std::string> shaderDefines;
    if (m_Settings.enableSpecular)
        shaderDefines.insert("SPECULAR");
    // Shadowless
    m_PointLightShader->initFromFile(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostPLShader.frag", shaderLayout, shaderDefines);
    m_SpotLightShader->initFromFile(folder  + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostSLShader.frag", shaderLayout, shaderDefines);
    m_AmbDirIllShader->initFromFile(folder  + "shared/postShaderQuad.vert", folder + "deferred/post/deferredPostAmbientIllShader.frag", shaderLayout, shaderDefines);
    // Shadow
    //if (m_Settings.enableShadows)
        shaderDefines.insert("SHADOWS");
    m_ShadowSpotLightShader->initFromFile(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostSLShader.frag", shaderLayout, shaderDefines);

    //SharedBuffer
    m_SharedShaderData.pSharedBuffer = m_PointLightShader->setBuffer(m_PointLightShader->getBufferId("SharedBuffer"));
    m_SpotLightShader->setBuffer(m_SpotLightShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);
    m_ShadowSpotLightShader->setBuffer(m_ShadowSpotLightShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);
    m_AmbDirIllShader->setBuffer(m_AmbDirIllShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);

    m_SharedShaderData.pSharedBuffer->getShaderVar("projParams", m_SharedShaderData.projParams);

    //----PL----------------------------------------------------------------------
    m_PointLightData.position = m_PointLightShader->getShaderVarId("light.position");
    m_PointLightData.multiplier = m_PointLightShader->getShaderVarId("light.multiplier");
    m_PointLightData.color = m_PointLightShader->getShaderVarId("light.color");
    m_PointLightData.beginAttenuation = m_PointLightShader->getShaderVarId("light.beginAttenuation");
    m_PointLightData.endAttenuation = m_PointLightShader->getShaderVarId("light.endAttenuation");
    m_PointLightData.colorIllMap = m_PointLightShader->getShaderSamplerId("colorIllMap");
    m_PointLightData.normalDepthMap = m_PointLightShader->getShaderSamplerId("normalDepthMap");
    if (m_Settings.enableSpecular)
        m_PointLightData.sgMap = m_PointLightShader->getShaderSamplerId("sgMap");
    m_PointLightData.wvp = m_PointLightShader->getShaderVarId("mtxWVP");
    //----SL----------------------------------------------------------------------
        // No Shadow
        m_SpotLightData.position = m_SpotLightShader->getShaderVarId("light.position");
        m_SpotLightData.multiplier = m_SpotLightShader->getShaderVarId("light.multiplier");
        m_SpotLightData.direction = m_SpotLightShader->getShaderVarId("light.direction");
        m_SpotLightData.beginAttenuation = m_SpotLightShader->getShaderVarId("light.beginAttenuation");
        m_SpotLightData.color = m_SpotLightShader->getShaderVarId("light.color");
        m_SpotLightData.endAttenuation = m_SpotLightShader->getShaderVarId("light.endAttenuation");
        m_SpotLightData.cosCutOff = m_SpotLightShader->getShaderVarId("light.cosCutoff");
        m_SpotLightData.colorIllMap = m_SpotLightShader->getShaderSamplerId("colorIllMap");
        m_SpotLightData.normalDepthMap = m_SpotLightShader->getShaderSamplerId("normalDepthMap");
        if (m_Settings.enableSpecular)
            m_SpotLightData.sgMap = m_SpotLightShader->getShaderSamplerId("sgMap");
        m_SpotLightData.wvp = m_SpotLightShader->getShaderVarId("mtxWVP");

        // Shadow
        m_ShadowSpotLightData.position = m_ShadowSpotLightShader->getShaderVarId("light.position");
        m_ShadowSpotLightData.multiplier = m_ShadowSpotLightShader->getShaderVarId("light.multiplier");
        m_ShadowSpotLightData.direction = m_ShadowSpotLightShader->getShaderVarId("light.direction");
        m_ShadowSpotLightData.beginAttenuation = m_ShadowSpotLightShader->getShaderVarId("light.beginAttenuation");
        m_ShadowSpotLightData.color = m_ShadowSpotLightShader->getShaderVarId("light.color");
        m_ShadowSpotLightData.endAttenuation = m_ShadowSpotLightShader->getShaderVarId("light.endAttenuation");
        m_ShadowSpotLightData.cosCutOff = m_ShadowSpotLightShader->getShaderVarId("light.cosCutoff");
        m_ShadowSpotLightData.colorIllMap = m_ShadowSpotLightShader->getShaderSamplerId("colorIllMap");
        m_ShadowSpotLightData.normalDepthMap = m_ShadowSpotLightShader->getShaderSamplerId("normalDepthMap");
        if (m_Settings.enableSpecular)
            m_ShadowSpotLightData.sgMap = m_ShadowSpotLightShader->getShaderSamplerId("sgMap");
        m_ShadowSpotLightData.wvp = m_ShadowSpotLightShader->getShaderVarId("mtxWVP");
        m_ShadowSpotLightData.shadowMap = m_ShadowSpotLightShader->getShaderSamplerId("shadowMap");
        m_ShadowSpotLightData.shadowMatrix = m_ShadowSpotLightShader->getShaderVarId("shadowMatrix");

    //----AL----------------------------------------------------------------------   
    m_AmbDirIllLightData.ambColor = m_AmbDirIllShader->getShaderVarId("ambLight.color");
    m_AmbDirIllLightData.dirColor = m_AmbDirIllShader->getShaderVarId("dirLight.color");
    m_AmbDirIllLightData.dirDirection = m_AmbDirIllShader->getShaderVarId("dirLight.direction");
    m_AmbDirIllLightData.dirPosition = m_AmbDirIllShader->getShaderVarId("dirLight.position");
    m_AmbDirIllLightData.dirNearFar = m_AmbDirIllShader->getShaderVarId("dirLight.nearFar");

    m_AmbDirIllLightData.colorIllMap  = m_AmbDirIllShader->getShaderSamplerId("colorIllMap");
    m_AmbDirIllLightData.normalDepthMap  = m_AmbDirIllShader->getShaderSamplerId("normalDepthMap");
    if (m_Settings.enableSpecular)
        m_AmbDirIllLightData.sgMap  = m_AmbDirIllShader->getShaderSamplerId("sgMap");
    
    if (m_Settings.enableShadows)
    {
        m_AmbDirIllLightData.shadowMap0 = m_AmbDirIllShader->getShaderSamplerId("shadowMap0");
        m_AmbDirIllLightData.shadowMap1 = m_AmbDirIllShader->getShaderSamplerId("shadowMap1");
        m_AmbDirIllLightData.shadowMap2 = m_AmbDirIllShader->getShaderSamplerId("shadowMap2");
        m_AmbDirIllLightData.shadowMap3 = m_AmbDirIllShader->getShaderSamplerId("shadowMap3");
    }
}

void Deferred3DRenderer::onSettingChanged()
{
    if (m_View->getSettings().lightingSettings != m_Settings)
    {
        m_Settings = m_View->getSettings().lightingSettings;
        compileShaders();
    }
}

void Deferred3DRenderer::onViewResized()
{
    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    uint width(m_View->getViewport().width); 
    uint height(m_View->getViewport().height);


    //Collection Textures - just SGI and color others are shared
    // Color
    m_ColorIllTexture->setData(width, height, 0, 
        TextureBufferLayout_BGRA, TextureBufferType_Byte, 0 );

    // SG
    m_SGTexture->setData(width, height, 0, 
        TextureBufferLayout_BGRA, TextureBufferType_Byte, 0 );

}

void Deferred3DRenderer::draw()
{
    const Scene* scene(m_View->getScene());

    //////////////////////////////////////////////////////////////////////////
    ///                             BEGIN                                  ///
    //////////////////////////////////////////////////////////////////////////
    m_CollectionRenderTarget->prepareForRendering();
    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
    GL::heBlendFunc(BlendFunc_One, BlendFunc_Zero);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heBlendEnabled(false);
    m_CollectionRenderTarget->clear(he::Color(0.0f, 0, 0, 0));


    //////////////////////////////////////////////////////////////////////////
    ///                             DRAW                                   ///
    //////////////////////////////////////////////////////////////////////////
    const CameraPerspective* camera(m_View->getCamera());
    scene->getDrawList().draw(DrawListContainer::BlendFilter_Opac, camera, [&camera](IDrawable* d)
    //scene->getDrawList().drawAndCreateDebugMesh(DrawListContainer::BlendFilter_Opac, camera, [&camera](IDrawable* d)
    {
        d->applyMaterial(camera);
        d->draw();
    }/*, m_View->getDebugVertices(), m_View->getDebugIndices()*/);


    //////////////////////////////////////////////////////////////////////////
    ///                             POST                                   ///
    //////////////////////////////////////////////////////////////////////////
    m_OutputRenderTarget->prepareForRendering(1);

    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_One, BlendFunc_One);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    m_SharedShaderData.projParams.set(vec4(
        camera->getProjection()(0, 0),
        camera->getProjection()(1, 1),
        camera->getNearClip(),
        camera->getFarClip()));
    m_SharedShaderData.pSharedBuffer->setShaderVar(m_SharedShaderData.projParams);

    m_PointLightShader->bind();
    postPointLights(scene);           
 
    postSpotLights(scene);

    m_AmbDirIllShader->bind();
    postAmbDirIllLight(scene);

    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
}
void Deferred3DRenderer::draw2D(Canvas2D* canvas)
{
    /*
    if (m_ShowDebugTextures)
    {*/
        canvas->getRenderer2D()->drawTexture2DToScreen(m_ColorIllTexture, vec2(12 * 1 + 256 * 0, 12), false, vec2(256, 144));
        canvas->getRenderer2D()->drawTexture2DToScreen(m_SGTexture,       vec2(12 * 2 + 256 * 1, 12), false, vec2(256, 144));
        canvas->getRenderer2D()->drawTexture2DToScreen(m_NormalDepthTexture,   vec2(12 * 3 + 256 * 2, 12), false, vec2(256, 144));
        //canvas->getRenderer2D()->drawTexture2DToScreen(m_CollectionRenderTarget->getDepthTarget(),      vec2(12 * 4 + 256 * 3, 12), false, vec2(256, 144));

        canvas->getRenderer2D()->drawTexture2DToScreen(m_OutputRenderTarget->getTextureTarget(0), vec2(12 * 1 + 256 * 0, 12 * 2 + 1 * 144), false, vec2(256, 144));
    //}
}

void Deferred3DRenderer::postAmbDirIllLight(const Scene* scene)
{
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    LightManager* lightManager(scene->getLightManager());

    const AmbientLight* ambLight(lightManager->getAmbientLight());
    const DirectionalLight* dirLight(lightManager->getDirectionalLight());

    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.ambColor, vec4(ambLight->color, ambLight->multiplier));
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.dirColor, vec4(dirLight->getColor(), dirLight->getMultiplier()));
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.dirDirection, normalize((m_View->getCamera()->getView() * vec4(dirLight->getDirection(), 0.0f)).xyz()));
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.dirPosition, (m_View->getCamera()->getView() * vec4(dirLight->getShadowPosition(), 1.0f)).xyz());
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.dirNearFar, dirLight->getShadowNearFar());

    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.colorIllMap, m_ColorIllTexture);
    if (m_Settings.enableSpecular)
        m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.sgMap,   m_SGTexture);
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.normalDepthMap,   m_NormalDepthTexture);

    GL::heBindVao(m_pQuad->getVertexArraysID());       
    glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
}
void Deferred3DRenderer::postPointLights(const Scene* scene)
{
    LightManager* lightManager(scene->getLightManager());
    const std::vector<ObjectHandle>& lights(lightManager->getPointLights());

    const LightFactory* lightFactory(LightFactory::getInstance());

    if (lights.size() == 0 || lightFactory->getPointLight(lights.back())->getLightVolume()->isLoaded() == false)
        return;

    m_PointLightShader->setShaderVar(m_PointLightData.colorIllMap, m_ColorIllTexture);
    if (m_Settings.enableSpecular)
        m_PointLightShader->setShaderVar(m_PointLightData.sgMap,   m_SGTexture);
    m_PointLightShader->setShaderVar(m_PointLightData.normalDepthMap,   m_NormalDepthTexture);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);

    const CameraPerspective& camera(*m_View->getCamera());
    vec3 position;
    std::for_each(lights.cbegin(), lights.cend(), [&](const ObjectHandle& lightHandle)
    {
        PointLight* light(lightFactory->getPointLight(lightHandle));

        light->getWorldMatrix().getTranslationComponent(position);
        Sphere sphere(position, light->getScaledEndAttenuation());
        if (camera.intersect(sphere) != IntersectResult_Outside)  
        {
            if (lengthSqr(position - camera.getPosition()) < sqr(light->getScaledEndAttenuation()) + 2*light->getScaledEndAttenuation()*camera.getNearClip() + sqr(camera.getNearClip()))
            {
                GL::heSetCullFace(true);
                GL::heSetDepthFunc(DepthFunc_GeaterOrEqual);
            }
            else
            {
                GL::heSetCullFace(false);
                GL::heSetDepthFunc(DepthFunc_LessOrEqual);
            }

            m_PointLightShader->setShaderVar(m_PointLightData.position, camera.getView() * position);
            m_PointLightShader->setShaderVar(m_PointLightData.multiplier, light->getMultiplier());
            m_PointLightShader->setShaderVar(m_PointLightData.color, light->getColor());
            m_PointLightShader->setShaderVar(m_PointLightData.beginAttenuation, light->getScaledBeginAttenuation());
            m_PointLightShader->setShaderVar(m_PointLightData.endAttenuation, light->getScaledEndAttenuation());

            m_PointLightShader->setShaderVar(m_PointLightData.wvp, camera.getViewProjection() * light->getWorldMatrix());

            GL::heBindVao(light->getLightVolume()->getVertexArraysID());
            glDrawElements(GL_TRIANGLES, light->getLightVolume()->getNumIndices(), light->getLightVolume()->getIndexType(), 0);
        }
    });
    GL::heSetCullFace(false);
}

void Deferred3DRenderer::postSpotLights(const Scene* scene)
{
    LightManager* pLightManager(scene->getLightManager());
    const std::vector<ObjectHandle>& lights(pLightManager->getSpotLights());

    const LightFactory* lightFactory(LightFactory::getInstance());

    if (lights.size() == 0 || lightFactory->getSpotLight(lights.back())->getLightVolume()->isLoaded() == false)
        return;

    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);
    const CameraPerspective& camera(*m_View->getCamera());
    vec3 position;

    bool shadowLights(false);
    for (size_t i(0); i < 2; ++i)
    {
        if (i == 0)
        {
            m_SpotLightShader->bind();
            m_SpotLightShader->setShaderVar(m_SpotLightData.colorIllMap, m_ColorIllTexture);
            if (m_Settings.enableSpecular)
                m_SpotLightShader->setShaderVar(m_SpotLightData.sgMap,   m_SGTexture);
            m_SpotLightShader->setShaderVar(m_SpotLightData.normalDepthMap,   m_NormalDepthTexture);
        }
        else
        {
            m_ShadowSpotLightShader->bind();
            m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.colorIllMap, m_ColorIllTexture);
            if (m_Settings.enableSpecular)
                m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.sgMap,   m_SGTexture);
            m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.normalDepthMap,   m_NormalDepthTexture);
        }

        std::vector<ObjectHandle>::const_iterator it(lights.cbegin());
        for (; it != lights.cend(); ++it)
        {
            SpotLight* light(lightFactory->getSpotLight(*it));

            bool isShadowed(false);
            if (light->getShadowResolution() != ShadowResolution_None)
            {
                shadowLights = true;
                isShadowed = true;
                if (i == 0) continue;
            }
            else if (i != 0)
            {
                continue;
            }

            light->getWorldMatrix().getTranslationComponent(position);
            Sphere sphere(position, light->getScaledEndAttenuation());      /// BAAAD

            bool inView(camera.intersect(sphere) != IntersectResult_Outside);
            light->setVisibleLastFrame(inView);
            if (inView)  
            {
                if (lengthSqr(position - camera.getPosition()) < sqr(light->getScaledEndAttenuation()) + 2*light->getScaledEndAttenuation()*camera.getNearClip() + sqr(camera.getNearClip()))
                {
                    GL::heSetCullFace(true);
                    GL::heSetDepthFunc(DepthFunc_GeaterOrEqual);
                }
                else
                {
                    GL::heSetCullFace(false);
                    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
                }

                if (isShadowed)
                {
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.wvp,              camera.getViewProjection() * light->getWorldMatrix());
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.position,         camera.getView() * position);
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.multiplier,       light->getMultiplier());
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.direction,        normalize((camera.getView() * vec4(light->getWorldDirection(), 0)).xyz()));
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.beginAttenuation, light->getScaledBeginAttenuation());
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.color,            light->getColor());
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.endAttenuation,   light->getScaledEndAttenuation());
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.cosCutOff,        light->getCosCutoff());

                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.shadowMatrix,     light->getShadowCamera().getViewProjection() * camera.getView().inverse());
                    m_ShadowSpotLightShader->setShaderVar(m_ShadowSpotLightData.shadowMap,        light->getShadowMap());
                }
                else
                {
                    m_SpotLightShader->setShaderVar(m_SpotLightData.wvp,              camera.getViewProjection() * light->getWorldMatrix());
                    m_SpotLightShader->setShaderVar(m_SpotLightData.position,         camera.getView() * position);
                    m_SpotLightShader->setShaderVar(m_SpotLightData.multiplier,       light->getMultiplier());
                    m_SpotLightShader->setShaderVar(m_SpotLightData.direction,        normalize((camera.getView() * vec4(light->getWorldDirection(), 0)).xyz()));
                    m_SpotLightShader->setShaderVar(m_SpotLightData.beginAttenuation, light->getScaledBeginAttenuation());
                    m_SpotLightShader->setShaderVar(m_SpotLightData.color,            light->getColor());
                    m_SpotLightShader->setShaderVar(m_SpotLightData.endAttenuation,   light->getScaledEndAttenuation());
                    m_SpotLightShader->setShaderVar(m_SpotLightData.cosCutOff,        light->getCosCutoff());
                }

                const gfx::ModelMesh* volume(light->getLightVolume());
                GL::heBindVao(volume->getVertexArraysID());
                glDrawElements(GL_TRIANGLES, volume->getNumIndices(), volume->getIndexType(), 0);
            }
        }
        if (shadowLights == false)
            break;
    }
    GL::heSetCullFace(false);
}

} } //end namespace
