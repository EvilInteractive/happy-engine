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
#include "Light.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"

#include "LightManager.h"
#include "CameraManager.h"
#include "Camera.h"
#include "DrawManager.h"

namespace he {
namespace gfx {

//const int Deferred3DRenderer::TEXTURE_FORMAT[TEXTURES] = { GL_BGRA, GL_BGRA, GL_RG, GL_DEPTH_COMPONENT };
//const int Deferred3DRenderer::TEXTURE_INTERNALFORMAT[TEXTURES] = {GL_RGBA8, GL_RGBA8, GL_RG16F, GL_DEPTH_COMPONENT32F };
//const int Deferred3DRenderer::TEXTURE_ATTACHMENT[TEXTURES] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT};
BufferLayout Deferred3DRenderer::s_VertexLayoutFullscreenQuad = BufferLayout();

Deferred3DRenderer::Deferred3DRenderer(): 
            m_pQuad(nullptr), 
            m_ShowDebugTextures(false),
            m_pPointLightShader(nullptr),
            m_pSpotLightShader(nullptr),
            m_pAmbDirIllShader(nullptr),
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
    CONSOLE->registerVar(&m_ShowDebugTextures, "debugTex");

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

    //m_pColorRampTex->release();

    m_pQuad->release();

    glDeleteFramebuffers(1, &m_CollectionFboId);
    glDeleteFramebuffers(1, &m_RenderFboId);

    delete m_pPointLightShader;
    delete m_pSpotLightShader;
    delete m_pAmbDirIllShader;
}
void Deferred3DRenderer::compileShaders()
{
    //////////////////////////////////////////////////////////////////////////
    ///                                 CLEAN                              ///
    //////////////////////////////////////////////////////////////////////////
    delete m_pPointLightShader;
    delete m_pSpotLightShader;
    delete m_pAmbDirIllShader;

    m_pPointLightShader = NEW Shader();
    m_pSpotLightShader = NEW Shader();
    m_pAmbDirIllShader = NEW Shader();

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
    m_pPointLightShader->initFromFile(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostPLShader.frag", shaderLayout, shaderDefines);
    m_pSpotLightShader->initFromFile(folder  + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostSLShader.frag", shaderLayout, shaderDefines);
    m_pAmbDirIllShader->initFromFile(folder  + "shared/postShaderQuad.vert", folder + "deferred/post/deferredPostAmbientIllShader.frag", shaderLayout, shaderDefines);

    //SharedBuffer
    m_SharedShaderData.pSharedBuffer = m_pPointLightShader->setBuffer(m_pPointLightShader->getBufferId("SharedBuffer"));
    m_pSpotLightShader->setBuffer(m_pSpotLightShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);
    m_pAmbDirIllShader->setBuffer(m_pAmbDirIllShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);

    m_SharedShaderData.pSharedBuffer->getShaderVar("projParams", m_SharedShaderData.projParams);

    //LightBuffer
    m_PointLightData.pLightBuffer = m_pPointLightShader->setBuffer(m_pPointLightShader->getBufferId("LightBuffer"));
    m_SpotLightData.pLightBuffer = m_pSpotLightShader->setBuffer(m_pSpotLightShader->getBufferId("LightBuffer"));
    m_AmbDirIllLightData.pLightBuffer = m_pAmbDirIllShader->setBuffer(m_pAmbDirIllShader->getBufferId("LightBuffer"));
    if (m_RenderSettings.enableShadows)
        m_AmbDirIllLightData.pPerFrameBuffer = m_pAmbDirIllShader->setBuffer(m_pAmbDirIllShader->getBufferId("PerFrameBuffer"));

    //----PL----------------------------------------------------------------------
    m_PointLightData.pLightBuffer->getShaderVar("light.position", m_PointLightData.position);
    m_PointLightData.pLightBuffer->getShaderVar("light.multiplier", m_PointLightData.multiplier);
    m_PointLightData.pLightBuffer->getShaderVar("light.color", m_PointLightData.color);
    m_PointLightData.pLightBuffer->getShaderVar("light.beginAttenuation", m_PointLightData.beginAttenuation);
    m_PointLightData.pLightBuffer->getShaderVar("light.endAttenuation", m_PointLightData.endAttenuation);
    m_PointLightData.colorIllMap = m_pPointLightShader->getShaderSamplerId("colorIllMap");
    m_PointLightData.normalMap = m_pPointLightShader->getShaderSamplerId("normalMap");
    if (m_RenderSettings.enableSpecular)
        m_PointLightData.sgMap = m_pPointLightShader->getShaderSamplerId("sgMap");
    m_PointLightData.depthMap = m_pPointLightShader->getShaderSamplerId("depthMap");
    m_PointLightData.wvp = m_pPointLightShader->getShaderVarId("mtxWVP");
    //----SL----------------------------------------------------------------------
    m_SpotLightData.pLightBuffer->getShaderVar("light.position", m_SpotLightData.position);
    m_SpotLightData.pLightBuffer->getShaderVar("light.multiplier", m_SpotLightData.multiplier);
    m_SpotLightData.pLightBuffer->getShaderVar("light.direction", m_SpotLightData.direction);
    m_SpotLightData.pLightBuffer->getShaderVar("light.beginAttenuation", m_SpotLightData.beginAttenuation);
    m_SpotLightData.pLightBuffer->getShaderVar("light.color", m_SpotLightData.color);
    m_SpotLightData.pLightBuffer->getShaderVar("light.endAttenuation", m_SpotLightData.endAttenuation);
    m_SpotLightData.pLightBuffer->getShaderVar("light.cosCutoff", m_SpotLightData.cosCutOff);
    m_SpotLightData.colorIllMap = m_pSpotLightShader->getShaderSamplerId("colorIllMap");
    m_SpotLightData.normalMap = m_pSpotLightShader->getShaderSamplerId("normalMap");
    if (m_RenderSettings.enableSpecular)
        m_SpotLightData.sgMap = m_pSpotLightShader->getShaderSamplerId("sgMap");
    m_SpotLightData.depthMap = m_pSpotLightShader->getShaderSamplerId("depthMap");
    m_SpotLightData.wvp = m_pSpotLightShader->getShaderVarId("mtxWVP");

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

    m_AmbDirIllLightData.colorIllMap  = m_pAmbDirIllShader->getShaderSamplerId("colorIllMap");
    if (m_RenderSettings.enableSpecular)
        m_AmbDirIllLightData.sgMap  = m_pAmbDirIllShader->getShaderSamplerId("sgMap");
    m_AmbDirIllLightData.normalMap  = m_pAmbDirIllShader->getShaderSamplerId("normalMap");
    m_AmbDirIllLightData.depthMap  = m_pAmbDirIllShader->getShaderSamplerId("depthMap");
    //m_AmbDirIllLightData.colorRamp  = m_pAmbDirIllShader->getShaderSamplerId("colorRamp");
    //m_pColorRampTex = CONTENT->asyncLoadTexture("engine/shadingRamp.png");
    
    if (m_RenderSettings.enableShadows)
    {
        m_AmbDirIllLightData.shadowMap0 = m_pAmbDirIllShader->getShaderSamplerId("shadowMap0");
        m_AmbDirIllLightData.shadowMap1 = m_pAmbDirIllShader->getShaderSamplerId("shadowMap1");
        m_AmbDirIllLightData.shadowMap2 = m_pAmbDirIllShader->getShaderSamplerId("shadowMap2");
        m_AmbDirIllLightData.shadowMap3 = m_pAmbDirIllShader->getShaderSamplerId("shadowMap3");
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
    GL::heClearColor(Color(vec4(backgroundColor, 1.0f)));
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

    m_SharedShaderData.projParams = vec4(
        CAMERAMANAGER->getActiveCamera()->getProjection()(0, 0),
        CAMERAMANAGER->getActiveCamera()->getProjection()(1, 1),
        CAMERAMANAGER->getActiveCamera()->getProjection()(2, 2),
        CAMERAMANAGER->getActiveCamera()->getProjection()(2, 3));
    m_SharedShaderData.pSharedBuffer->setShaderVar(m_SharedShaderData.projParams);


    m_pPointLightShader->bind();
    postPointLights();           

    m_pSpotLightShader->bind();
    postSpotLights();

    m_pAmbDirIllShader->bind();
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
    //if (m_ShowDebugTextures)
    {
        //GUI_NEW->drawTexture2DToScreen(m_pColorIllTexture, vec2(12 * 1 + 256 * 0, 12), false, vec2(256, 144));
        //GUI_NEW->drawTexture2DToScreen(m_pSGTexture,       vec2(12 * 2 + 256 * 1, 12), false, vec2(256, 144));
        //GUI_NEW->drawTexture2DToScreen(m_pNormalTexture,   vec2(12 * 3 + 256 * 2, 12), false, vec2(256, 144));
        //GUI_NEW->drawTexture2DToScreen(m_pDepthTexture,    vec2(12 * 4 + 256 * 3, 12), false, vec2(256, 144));
    }
}

void Deferred3DRenderer::postAmbDirIllLight()
{
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    LightManager* pLightManager(GRAPHICS->getLightManager());

    const AmbientLight::pointer& pAmbLight(pLightManager->getAmbientLight());
    const DirectionalLight::pointer& pDirLight(pLightManager->getDirectionalLight());

    m_AmbDirIllLightData.ambColor = vec4(pAmbLight->color, pAmbLight->multiplier);
    m_AmbDirIllLightData.dirColor = vec4(pDirLight->getColor(), pDirLight->getMultiplier());
    m_AmbDirIllLightData.dirDirection = normalize((CAMERAMANAGER->getActiveCamera()->getView() * vec4(pDirLight->getDirection(), 0.0f)).xyz());
    m_AmbDirIllLightData.dirPosition = (CAMERAMANAGER->getActiveCamera()->getView() * vec4(pDirLight->getShadowPosition(), 1.0f)).xyz();
    m_AmbDirIllLightData.dirNearFar = pDirLight->getShadowNearFar();

    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.ambColor);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirColor);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirDirection);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirPosition);
    m_AmbDirIllLightData.pLightBuffer->setShaderVar(m_AmbDirIllLightData.dirNearFar);

    m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.colorIllMap, m_pColorIllTexture);
    if (m_RenderSettings.enableSpecular)
        m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.sgMap,   m_pSGTexture);
    m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.normalMap,   m_pNormalTexture);
    m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.depthMap,    m_pDepthTexture);
    //m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.colorRamp,   m_pColorRampTex);

    if (m_RenderSettings.enableShadows)       
    {
        m_AmbDirIllLightData.mtxDirLight0 = pLightManager->getDirectionalLight()->getShadowMatrix(0);
        m_AmbDirIllLightData.mtxDirLight1 = pLightManager->getDirectionalLight()->getShadowMatrix(1);
        m_AmbDirIllLightData.mtxDirLight2 = pLightManager->getDirectionalLight()->getShadowMatrix(2);
        m_AmbDirIllLightData.mtxDirLight3 = pLightManager->getDirectionalLight()->getShadowMatrix(3);

        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight0);
        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight1);
        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight2);
        m_AmbDirIllLightData.pPerFrameBuffer->setShaderVar(m_AmbDirIllLightData.mtxDirLight3);

        m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap0, pLightManager->getDirectionalLight()->getShadowMap(0));
        m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap1, pLightManager->getDirectionalLight()->getShadowMap(1));
        m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap2, pLightManager->getDirectionalLight()->getShadowMap(2));
        m_pAmbDirIllShader->setShaderVar(m_AmbDirIllLightData.shadowMap3, pLightManager->getDirectionalLight()->getShadowMap(3));
    }

    GL::heBindVao(m_pQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
}
void Deferred3DRenderer::postPointLights()
{
    LightManager* pLightManager(GRAPHICS->getLightManager());
    const std::vector<PointLight::pointer>& lights(pLightManager->getPointLights());

    if (lights.size() == 0 || lights.back()->getLightVolume()->isLoaded() == false)
        return;

    m_pPointLightShader->setShaderVar(m_PointLightData.colorIllMap, m_pColorIllTexture);
    if (m_RenderSettings.enableSpecular)
        m_pPointLightShader->setShaderVar(m_PointLightData.sgMap,   m_pSGTexture);
    m_pPointLightShader->setShaderVar(m_PointLightData.normalMap,   m_pNormalTexture);
    m_pPointLightShader->setShaderVar(m_PointLightData.depthMap,    m_pDepthTexture);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);
    const Camera& camera(*CAMERAMANAGER->getActiveCamera());
    std::for_each(lights.cbegin(), lights.cend(), [&](const PointLight::pointer& pLight)
    {
        shapes::Sphere bsphere(pLight->getPosition(), pLight->getEndAttenuation());
        if (DrawManager::viewClip(camera.getPosition(), camera.getLook(), camera.getFarClip(), bsphere) == false)  
        {
            if (lengthSqr(pLight->getPosition() - camera.getPosition()) < sqr(pLight->getEndAttenuation() * 2 + camera.getNearClip())) //if inside light //HACK
            {
                GL::heSetCullFace(true);
                GL::heSetDepthFunc(DepthFunc_GeaterOrEqual);
            }
            else
            {
                GL::heSetCullFace(false);
                GL::heSetDepthFunc(DepthFunc_LessOrEqual);
            }

            m_PointLightData.position = camera.getView() * pLight->getPosition();
            m_PointLightData.multiplier = pLight->getMultiplier();
            m_PointLightData.color = pLight->getColor();
            m_PointLightData.beginAttenuation = pLight->getBeginAttenuation();
            m_PointLightData.endAttenuation = pLight->getEndAttenuation();

            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.position);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.multiplier);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.color);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.beginAttenuation);
            m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.endAttenuation);

            m_pPointLightShader->setShaderVar(m_PointLightData.wvp, camera.getViewProjection() * pLight->getWorldMatrix());

            GL::heBindVao(pLight->getLightVolume()->getVertexArraysID());
            glDrawElements(GL_TRIANGLES, pLight->getLightVolume()->getNumIndices(), pLight->getLightVolume()->getIndexType(), 0);
        }
    });
    GL::heSetCullFace(false);
}
void Deferred3DRenderer::postSpotLights()
{
    LightManager* pLightManager(GRAPHICS->getLightManager());
    const std::vector<SpotLight::pointer>& lights(pLightManager->getSpotLights());
    if (lights.size() == 0 || lights.back()->getLightVolume()->isLoaded() == false)
        return;

    m_pSpotLightShader->setShaderVar(m_SpotLightData.colorIllMap, m_pColorIllTexture);
    if (m_RenderSettings.enableSpecular)
        m_pSpotLightShader->setShaderVar(m_SpotLightData.sgMap,   m_pSGTexture);
    m_pSpotLightShader->setShaderVar(m_SpotLightData.normalMap,   m_pNormalTexture);
    m_pSpotLightShader->setShaderVar(m_SpotLightData.depthMap,    m_pDepthTexture);

    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(true);
    const Camera& camera(*CAMERAMANAGER->getActiveCamera());
    std::for_each(lights.cbegin(), lights.cend(), [&](const SpotLight::pointer& pLight)
    {
        shapes::Sphere bsphere(pLight->getPosition(), pLight->getEndAttenuation());
        if (DrawManager::viewClip(camera.getPosition(), camera.getLook(), camera.getFarClip(), bsphere) == false) 
        {
            if (lengthSqr(pLight->getPosition() - camera.getPosition()) < sqr(pLight->getEndAttenuation() * 2 + camera.getNearClip())) //if inside light //HACK
            {
                GL::heSetCullFace(true);
                GL::heSetDepthFunc(DepthFunc_GeaterOrEqual);
            }
            else
            {
                GL::heSetCullFace(false);
                GL::heSetDepthFunc(DepthFunc_LessOrEqual);
            }
            m_SpotLightData.position = camera.getView() * pLight->getPosition();
            m_SpotLightData.multiplier = pLight->getMultiplier();
            m_SpotLightData.direction = normalize((camera.getView() * vec4(pLight->getDirection(), 0)).xyz());
            m_SpotLightData.beginAttenuation = pLight->getBeginAttenuation();
            m_SpotLightData.color = pLight->getColor();
            m_SpotLightData.endAttenuation = pLight->getEndAttenuation();
            m_SpotLightData.cosCutOff = pLight->getCosCutoff();

            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.position);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.multiplier);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.direction);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.beginAttenuation);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.color);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.endAttenuation);
            m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.cosCutOff);

            m_pSpotLightShader->setShaderVar(m_SpotLightData.wvp, camera.getViewProjection() * pLight->getWorldMatrix());

            GL::heBindVao(pLight->getLightVolume()->getVertexArraysID());
            glDrawElements(GL_TRIANGLES, pLight->getLightVolume()->getNumIndices(), pLight->getLightVolume()->getIndexType(), 0);
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