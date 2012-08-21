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

namespace he {
namespace gfx {

BufferLayout Deferred3DRenderer::s_VertexLayoutFullscreenQuad = BufferLayout();

Deferred3DRenderer::Deferred3DRenderer(): 
            m_pQuad(nullptr), 
            m_ShowDebugTextures(true),
            m_PointLightShader(nullptr),
            m_SpotLightShader(nullptr),
            m_AmbDirIllShader(nullptr),
            m_CollectionRenderTarget(nullptr),
            m_OutputRenderTarget(nullptr),
            m_View(nullptr),
            m_pNormalTexture(nullptr)
{
    ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
    m_pColorIllTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_pColorIllTexture->setName("Deferred3DRenderer::m_pColorIllTexture");
    m_pColorIllTexture->init(Texture2D::WrapType_Clamp, Texture2D::FilterType_Nearest, 
        Texture2D::TextureFormat_RGBA8, false);

    handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_pSGTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_pSGTexture->setName("Deferred3DRenderer::m_pSGTexture");
    m_pSGTexture->init(Texture2D::WrapType_Clamp, Texture2D::FilterType_Nearest, 
        Texture2D::TextureFormat_RGBA8, false);

}
void Deferred3DRenderer::init( View* view, const RenderTarget* target )
{
    HE_ASSERT(m_View == nullptr, "Deferred3DRenderer inited twice!");
    //CONSOLE->registerVar(&m_ShowDebugTextures, "debugDefTex");
    
    m_View = view;
    m_CollectionRenderTarget = NEW RenderTarget(m_View->getWindow()->getContext());
    m_OutputRenderTarget = NEW RenderTarget(m_View->getWindow()->getContext());
    m_OutputRenderTarget->addTextureTarget(target->getTextureTarget(0));
    m_OutputRenderTarget->setDepthTarget(target->getDepthTarget());
    m_OutputRenderTarget->init();

    m_pNormalTexture = target->getTextureTarget(1);

    eventCallback0<void> settingsChangedHandler(boost::bind(&Deferred3DRenderer::onSettingChanged, this));
    m_View->SettingsChanged += settingsChangedHandler;
    eventCallback0<void> viewportSizeChangedHandler(boost::bind(&Deferred3DRenderer::onViewResized, this));
    m_View->ViewportSizeChanged += viewportSizeChangedHandler;

    m_Settings = m_View->getSettings().lightingSettings;
    compileShaders();
    onViewResized();

    //FBO Collection
    m_CollectionRenderTarget->removeAllTargets();
    m_CollectionRenderTarget->addTextureTarget(m_pColorIllTexture);
    m_CollectionRenderTarget->addTextureTarget(m_pNormalTexture);
    m_CollectionRenderTarget->addTextureTarget(m_pSGTexture);
    m_CollectionRenderTarget->setDepthTarget(target->getDepthTarget());
    m_CollectionRenderTarget->init();
    


    m_View->get2DRenderer()->attachToRender(this);

    m_pQuad = CONTENT->getFullscreenQuad();
}


Deferred3DRenderer::~Deferred3DRenderer()
{
    m_View->get2DRenderer()->detachFromRender(this);

    m_pColorIllTexture->release();
    m_pSGTexture->release();

    delete m_CollectionRenderTarget;
    delete m_OutputRenderTarget;
    
    m_pQuad->release();

    if (m_PointLightShader != nullptr)
        m_PointLightShader->release();    
    if (m_SpotLightShader != nullptr)
        m_SpotLightShader->release();
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
    if (m_AmbDirIllShader != nullptr)
        m_AmbDirIllShader->release();

    m_PointLightShader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());
    m_SpotLightShader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());
    m_AmbDirIllShader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());

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
    if (m_Settings.enableShadows)
        shaderDefines.insert("SHADOWS");
    m_PointLightShader->initFromFile(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostPLShader.frag", shaderLayout, shaderDefines);
    m_SpotLightShader->initFromFile(folder  + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostSLShader.frag", shaderLayout, shaderDefines);
    m_AmbDirIllShader->initFromFile(folder  + "shared/postShaderQuad.vert", folder + "deferred/post/deferredPostAmbientIllShader.frag", shaderLayout, shaderDefines);

    //SharedBuffer
    m_SharedShaderData.pSharedBuffer = m_PointLightShader->setBuffer(m_PointLightShader->getBufferId("SharedBuffer"));
    m_SpotLightShader->setBuffer(m_SpotLightShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);
    m_AmbDirIllShader->setBuffer(m_AmbDirIllShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);

    m_SharedShaderData.pSharedBuffer->getShaderVar("projParams", m_SharedShaderData.projParams);

    //LightBuffer
    m_PointLightData.pLightBuffer = m_PointLightShader->setBuffer(m_PointLightShader->getBufferId("LightBuffer"));
    m_SpotLightData.pLightBuffer = m_SpotLightShader->setBuffer(m_SpotLightShader->getBufferId("LightBuffer"));
    m_AmbDirIllLightData.pLightBuffer = m_AmbDirIllShader->setBuffer(m_AmbDirIllShader->getBufferId("LightBuffer"));
    if (m_Settings.enableShadows)
        m_AmbDirIllLightData.pPerFrameBuffer = m_AmbDirIllShader->setBuffer(m_AmbDirIllShader->getBufferId("PerFrameBuffer"));

    //----PL----------------------------------------------------------------------
    m_PointLightData.pLightBuffer->getShaderVar("light.position", m_PointLightData.position);
    m_PointLightData.pLightBuffer->getShaderVar("light.multiplier", m_PointLightData.multiplier);
    m_PointLightData.pLightBuffer->getShaderVar("light.color", m_PointLightData.color);
    m_PointLightData.pLightBuffer->getShaderVar("light.beginAttenuation", m_PointLightData.beginAttenuation);
    m_PointLightData.pLightBuffer->getShaderVar("light.endAttenuation", m_PointLightData.endAttenuation);
    m_PointLightData.colorIllMap = m_PointLightShader->getShaderSamplerId("colorIllMap");
    m_PointLightData.normalMap = m_PointLightShader->getShaderSamplerId("normalMap");
    if (m_Settings.enableSpecular)
        m_PointLightData.sgMap = m_PointLightShader->getShaderSamplerId("sgMap");
    m_PointLightData.depthMap = m_PointLightShader->getShaderSamplerId("depthMap");
    m_PointLightData.wvp = m_PointLightShader->getShaderVarId("mtxWVP");
    //----SL----------------------------------------------------------------------
    m_SpotLightData.pLightBuffer->getShaderVar("light.position", m_SpotLightData.position);
    m_SpotLightData.pLightBuffer->getShaderVar("light.multiplier", m_SpotLightData.multiplier);
    m_SpotLightData.pLightBuffer->getShaderVar("light.direction", m_SpotLightData.direction);
    m_SpotLightData.pLightBuffer->getShaderVar("light.beginAttenuation", m_SpotLightData.beginAttenuation);
    m_SpotLightData.pLightBuffer->getShaderVar("light.color", m_SpotLightData.color);
    m_SpotLightData.pLightBuffer->getShaderVar("light.endAttenuation", m_SpotLightData.endAttenuation);
    m_SpotLightData.pLightBuffer->getShaderVar("light.cosCutoff", m_SpotLightData.cosCutOff);
    m_SpotLightData.colorIllMap = m_SpotLightShader->getShaderSamplerId("colorIllMap");
    m_SpotLightData.normalMap = m_SpotLightShader->getShaderSamplerId("normalMap");
    if (m_Settings.enableSpecular)
        m_SpotLightData.sgMap = m_SpotLightShader->getShaderSamplerId("sgMap");
    m_SpotLightData.depthMap = m_SpotLightShader->getShaderSamplerId("depthMap");
    m_SpotLightData.wvp = m_SpotLightShader->getShaderVarId("mtxWVP");

    //----AL----------------------------------------------------------------------   
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("ambLight.color", m_AmbDirIllLightData.ambColor);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.color", m_AmbDirIllLightData.dirColor);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.direction", m_AmbDirIllLightData.dirDirection);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.position", m_AmbDirIllLightData.dirPosition);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.nearFar", m_AmbDirIllLightData.dirNearFar);
    if (m_Settings.enableShadows)
    {
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight0", m_AmbDirIllLightData.mtxDirLight0);
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight1", m_AmbDirIllLightData.mtxDirLight1);
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight2", m_AmbDirIllLightData.mtxDirLight2);
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight3", m_AmbDirIllLightData.mtxDirLight3);
    }

    m_AmbDirIllLightData.colorIllMap  = m_AmbDirIllShader->getShaderSamplerId("colorIllMap");
    if (m_Settings.enableSpecular)
        m_AmbDirIllLightData.sgMap  = m_AmbDirIllShader->getShaderSamplerId("sgMap");
    m_AmbDirIllLightData.normalMap  = m_AmbDirIllShader->getShaderSamplerId("normalMap");
    m_AmbDirIllLightData.depthMap  = m_AmbDirIllShader->getShaderSamplerId("depthMap");
    
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
    uint width(m_OutputRenderTarget->getWidth()); 
    uint height(m_OutputRenderTarget->getHeight());


    //Collection Textures - just SGI and color others are shared
    // Color
    m_pColorIllTexture->setData(width, height, 0, 
        Texture2D::BufferLayout_BGRA, Texture2D::BufferType_Byte, 0 );

    // SG
    m_pSGTexture->setData(width, height, 0, 
        Texture2D::BufferLayout_BGRA, Texture2D::BufferType_Byte, 0 );

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
    GL::heBlendEnabled(false);
    m_CollectionRenderTarget->clear(he::Color(0.0f, 0, 0, 0));


    //////////////////////////////////////////////////////////////////////////
    ///                             DRAW                                   ///
    //////////////////////////////////////////////////////////////////////////
    const CameraPerspective* camera(scene->getCameraManager()->getActiveCamera());
    scene->getDrawList().draw(DrawListContainer::BlendFilter_Opac, camera, [&camera](IDrawable* d)
    {
        d->applyMaterial(camera);
        d->draw();
    });


    //////////////////////////////////////////////////////////////////////////
    ///                             POST                                   ///
    //////////////////////////////////////////////////////////////////////////
    m_OutputRenderTarget->prepareForRendering();

    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_One, BlendFunc_One);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    m_SharedShaderData.projParams.set(vec4(
        camera->getProjection()(0, 0),
        camera->getProjection()(1, 1),
        camera->getProjection()(2, 2),
        camera->getProjection()(2, 3)));
    m_SharedShaderData.pSharedBuffer->setShaderVar(m_SharedShaderData.projParams);

    m_PointLightShader->bind();
    postPointLights(scene);           
 
    m_SpotLightShader->bind();
    postSpotLights(scene);

    m_AmbDirIllShader->bind();
    postAmbDirIllLight(scene);

    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
}
void Deferred3DRenderer::draw2D(Renderer2D* renderer)
{
    if (m_ShowDebugTextures)
    {
        renderer->drawTexture2DToScreen(m_pColorIllTexture, vec2(12 * 1 + 256 * 0, 12), false, vec2(256, 144));
        renderer->drawTexture2DToScreen(m_pSGTexture,       vec2(12 * 2 + 256 * 1, 12), false, vec2(256, 144));
        renderer->drawTexture2DToScreen(m_pNormalTexture,   vec2(12 * 3 + 256 * 2, 12), false, vec2(256, 144));
        renderer->drawTexture2DToScreen(m_CollectionRenderTarget->getDepthTarget(),      vec2(12 * 4 + 256 * 3, 12), false, vec2(256, 144));

        renderer->drawTexture2DToScreen(m_OutputRenderTarget->getTextureTarget(0), vec2(12 * 1 + 256 * 0, 12 * 2 + 1 * 144), false, vec2(256, 144));
    }
}

void Deferred3DRenderer::postAmbDirIllLight(const Scene* scene)
{
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    LightManager* lightManager(scene->getLightManager());

    const AmbientLight* ambLight(lightManager->getAmbientLight());
    const DirectionalLight* dirLight(lightManager->getDirectionalLight());

    m_AmbDirIllLightData.ambColor.set(vec4(ambLight->color, ambLight->multiplier));
    m_AmbDirIllLightData.dirColor.set(vec4(dirLight->getColor(), dirLight->getMultiplier()));
    m_AmbDirIllLightData.dirDirection.set(normalize((scene->getCameraManager()->getActiveCamera()->getView() * vec4(dirLight->getDirection(), 0.0f)).xyz()));
    m_AmbDirIllLightData.dirPosition.set((scene->getCameraManager()->getActiveCamera()->getView() * vec4(dirLight->getShadowPosition(), 1.0f)).xyz());
    m_AmbDirIllLightData.dirNearFar.set(dirLight->getShadowNearFar());

    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.ambColor);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirColor);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirDirection);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirPosition);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirNearFar);

    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.colorIllMap, m_pColorIllTexture);
    if (m_Settings.enableSpecular)
        m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.sgMap,   m_pSGTexture);
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.normalMap,   m_pNormalTexture);
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.depthMap,    m_CollectionRenderTarget->getDepthTarget());

    if (m_Settings.enableShadows)       
    {
        m_AmbDirIllLightData.mtxDirLight0.set(lightManager->getDirectionalLight()->getShadowMatrix(0));
        m_AmbDirIllLightData.mtxDirLight1.set(lightManager->getDirectionalLight()->getShadowMatrix(1));
        m_AmbDirIllLightData.mtxDirLight2.set(lightManager->getDirectionalLight()->getShadowMatrix(2));
        m_AmbDirIllLightData.mtxDirLight3.set(lightManager->getDirectionalLight()->getShadowMatrix(3));

        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight0);
        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight1);
        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight2);
        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight3);

        m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap0, lightManager->getDirectionalLight()->getShadowMap(0));
        m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap1, lightManager->getDirectionalLight()->getShadowMap(1));
        m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap2, lightManager->getDirectionalLight()->getShadowMap(2));
        m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap3, lightManager->getDirectionalLight()->getShadowMap(3));
    }

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

    m_PointLightShader->setShaderVar(m_PointLightData.colorIllMap, m_pColorIllTexture);
    if (m_Settings.enableSpecular)
        m_PointLightShader->setShaderVar(m_PointLightData.sgMap,   m_pSGTexture);
    m_PointLightShader->setShaderVar(m_PointLightData.normalMap,   m_pNormalTexture);
    m_PointLightShader->setShaderVar(m_PointLightData.depthMap,    m_CollectionRenderTarget->getDepthTarget());
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);

    const CameraPerspective& camera(*scene->getCameraManager()->getActiveCamera());
    std::for_each(lights.cbegin(), lights.cend(), [&](const ObjectHandle& lightHandle)
    {
        PointLight* light(lightFactory->getPointLight(lightHandle));

        Sphere sphere(light->getPosition(), light->getEndAttenuation());
        if (camera.intersect(sphere) != IntersectResult_Outside)  
        {
            if (lengthSqr(light->getPosition() - camera.getPosition()) < sqr(light->getEndAttenuation() * 2 + camera.getNearClip())) //if inside light //HACK
            {
                GL::heSetCullFace(true);
                GL::heSetDepthFunc(DepthFunc_GeaterOrEqual);
            }
            else
            {
                GL::heSetCullFace(false);
                GL::heSetDepthFunc(DepthFunc_LessOrEqual);
            }

            m_PointLightData.position.set(camera.getView() * light->getPosition());
            m_PointLightData.multiplier.set(light->getMultiplier());
            m_PointLightData.color.set(light->getColor());
            m_PointLightData.beginAttenuation.set(light->getBeginAttenuation());
            m_PointLightData.endAttenuation.set(light->getEndAttenuation());

            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.position);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.multiplier);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.color);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.beginAttenuation);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.endAttenuation);

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

    m_SpotLightShader->setShaderVar(m_SpotLightData.colorIllMap, m_pColorIllTexture);
    if (m_Settings.enableSpecular)
        m_SpotLightShader->setShaderVar(m_SpotLightData.sgMap,   m_pSGTexture);
    m_SpotLightShader->setShaderVar(m_SpotLightData.normalMap,   m_pNormalTexture);
    m_SpotLightShader->setShaderVar(m_SpotLightData.depthMap,    m_CollectionRenderTarget->getDepthTarget());

    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);
    const CameraPerspective& camera(*scene->getCameraManager()->getActiveCamera());
    std::for_each(lights.cbegin(), lights.cend(), [&](const ObjectHandle& lightHandle)
    {
        SpotLight* light(lightFactory->getSpotLight(lightHandle));

        Sphere sphere(light->getPosition(), light->getEndAttenuation());
        if (camera.intersect(sphere) != IntersectResult_Outside)  
        {
            if (lengthSqr(light->getPosition() - camera.getPosition()) < sqr(light->getEndAttenuation() * 2 + camera.getNearClip())) //if inside light //HACK
            {
                GL::heSetCullFace(true);
                GL::heSetDepthFunc(DepthFunc_GeaterOrEqual);
            }
            else
            {
                GL::heSetCullFace(false);
                GL::heSetDepthFunc(DepthFunc_LessOrEqual);
            }
            m_SpotLightData.position.set(camera.getView() * light->getPosition());
            m_SpotLightData.multiplier.set(light->getMultiplier());
            m_SpotLightData.direction.set(normalize((camera.getView() * vec4(light->getDirection(), 0)).xyz()));
            m_SpotLightData.beginAttenuation.set(light->getBeginAttenuation());
            m_SpotLightData.color.set(light->getColor());
            m_SpotLightData.endAttenuation.set(light->getEndAttenuation());
            m_SpotLightData.cosCutOff.set(light->getCosCutoff());

            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.position);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.multiplier);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.direction);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.beginAttenuation);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.color);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.endAttenuation);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.cosCutOff);

            m_SpotLightShader->setShaderVar(m_SpotLightData.wvp, camera.getViewProjection() * light->getWorldMatrix());

            GL::heBindVao(light->getLightVolume()->getVertexArraysID());
            glDrawElements(GL_TRIANGLES, light->getLightVolume()->getNumIndices(), light->getLightVolume()->getIndexType(), 0);
        }
    });
    GL::heSetCullFace(false);
}

} } //end namespace