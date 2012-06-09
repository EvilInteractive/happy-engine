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
#include "Camera.h"
#include "DrawManager.h"
#include "LightFactory.h"

namespace he {
namespace gfx {

BufferLayout Deferred3DRenderer::s_VertexLayoutFullscreenQuad = BufferLayout();

Deferred3DRenderer::Deferred3DRenderer(): 
            m_pQuad(nullptr), 
            m_ShowDebugTextures(false),
            m_PointLightShader(nullptr),
            m_SpotLightShader(nullptr),
            m_AmbDirIllShader(nullptr),
            m_CollectionFboId(UINT_MAX),
            m_RenderFboId(UINT_MAX)
{
    ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
    m_pColorIllTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_pColorIllTexture->setName("Deferred3DRenderer::m_pColorIllTexture");
    handle = ResourceFactory<Texture2D>::getInstance()->create();
    m_pSGTexture = ResourceFactory<Texture2D>::getInstance()->get(handle);
    m_pSGTexture->setName("Deferred3DRenderer::m_pSGTexture");
}
void Deferred3DRenderer::init( const RenderSettings& settings, 
            const Texture2D* pOutTarget, const Texture2D* pOutNormalTarget, const Texture2D* pOutDepthTarget )
{
    m_RenderSettings = settings;
    CONSOLE->registerVar(&m_ShowDebugTextures, "debugDefTex");

    ResourceFactory<Texture2D>::getInstance()->instantiate(pOutTarget->getHandle());
    ResourceFactory<Texture2D>::getInstance()->instantiate(pOutNormalTarget->getHandle());
    ResourceFactory<Texture2D>::getInstance()->instantiate(pOutDepthTarget->getHandle());
    m_pOutTexture = pOutTarget;
    m_pNormalTexture = pOutNormalTarget;
    m_pDepthTexture = pOutDepthTarget;
    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    onScreenResized();


    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD SHADERS                              ///
    //////////////////////////////////////////////////////////////////////////
    compileShaders();


    //////////////////////////////////////////////////////////////////////////
    ///                         LOAD RENDER QUAD                           ///
    //////////////////////////////////////////////////////////////////////////
    m_pQuad = CONTENT->getFullscreenQuad();
}


Deferred3DRenderer::~Deferred3DRenderer()
{
    m_pColorIllTexture->release();
    m_pSGTexture->release();
    m_pOutTexture->release();
    m_pNormalTexture->release();
    m_pDepthTexture->release();
    
    m_pQuad->release();

    glDeleteFramebuffers(1, &m_CollectionFboId);
    glDeleteFramebuffers(1, &m_RenderFboId);

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
    if (m_RenderSettings.enableSpecular)
        shaderDefines.insert("SPECULAR");
    if (m_RenderSettings.enableShadows)
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
    if (m_RenderSettings.enableShadows)
        m_AmbDirIllLightData.pPerFrameBuffer = m_AmbDirIllShader->setBuffer(m_AmbDirIllShader->getBufferId("PerFrameBuffer"));

    //----PL----------------------------------------------------------------------
    m_PointLightData.pLightBuffer->getShaderVar("light.position", m_PointLightData.position);
    m_PointLightData.pLightBuffer->getShaderVar("light.multiplier", m_PointLightData.multiplier);
    m_PointLightData.pLightBuffer->getShaderVar("light.color", m_PointLightData.color);
    m_PointLightData.pLightBuffer->getShaderVar("light.beginAttenuation", m_PointLightData.beginAttenuation);
    m_PointLightData.pLightBuffer->getShaderVar("light.endAttenuation", m_PointLightData.endAttenuation);
    m_PointLightData.colorIllMap = m_PointLightShader->getShaderSamplerId("colorIllMap");
    m_PointLightData.normalMap = m_PointLightShader->getShaderSamplerId("normalMap");
    if (m_RenderSettings.enableSpecular)
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
    if (m_RenderSettings.enableSpecular)
        m_SpotLightData.sgMap = m_SpotLightShader->getShaderSamplerId("sgMap");
    m_SpotLightData.depthMap = m_SpotLightShader->getShaderSamplerId("depthMap");
    m_SpotLightData.wvp = m_SpotLightShader->getShaderVarId("mtxWVP");

    //----AL----------------------------------------------------------------------   
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("ambLight.color", m_AmbDirIllLightData.ambColor);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.color", m_AmbDirIllLightData.dirColor);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.direction", m_AmbDirIllLightData.dirDirection);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.position", m_AmbDirIllLightData.dirPosition);
    m_AmbDirIllLightData.pLightBuffer->getShaderVar("dirLight.nearFar", m_AmbDirIllLightData.dirNearFar);
    if (m_RenderSettings.enableShadows)
    {
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight0", m_AmbDirIllLightData.mtxDirLight0);
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight1", m_AmbDirIllLightData.mtxDirLight1);
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight2", m_AmbDirIllLightData.mtxDirLight2);
        m_AmbDirIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight3", m_AmbDirIllLightData.mtxDirLight3);
    }

    m_AmbDirIllLightData.colorIllMap  = m_AmbDirIllShader->getShaderSamplerId("colorIllMap");
    if (m_RenderSettings.enableSpecular)
        m_AmbDirIllLightData.sgMap  = m_AmbDirIllShader->getShaderSamplerId("sgMap");
    m_AmbDirIllLightData.normalMap  = m_AmbDirIllShader->getShaderSamplerId("normalMap");
    m_AmbDirIllLightData.depthMap  = m_AmbDirIllShader->getShaderSamplerId("depthMap");
    
    if (m_RenderSettings.enableShadows)
    {
        m_AmbDirIllLightData.shadowMap0 = m_AmbDirIllShader->getShaderSamplerId("shadowMap0");
        m_AmbDirIllLightData.shadowMap1 = m_AmbDirIllShader->getShaderSamplerId("shadowMap1");
        m_AmbDirIllLightData.shadowMap2 = m_AmbDirIllShader->getShaderSamplerId("shadowMap2");
        m_AmbDirIllLightData.shadowMap3 = m_AmbDirIllShader->getShaderSamplerId("shadowMap3");
    }
}

void Deferred3DRenderer::onScreenResized()
{
    if (m_CollectionFboId != UINT_MAX)
        glDeleteFramebuffers(1, &m_CollectionFboId);
    if (m_RenderFboId != UINT_MAX)
        glDeleteFramebuffers(1, &m_RenderFboId);

    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    int width  = GRAPHICS->getScreenWidth(), 
        height = GRAPHICS->getScreenHeight();


    //Collection Textures - just SGI and color others are shared

    // Color
    m_pColorIllTexture->setData(width, height, Texture2D::TextureFormat_RGBA8, 0, 
        Texture2D::BufferLayout_BGRA, Texture2D::BufferType_Byte,
        Texture2D::WrapType_Clamp, Texture2D::FilterType_Nearest, false, false );

    // SG
    m_pSGTexture->setData(width, height, Texture2D::TextureFormat_RGBA8, 0, 
        Texture2D::BufferLayout_BGRA, Texture2D::BufferType_Byte,
        Texture2D::WrapType_Clamp, Texture2D::FilterType_Nearest, false, false );

    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////

    //FBO Collection
    glGenFramebuffers(1, &m_CollectionFboId);
    GL::heBindFbo(m_CollectionFboId);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pColorIllTexture->getID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_pSGTexture->getID(),       0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_pNormalTexture->getID(),   0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, m_pDepthTexture->getID(),    0);
    err::checkFboStatus("deferred collection");

    //Fbo Render
    glGenFramebuffers(1, &m_RenderFboId);
    GL::heBindFbo(m_RenderFboId);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pOutTexture->getID(),   0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, m_pDepthTexture->getID(), 0); //depth needed for light volumes

    err::checkFboStatus("deferred render");

    GL::heBindFbo(0);
}

void Deferred3DRenderer::clear( bool color, bool normal, bool depth )
{
    GL::heBindFbo(m_CollectionFboId);

    GLenum buffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    int numBuffers(0);
    if (color)
    {
        //buffers[numBuffers++] = GL_COLOR_ATTACHMENT0; //no use use the color of rendertarget
        buffers[numBuffers++] = GL_COLOR_ATTACHMENT1;
    }
    if (normal)
    {
        buffers[numBuffers++] = GL_COLOR_ATTACHMENT2;
    }
    glDrawBuffers(numBuffers, buffers);

    GLbitfield flags(0);
    if (color || normal)
        flags |= GL_COLOR_BUFFER_BIT;
    if (depth)
    {
        GL::heSetDepthWrite(true);
        flags |= GL_DEPTH_BUFFER_BIT;
    }

    vec3 backgroundColor(GRAPHICS->getLightManager()->getDirectionalLight()->getColor() * GRAPHICS->getLightManager()->getDirectionalLight()->getMultiplier() * 2);
    GL::heClearColor(he::Color(he::vec4(backgroundColor, 1.0f)));
    //GL::heClearColor(Color(0.0f,0.0f,0.0f,0.0f));
    glClear(flags);

    if (color)
    {
        GL::heClearColor(Color(vec4(0.0f, 0.0f, 0.0f, 0.0f)));
        GL::heBindFbo(m_RenderFboId);
        buffers[0] = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, buffers);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Deferred3DRenderer::draw(const DrawListContainer& drawList, uint renderFlags)
{
    //////////////////////////////////////////////////////////////////////////
    ///                             BEGIN                                  ///
    //////////////////////////////////////////////////////////////////////////
    GL::heBindFbo(m_CollectionFboId);
    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
    GL::heBlendEnabled(false);

    const static GLenum tempBuffer[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, tempBuffer);
    //GL::heClearColor(Color(vec4(backgroundColor, 1.0f)));    
    //glClear(GL_COLOR_BUFFER_BIT);

    const static GLenum collectBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, collectBuffers);

    //////////////////////////////////////////////////////////////////////////
    ///                             DRAW                                   ///
    //////////////////////////////////////////////////////////////////////////
    drawList.for_each(renderFlags, [](IDrawable* pD)
    {
        if (pD->isInCamera(CAMERAMANAGER->getActiveCamera()))
        {
            pD->applyMaterial(CAMERAMANAGER->getActiveCamera());
            pD->draw();
        }
    });


    //////////////////////////////////////////////////////////////////////////
    ///                             POST                                   ///
    //////////////////////////////////////////////////////////////////////////
    GL::heBindFbo(m_RenderFboId);
    const static GLenum renderBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, renderBuffers);

    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_One, BlendFunc_One);
    GL::heBlendEquation(BlendEquation_Add);
    GL::heSetDepthRead(false);

    m_SharedShaderData.projParams.set(vec4(
        CAMERAMANAGER->getActiveCamera()->getProjection()(0, 0),
        CAMERAMANAGER->getActiveCamera()->getProjection()(1, 1),
        CAMERAMANAGER->getActiveCamera()->getProjection()(2, 2),
        CAMERAMANAGER->getActiveCamera()->getProjection()(2, 3)));
    m_SharedShaderData.pSharedBuffer->setShaderVar(m_SharedShaderData.projParams);


    m_PointLightShader->bind();
    postPointLights();           

    m_SpotLightShader->bind();
    postSpotLights();

    m_AmbDirIllShader->bind();
    postAmbDirIllLight();

    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);


    GL::heBindFbo(0);

    GL::heBindTexture2D(0, m_pOutTexture->getID());
    glGenerateMipmap(GL_TEXTURE_2D);


    drawDebugTextures();
}
void Deferred3DRenderer::drawDebugTextures() const
{
    if (m_ShowDebugTextures)
    {
        GUI->drawTexture2DToScreen(m_pColorIllTexture, vec2(12 * 1 + 256 * 0, 12), false, vec2(256, 144));
        GUI->drawTexture2DToScreen(m_pSGTexture,       vec2(12 * 2 + 256 * 1, 12), false, vec2(256, 144));
        GUI->drawTexture2DToScreen(m_pNormalTexture,   vec2(12 * 3 + 256 * 2, 12), false, vec2(256, 144));
        GUI->drawTexture2DToScreen(m_pDepthTexture,    vec2(12 * 4 + 256 * 3, 12), false, vec2(256, 144));
    }
}

void Deferred3DRenderer::postAmbDirIllLight()
{
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    LightManager* lightManager(GRAPHICS->getLightManager());

    const AmbientLight* ambLight(lightManager->getAmbientLight());
    const DirectionalLight* dirLight(lightManager->getDirectionalLight());

    m_AmbDirIllLightData.ambColor.set(vec4(ambLight->color, ambLight->multiplier));
    m_AmbDirIllLightData.dirColor.set(vec4(dirLight->getColor(), dirLight->getMultiplier()));
    m_AmbDirIllLightData.dirDirection.set(normalize((CAMERAMANAGER->getActiveCamera()->getView() * vec4(dirLight->getDirection(), 0.0f)).xyz()));
    m_AmbDirIllLightData.dirPosition.set((CAMERAMANAGER->getActiveCamera()->getView() * vec4(dirLight->getShadowPosition(), 1.0f)).xyz());
    m_AmbDirIllLightData.dirNearFar.set(dirLight->getShadowNearFar());

    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.ambColor);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirColor);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirDirection);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirPosition);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirNearFar);

    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.colorIllMap, m_pColorIllTexture);
    if (m_RenderSettings.enableSpecular)
        m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.sgMap,   m_pSGTexture);
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.normalMap,   m_pNormalTexture);
    m_AmbDirIllShader->setShaderVar(m_AmbDirIllLightData.depthMap,    m_pDepthTexture);

    if (m_RenderSettings.enableShadows)       
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
void Deferred3DRenderer::postPointLights()
{
    LightManager* lightManager(GRAPHICS->getLightManager());
    const std::vector<ObjectHandle>& lights(lightManager->getPointLights());

    const LightFactory* lightFactory(LightFactory::getInstance());

    if (lights.size() == 0 || lightFactory->getPointLight(lights.back())->getLightVolume()->isLoaded() == false)
        return;

    m_PointLightShader->setShaderVar(m_PointLightData.colorIllMap, m_pColorIllTexture);
    if (m_RenderSettings.enableSpecular)
        m_PointLightShader->setShaderVar(m_PointLightData.sgMap,   m_pSGTexture);
    m_PointLightShader->setShaderVar(m_PointLightData.normalMap,   m_pNormalTexture);
    m_PointLightShader->setShaderVar(m_PointLightData.depthMap,    m_pDepthTexture);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);
    const Camera& camera(*CAMERAMANAGER->getActiveCamera());
    std::for_each(lights.cbegin(), lights.cend(), [&](const ObjectHandle& lightHandle)
    {
        PointLight* light(lightFactory->getPointLight(lightHandle));

        shapes::Sphere bsphere(light->getPosition(), light->getEndAttenuation());
        if (DrawManager::viewClip(camera.getPosition(), camera.getLook(), camera.getFarClip(), bsphere) == false)  
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
void Deferred3DRenderer::postSpotLights()
{
    LightManager* pLightManager(GRAPHICS->getLightManager());
    const std::vector<ObjectHandle>& lights(pLightManager->getSpotLights());

    const LightFactory* lightFactory(LightFactory::getInstance());

    if (lights.size() == 0 || lightFactory->getSpotLight(lights.back())->getLightVolume()->isLoaded() == false)
        return;

    m_SpotLightShader->setShaderVar(m_SpotLightData.colorIllMap, m_pColorIllTexture);
    if (m_RenderSettings.enableSpecular)
        m_SpotLightShader->setShaderVar(m_SpotLightData.sgMap,   m_pSGTexture);
    m_SpotLightShader->setShaderVar(m_SpotLightData.normalMap,   m_pNormalTexture);
    m_SpotLightShader->setShaderVar(m_SpotLightData.depthMap,    m_pDepthTexture);

    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);
    const Camera& camera(*CAMERAMANAGER->getActiveCamera());
    std::for_each(lights.cbegin(), lights.cend(), [&](const ObjectHandle& lightHandle)
    {
        SpotLight* light(lightFactory->getSpotLight(lightHandle));

        shapes::Sphere bsphere(light->getPosition(), light->getEndAttenuation());
        if (DrawManager::viewClip(camera.getPosition(), camera.getLook(), camera.getFarClip(), bsphere) == false) 
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

void Deferred3DRenderer::setRenderSettings( const RenderSettings& settings )
{
    m_RenderSettings = settings;
    onScreenResized();
    compileShaders();
}

} } //end namespace