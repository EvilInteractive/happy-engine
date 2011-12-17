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
#include "HappyPCH.h" 

#include "Deferred3DRenderer.h"
#include "Happy2DRenderer.h"
#include "HappyEngine.h"
#include "OpenGL.h"
#include "VertexLayout.h"
#include "Vertex.h"
#include "HeAssert.h"
#include "Light.h"
#include "HappyNew.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "ExternalError.h"

#include "LightManager.h"
#include "CameraManager.h"
#include "Camera.h"
#include "Bloom.h"
#include "AutoExposure.h"
#include "DrawSettings.h"

#include <vector>

namespace he {
namespace gfx {

// Textures:
//      - Color.rgb, ill       GL_RGBA8
//      - spec, gloss,         GL_RGBA8
//      - Normal.xy            GL_RG16F
//      - Depth                GL_DEPTH24_STENCIL8
const int Deferred3DRenderer::TEXTURE_FORMAT[TEXTURES] = { GL_BGRA, GL_BGRA, GL_RG, GL_DEPTH_COMPONENT };
const int Deferred3DRenderer::TEXTURE_INTERNALFORMAT[TEXTURES] = {GL_RGBA8, GL_RGBA8, GL_RG16F, GL_DEPTH_COMPONENT32F };
const int Deferred3DRenderer::TEXTURE_ATTACHMENT[TEXTURES] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT};
VertexLayout Deferred3DRenderer::s_VertexLayoutFullscreenQuad = VertexLayout();

Deferred3DRenderer::Deferred3DRenderer(const DrawSettings& settings): 
            m_pQuad(NEW ModelMesh("deferred3DRenderer_QUAD")), 
            m_pLightManager(NEW LightManager()),
            m_pBloom(NEW Bloom()),
            m_pAutoExposure(NEW AutoExposure()),
            m_pRenderTexture(NEW Texture2D()),
            m_pToneMapShader(nullptr),
            m_Bloom(settings.getBloomEnabled()),
            m_Exposure(0.5f),
            m_Gamma(1.0f),
            m_ShowDebugTextures(false),
            m_bSSAO(settings.getSSAOEnabled()),
            m_pPointLightShader(NEW Shader()),
            m_pSpotLightShader(NEW Shader()),
            m_pAmbIllShader(NEW Shader())
{
    //CONSOLE->registerVar(&m_Exposure, "exposure");
    //CONSOLE->registerVar(&m_Gamma, "gamma");
    CONSOLE->registerVar(&m_ShowDebugTextures, "debugTex");
    //CONSOLE->registerVar(&m_Bloom, "bloom");


    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    resized();
    

    if (m_Bloom)
        m_pBloom->init();
    m_pAutoExposure->init();
    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD SHADERS                              ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    s_VertexLayoutFullscreenQuad.addElement(VertexElement(0, VertexElement::Type_Vec3, VertexElement::Usage_Position, 12, 0));

    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    m_pPointLightShader->init(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostPLShader.frag", shaderLayout);
    m_pSpotLightShader->init(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostSLShader.frag", shaderLayout);
    m_pAmbIllShader->init(folder + "deferred/post/deferredPostShaderQuad.vert", folder + "deferred/post/deferredPostAmbientIllShader.frag", shaderLayout);
    
    //SharedBuffer
    m_SharedShaderData.pSharedBuffer = m_pPointLightShader->setBuffer(m_pPointLightShader->getBufferId("SharedBuffer"));
    m_pSpotLightShader->setBuffer(m_pSpotLightShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);
    m_pAmbIllShader->setBuffer(m_pAmbIllShader->getBufferId("SharedBuffer"), m_SharedShaderData.pSharedBuffer);

    m_SharedShaderData.pSharedBuffer->getShaderVar("projParams", m_SharedShaderData.projParams);

    //LightBuffer
    m_PointLightData.pLightBuffer = m_pPointLightShader->setBuffer(m_pPointLightShader->getBufferId("LightBuffer"));
    m_SpotLightData.pLightBuffer = m_pSpotLightShader->setBuffer(m_pSpotLightShader->getBufferId("LightBuffer"));
    m_AmbIllLightData.pLightBuffer = m_pAmbIllShader->setBuffer(m_pAmbIllShader->getBufferId("LightBuffer"));
    m_AmbIllLightData.pPerFrameBuffer = m_pAmbIllShader->setBuffer(m_pAmbIllShader->getBufferId("PerFrameBuffer"));

    //----PL----------------------------------------------------------------------
    m_PointLightData.pLightBuffer->getShaderVar("light.position", m_PointLightData.position);
    m_PointLightData.pLightBuffer->getShaderVar("light.multiplier", m_PointLightData.multiplier);
    m_PointLightData.pLightBuffer->getShaderVar("light.color", m_PointLightData.color);
    m_PointLightData.pLightBuffer->getShaderVar("light.beginAttenuation", m_PointLightData.beginAttenuation);
    m_PointLightData.pLightBuffer->getShaderVar("light.endAttenuation", m_PointLightData.endAttenuation);
    m_PointLightData.colorIllMap = m_pPointLightShader->getShaderSamplerId("colorIllMap");
    m_PointLightData.normalMap = m_pPointLightShader->getShaderSamplerId("normalMap");
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
    m_SpotLightData.sgMap = m_pSpotLightShader->getShaderSamplerId("sgMap");
    m_SpotLightData.depthMap = m_pSpotLightShader->getShaderSamplerId("depthMap");
    m_SpotLightData.wvp = m_pSpotLightShader->getShaderVarId("mtxWVP");

    //----AL----------------------------------------------------------------------   
    m_AmbIllLightData.pLightBuffer->getShaderVar("ambLight.color", m_AmbIllLightData.ambColor);
    m_AmbIllLightData.pLightBuffer->getShaderVar("dirLight.color", m_AmbIllLightData.dirColor);
    m_AmbIllLightData.pLightBuffer->getShaderVar("dirLight.direction", m_AmbIllLightData.dirDirection);

    m_AmbIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight0", m_AmbIllLightData.mtxDirLight0);
    m_AmbIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight1", m_AmbIllLightData.mtxDirLight1);
    m_AmbIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight2", m_AmbIllLightData.mtxDirLight2);
    m_AmbIllLightData.pPerFrameBuffer->getShaderVar("mtxDirLight3", m_AmbIllLightData.mtxDirLight3);

    m_AmbIllLightData.colorIllMap  = m_pAmbIllShader->getShaderSamplerId("colorIllMap");
    m_AmbIllLightData.sgMap  = m_pAmbIllShader->getShaderSamplerId("sgMap");
    m_AmbIllLightData.normalMap  = m_pAmbIllShader->getShaderSamplerId("normalMap");
    m_AmbIllLightData.depthMap  = m_pAmbIllShader->getShaderSamplerId("depthMap");
    m_AmbIllLightData.colorRamp  = m_pAmbIllShader->getShaderSamplerId("colorRamp");
    m_pColorRampTex = CONTENT->asyncLoadTexture("shadingRamp.png");

    m_AmbIllLightData.shadowMap0 = m_pAmbIllShader->getShaderSamplerId("shadowMap0");
    m_AmbIllLightData.shadowMap1 = m_pAmbIllShader->getShaderSamplerId("shadowMap1");
    m_AmbIllLightData.shadowMap2 = m_pAmbIllShader->getShaderSamplerId("shadowMap2");
    m_AmbIllLightData.shadowMap3 = m_pAmbIllShader->getShaderSamplerId("shadowMap3");

    //----ToneMap-----------------------------------------------------------------
    initToneMapShader(folder, shaderLayout);


    //////////////////////////////////////////////////////////////////////////
    ///                         LOAD RENDER QUAD                           ///
    //////////////////////////////////////////////////////////////////////////
    m_pQuad = CONTENT->getFullscreenQuad();
}
void Deferred3DRenderer::initToneMapShader(const std::string& folder, const ShaderLayout& layout)
{
    std::set<std::string> toneMapDefines;
    if (m_Bloom)
        toneMapDefines.insert("BLOOM");

    if (m_bSSAO)
        toneMapDefines.insert("SSAO");

    delete m_pToneMapShader;
    m_pToneMapShader = NEW Shader();
    m_pToneMapShader->init(folder + "deferred/post/deferredPostShaderQuad.vert", folder + "deferred/post/HDRtoLDR.frag", layout, toneMapDefines);
    m_ToneMapShaderPos[0] = m_pToneMapShader->getShaderSamplerId("hdrMap");
    if (m_Bloom)
    {
        m_ToneMapShaderPos[1] = m_pToneMapShader->getShaderSamplerId("blur0");
        m_ToneMapShaderPos[2] = m_pToneMapShader->getShaderSamplerId("blur1");
        m_ToneMapShaderPos[3] = m_pToneMapShader->getShaderSamplerId("blur2");
        m_ToneMapShaderPos[4] = m_pToneMapShader->getShaderSamplerId("blur3");
    }
    m_ToneMapShaderPos[5] = m_pToneMapShader->getShaderSamplerId("lumMap");
    m_ToneMapShaderPos[6] = m_pToneMapShader->getShaderSamplerId("normalMap");
    m_ToneMapShaderPos[7] = m_pToneMapShader->getShaderSamplerId("depthMap");

    // SSAO

    if (m_bSSAO)
    {
        m_pRandomNormals = CONTENT->asyncLoadTexture("engine/random_normals.png");
        m_ShaderSSAOPos[0] = m_pToneMapShader->getShaderSamplerId("randomNormals");
        m_ShaderSSAOPos[1] = m_pToneMapShader->getShaderVarId("radius");
        m_ShaderSSAOPos[2] = m_pToneMapShader->getShaderVarId("intensity");
        m_ShaderSSAOPos[3] = m_pToneMapShader->getShaderVarId("scale");
        m_ShaderSSAOPos[4] = m_pToneMapShader->getShaderVarId("bias");
        //m_ShaderSSAOPos[5] = m_pToneMapShader->getShaderVarId("passes");
        //m_ShaderSSAOPos[6] = m_pToneMapShader->getShaderVarId("minIterations");
        //m_ShaderSSAOPos[7] = m_pToneMapShader->getShaderVarId("maxIterations");
        m_ShaderSSAOPos[8] = m_pToneMapShader->getShaderVarId("projParams");
        m_ShaderSSAOPos[9] = m_pToneMapShader->getShaderVarId("viewPortSize");
        //m_ShaderSSAOPos[10] = m_pToneMapShader->getShaderVarId("farZ");
    }

    m_SSAOSettings.radius = 0.1f;
    m_SSAOSettings.intensity = 2.0f;
    m_SSAOSettings.scale = 1.0f;
    m_SSAOSettings.bias = 0.03f;
    m_SSAOSettings.minIterations = 4;
    m_SSAOSettings.maxIterations = 8;
    m_SSAOSettings.passes = 1;
}

Deferred3DRenderer::~Deferred3DRenderer()
{
    glDeleteFramebuffers(1, &m_CollectionFboId);
    glDeleteFramebuffers(1, &m_RenderFboId);

    delete m_pPointLightShader;
    delete m_pSpotLightShader;
    delete m_pAmbIllShader;
    delete m_pToneMapShader;
    delete m_pLightManager;
    delete m_pBloom;
    delete m_pAutoExposure;
}
void Deferred3DRenderer::resized()
{
    glDeleteFramebuffers(1, &m_CollectionFboId);
    glDeleteFramebuffers(1, &m_RenderFboId);

    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    int width  = GRAPHICS->getScreenWidth(), 
        height = GRAPHICS->getScreenHeight();

    //Collection Textures
    glGenTextures(TEXTURES, m_TextureId);
    for (int i = 0; i < TEXTURES; ++i)
    {
        GL::heBindTexture2D(0, m_TextureId[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, TEXTURE_INTERNALFORMAT[i], 
            width, height,
            0, TEXTURE_FORMAT[i], GL_UNSIGNED_BYTE, 0);
        m_pTexture[i] = Texture2D::pointer(NEW Texture2D());
        m_pTexture[i]->init(m_TextureId[i], width, height, TEXTURE_FORMAT[i]);
    }

    //HDR Texture
    glGenTextures(1, &m_RenderTextureId);
    GL::heBindTexture2D(0, m_RenderTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 
        width, height,
        0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    m_pRenderTexture->init(m_RenderTextureId, width, height, GL_RGBA16F);

    if (m_Bloom)
        m_pBloom->resize();

    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////

    //FBO Collection
    glGenFramebuffers(1, &m_CollectionFboId);
    GL::heBindFbo(m_CollectionFboId);

    for (int i = 0; i < TEXTURES; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, TEXTURE_ATTACHMENT[i], GL_TEXTURE_2D, m_TextureId[i], 0);
    }
    err::checkFboStatus("deferred collection");

    //Fbo Render
    glGenFramebuffers(1, &m_RenderFboId);
    GL::heBindFbo(m_RenderFboId);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TextureId[3], 0);

    err::checkFboStatus("deferred render");

    GL::heBindFbo(0);
}

const VertexLayout& Deferred3DRenderer::getVertexLayoutLightVolume() 
{
    return s_VertexLayoutFullscreenQuad; 
}
void Deferred3DRenderer::begin()
{
    GL::heBindFbo(m_CollectionFboId);
    GL::heSetDepthWrite(true);
    GL::heSetDepthRead(true);
    const static GLenum buffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, buffers);
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Deferred3DRenderer::end()
{    
    PROFILER_BEGIN("Deferred3DRenderer::end");
    //#if _DEBUG
    //glDisable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glLineWidth(4.0f);
    //std::for_each(m_pLightManager->getPointLights().cbegin(), m_pLightManager->getPointLights().cend(), [&](const PointLight::pointer& pLight)
    //{
    //    pLight->getMaterial().begin(pLight.get(), CAMERAMANAGER->getActiveCamera());
    //    GRAPHICS->draw(pLight->getModel());
    //    pLight->getMaterial().end();
    //});
    //std::for_each(m_pLightManager->getSpotLights().cbegin(), m_pLightManager->getSpotLights().cend(), [&](const SpotLight::pointer& pLight)
    //{
    //    pLight->getMaterial().begin(pLight.get(), CAMERAMANAGER->getActiveCamera());
    //    GRAPHICS->draw(pLight->getModel());
    //    pLight->getMaterial().end();      
    //});
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glEnable(GL_CULL_FACE);
    //#endif

    GL::heBindFbo(m_RenderFboId);
    const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 1.0f));    
    glClear(GL_COLOR_BUFFER_BIT);
    //////////////////////////////////////////////////////////////////////////
    ///                             Pass 1                                 ///
    //////////////////////////////////////////////////////////////////////////

    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_One, BlendFunc_One);
    GL::heSetDepthRead(false);

    GL::heSetCullFace(true);
    {
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
    }
    GL::heSetCullFace(false);

    m_pAmbIllShader->bind();
    postAmbIllLight();

    GL::heBindFbo(0);

    GL::heBindTexture2D(0, m_pRenderTexture->getID());
    glGenerateMipmap(GL_TEXTURE_2D);

    calculateExposure();
    GL::heBlendEnabled(false);
    if (m_Bloom)
        m_pBloom->render(m_pRenderTexture, m_pAutoExposure->getLuminanceMap());
    //////////////////////////////////////////////////////////////////////////
    ///                             Pass 2                                 ///
    //////////////////////////////////////////////////////////////////////////
    GL::heBindFbo(0);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);
    const static GLenum buffers2[1] = { GL_BACK_LEFT };
    glDrawBuffers(1, buffers2);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Tonemap
    m_pToneMapShader->bind();
    postToneMap();

    GL::heBindVao(0);
    GL::heScissorEnabled(false);
    GL::heSetDepthRead(true);

    if (m_ShowDebugTextures)
    {
        GL::heBlendEnabled(false);
        GUI->drawTexture2D(m_pTexture[0], vec2(12 * 1 + 256 * 0, 12), vec2(256, 144));
        GUI->drawTexture2D(m_pTexture[1], vec2(12 * 2 + 256 * 1, 12), vec2(256, 144));
        GUI->drawTexture2D(m_pTexture[2], vec2(12 * 3 + 256 * 2, 12), vec2(256, 144));
        GUI->drawTexture2D(m_pTexture[3], vec2(12 * 4 + 256 * 3, 12), vec2(256, 144));
        if (m_Bloom)
        {
            GUI->drawTexture2D(m_pBloom->getBloom(0), vec2(12 * 1 + 256 * 0, 12*2+144), vec2(256, 144));
            GUI->drawTexture2D(m_pBloom->getBloom(1), vec2(12 * 2 + 256 * 1, 12*2+144), vec2(256, 144));
            GUI->drawTexture2D(m_pBloom->getBloom(2), vec2(12 * 3 + 256 * 2, 12*2+144), vec2(256, 144));
            GUI->drawTexture2D(m_pBloom->getBloom(3), vec2(12 * 4 + 256 * 3, 12*2+144), vec2(256, 144));
        }
        if (getLightManager()->getDirectionalLight()->getShadowMap(0) != nullptr)
            GUI->drawTexture2D(getLightManager()->getDirectionalLight()->getShadowMap(0), vec2(12 * 1 + 256 * 0, 12*3 + 144*2), vec2(256, 256));
        if (getLightManager()->getDirectionalLight()->getShadowMap(1) != nullptr)
            GUI->drawTexture2D(getLightManager()->getDirectionalLight()->getShadowMap(1), vec2(12 * 2 + 256 * 1, 12*3 + 144*2), vec2(256, 256));
        if (getLightManager()->getDirectionalLight()->getShadowMap(2) != nullptr)
            GUI->drawTexture2D(getLightManager()->getDirectionalLight()->getShadowMap(2), vec2(12 * 3 + 256 * 2, 12*3 + 144*2), vec2(256, 256));
        if (getLightManager()->getDirectionalLight()->getShadowMap(3) != nullptr)
            GUI->drawTexture2D(getLightManager()->getDirectionalLight()->getShadowMap(3), vec2(12 * 4 + 256 * 3, 12*3 + 144*2), vec2(256, 256));
        GL::heBlendEnabled(true);
    }
    PROFILER_END("Deferred3DRenderer::end");
}
void Deferred3DRenderer::postAmbIllLight()
{
    const AmbientLight::pointer& pAmbLight(m_pLightManager->getAmbientLight());
    const DirectionalLight::pointer& pDirLight(m_pLightManager->getDirectionalLight());

    m_AmbIllLightData.ambColor = vec4(pAmbLight->color, pAmbLight->multiplier);
    m_AmbIllLightData.dirColor = vec4(pDirLight->getColor(), pDirLight->getMultiplier());
    m_AmbIllLightData.dirDirection = normalize((CAMERAMANAGER->getActiveCamera()->getView() * vec4(pDirLight->getDirection(), 0.0f)).xyz());

    m_AmbIllLightData.pLightBuffer->setShaderVar(m_AmbIllLightData.ambColor);
    m_AmbIllLightData.pLightBuffer->setShaderVar(m_AmbIllLightData.dirColor);
    m_AmbIllLightData.pLightBuffer->setShaderVar(m_AmbIllLightData.dirDirection);

    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.colorIllMap, m_pTexture[0]);
    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.sgMap,       m_pTexture[1]);
    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.normalMap,   m_pTexture[2]);
    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.depthMap,    m_pTexture[3]);
    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.colorRamp,   m_pColorRampTex);

    m_AmbIllLightData.mtxDirLight0 = m_pLightManager->getDirectionalLight()->getShadowMatrix(0);
    m_AmbIllLightData.mtxDirLight1 = m_pLightManager->getDirectionalLight()->getShadowMatrix(1);
    m_AmbIllLightData.mtxDirLight2 = m_pLightManager->getDirectionalLight()->getShadowMatrix(2);
    m_AmbIllLightData.mtxDirLight3 = m_pLightManager->getDirectionalLight()->getShadowMatrix(3);

    m_AmbIllLightData.pPerFrameBuffer->setShaderVar(m_AmbIllLightData.mtxDirLight0);
    m_AmbIllLightData.pPerFrameBuffer->setShaderVar(m_AmbIllLightData.mtxDirLight1);
    m_AmbIllLightData.pPerFrameBuffer->setShaderVar(m_AmbIllLightData.mtxDirLight2);
    m_AmbIllLightData.pPerFrameBuffer->setShaderVar(m_AmbIllLightData.mtxDirLight3);

    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.shadowMap0, m_pLightManager->getDirectionalLight()->getShadowMap(0));
    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.shadowMap1, m_pLightManager->getDirectionalLight()->getShadowMap(1));
    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.shadowMap2, m_pLightManager->getDirectionalLight()->getShadowMap(2));
    m_pAmbIllShader->setShaderVar(m_AmbIllLightData.shadowMap3, m_pLightManager->getDirectionalLight()->getShadowMap(3));

    GRAPHICS->draw(m_pQuad);
}
void Deferred3DRenderer::postPointLights()
{
    m_pPointLightShader->setShaderVar(m_PointLightData.colorIllMap, m_pTexture[0]);
    m_pPointLightShader->setShaderVar(m_PointLightData.sgMap,       m_pTexture[1]);
    m_pPointLightShader->setShaderVar(m_PointLightData.normalMap,   m_pTexture[2]);
    m_pPointLightShader->setShaderVar(m_PointLightData.depthMap,    m_pTexture[3]);

    const std::vector<PointLight::pointer>& lights(m_pLightManager->getPointLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const PointLight::pointer& pLight)
    {
        /*if (lengthSqr(pLight->getPosition() - CAMERAMANAGER->getActiveCamera()->getPosition()) + pLight->getEndAttenuation() * pLight->getEndAttenuation() 
            < m_Settings.getFogEnd() * m_Settings.getFogEnd())
        {*/
            if ( !(dot(normalize(pLight->getPosition() - CAMERAMANAGER->getActiveCamera()->getPosition()), CAMERAMANAGER->getActiveCamera()->getLook()) < 0 && 
                   length(pLight->getPosition() - CAMERAMANAGER->getActiveCamera()->getPosition()) > pLight->getEndAttenuation())) 
            {
                //RectI scissor(pLight->getScissor(pCamera));
                m_PointLightData.position = CAMERAMANAGER->getActiveCamera()->getView() * pLight->getPosition();
                m_PointLightData.multiplier = pLight->getMultiplier();
                m_PointLightData.color = pLight->getColor();
                m_PointLightData.beginAttenuation = pLight->getBeginAttenuation();
                m_PointLightData.endAttenuation = pLight->getEndAttenuation();

                m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.position);
                m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.multiplier);
                m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.color);
                m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.beginAttenuation);
                m_PointLightData.pLightBuffer->setShaderVar(m_PointLightData.endAttenuation);

                m_pPointLightShader->setShaderVar(m_PointLightData.wvp, CAMERAMANAGER->getActiveCamera()->getViewProjection() * pLight->getWorldMatrix());

                if (pLight->getLightVolume()->isLoaded())
                    GRAPHICS->draw(pLight->getLightVolume());
                //draw(m_pModel);
            }
        //}
    });
}
void Deferred3DRenderer::postSpotLights()
{
    m_pSpotLightShader->setShaderVar(m_SpotLightData.colorIllMap, m_pTexture[0]);
    m_pSpotLightShader->setShaderVar(m_SpotLightData.sgMap,       m_pTexture[1]);
    m_pSpotLightShader->setShaderVar(m_SpotLightData.normalMap,   m_pTexture[2]);
    m_pSpotLightShader->setShaderVar(m_SpotLightData.depthMap,    m_pTexture[3]);

    const std::vector<SpotLight::pointer>& lights(m_pLightManager->getSpotLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const SpotLight::pointer& pLight)
    {
        //RectI scissor(pLight->getScissor(pCamera));
        m_SpotLightData.position = CAMERAMANAGER->getActiveCamera()->getView() * pLight->getPosition();
        m_SpotLightData.multiplier = pLight->getMultiplier();
        m_SpotLightData.direction = normalize((CAMERAMANAGER->getActiveCamera()->getView() * vec4(pLight->getDirection(), 0)).xyz());
        m_SpotLightData.beginAttenuation = pLight->getBeginAttenuation();
        m_SpotLightData.color = pLight->getColor();
        m_SpotLightData.endAttenuation = pLight->getEndAttenuation();
        m_SpotLightData.cosCutOff = pLight->getCosCutoff();

        m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.position);
        m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.multiplier);
        m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.direction);
        m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.beginAttenuation);
        m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.beginAttenuation);
        m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.endAttenuation);
        m_SpotLightData.pLightBuffer->setShaderVar(m_SpotLightData.cosCutOff);

        m_pSpotLightShader->setShaderVar(m_SpotLightData.wvp, CAMERAMANAGER->getActiveCamera()->getViewProjection() * pLight->getWorldMatrix());

        if (pLight->getLightVolume()->isLoaded())
            GRAPHICS->draw(pLight->getLightVolume());
    });
}

void Deferred3DRenderer::postToneMap()
{
    m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[0], m_pRenderTexture);
    if (m_Bloom)
    {
        m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[1], m_pBloom->getBloom(0));
        m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[2], m_pBloom->getBloom(1));
        m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[3], m_pBloom->getBloom(2));
        m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[4], m_pBloom->getBloom(3));
    }
    m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[5], m_pAutoExposure->getLuminanceMap());
    m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[6], m_pTexture[2]);
    m_pToneMapShader->setShaderVar(m_ToneMapShaderPos[7], m_pTexture[3]);

    if (m_bSSAO)
    {
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[0], m_pRandomNormals);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[1], m_SSAOSettings.radius);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[2], m_SSAOSettings.intensity);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[3], m_SSAOSettings.scale);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[4], m_SSAOSettings.bias);
        //m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[5], (int)m_SSAOSettings.passes);
        //m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[6], m_SSAOSettings.minIterations);
        //m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[7], (int)m_SSAOSettings.maxIterations);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[8], vec4(CAMERAMANAGER->getActiveCamera()->getProjection()(0, 0),
                                                                CAMERAMANAGER->getActiveCamera()->getProjection()(1, 1),
                                                                CAMERAMANAGER->getActiveCamera()->getProjection()(2, 2),
                                                                CAMERAMANAGER->getActiveCamera()->getProjection()(2, 3)));
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[9], vec2((float)GRAPHICS->getScreenRect().width, (float)GRAPHICS->getScreenRect().height));
        //m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[10], CAMERAMANAGER->getActiveCamera()->getFarClip());
    }

    GRAPHICS->draw(m_pQuad);
}

LightManager* Deferred3DRenderer::getLightManager() const
{
    return m_pLightManager;
}

void Deferred3DRenderer::calculateExposure()
{
    m_pAutoExposure->calculate(m_pRenderTexture);
}

const Texture2D::pointer& Deferred3DRenderer::getDepthTexture() const
{
    return m_pTexture[3];
}

void Deferred3DRenderer::enableSSAO(bool enable)
{
    m_bSSAO = enable;
}

void Deferred3DRenderer::setSSAOSettings(const SSAOSettings& settings)
{
    m_SSAOSettings = settings;
}

} } //end namespace