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
#include "StdAfx.h" 

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
            m_bSSAO(settings.getSSAOEnabled())
{
    //CONSOLE->registerValue(&m_Exposure, "exposure");
    //CONSOLE->registerValue(&m_Gamma, "gamma");
    CONSOLE->registerValue(&m_ShowDebugTextures, "debugTex");
    //CONSOLE->registerValue(&m_Bloom, "bloom");


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

    for (int i = 0; i < LIGHTVOLUME_SHADERS; ++i)
    {
        m_pPostLightVolumeShader[i] = NEW Shader();
    }

    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    m_pPostLightVolumeShader[LightVolumeType_PointLight]->init(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostPLShader.frag", shaderLayout);
    m_pPostLightVolumeShader[LightVolumeType_SpotLight]->init(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostSLShader.frag", shaderLayout);
    
    for (int i = 0; i < LIGHTVOLUME_SHADERS; ++i)
    {
        m_ShaderLVColMapPos[i]    = m_pPostLightVolumeShader[i]->getShaderSamplerId("colorIllMap");
        m_ShaderLVWVP[i]          = m_pPostLightVolumeShader[i]->getShaderVarId("mtxWVP");
        m_ShaderLVNormalMapPos[i] = m_pPostLightVolumeShader[i]->getShaderSamplerId("normalMap");
        m_ShaderLVSGMapPos[i]     = m_pPostLightVolumeShader[i]->getShaderSamplerId("sgMap");
        m_ShaderLVDepthMapPos[i]  = m_pPostLightVolumeShader[i]->getShaderSamplerId("depthMap");
        m_ShaderLVProjParams[i]   = m_pPostLightVolumeShader[i]->getShaderVarId("projParams");
    }
    //----PL----------------------------------------------------------------------
    m_ShaderLVPLPos[0] = m_pPostLightVolumeShader[LightVolumeType_PointLight]->getShaderVarId("light.position");
    m_ShaderLVPLPos[1] = m_pPostLightVolumeShader[LightVolumeType_PointLight]->getShaderVarId("light.multiplier");
    m_ShaderLVPLPos[2] = m_pPostLightVolumeShader[LightVolumeType_PointLight]->getShaderVarId("light.color");
    m_ShaderLVPLPos[3] = m_pPostLightVolumeShader[LightVolumeType_PointLight]->getShaderVarId("light.beginAttenuation");
    m_ShaderLVPLPos[4] = m_pPostLightVolumeShader[LightVolumeType_PointLight]->getShaderVarId("light.endAttenuation");
    //----SL----------------------------------------------------------------------
    m_ShaderLVSLPos[0] = m_pPostLightVolumeShader[LightVolumeType_SpotLight]->getShaderVarId("light.position");
    m_ShaderLVSLPos[1] = m_pPostLightVolumeShader[LightVolumeType_SpotLight]->getShaderVarId("light.multiplier");
    m_ShaderLVSLPos[2] = m_pPostLightVolumeShader[LightVolumeType_SpotLight]->getShaderVarId("light.direction");
    m_ShaderLVSLPos[3] = m_pPostLightVolumeShader[LightVolumeType_SpotLight]->getShaderVarId("light.beginAttenuation");
    m_ShaderLVSLPos[4] = m_pPostLightVolumeShader[LightVolumeType_SpotLight]->getShaderVarId("light.color");
    m_ShaderLVSLPos[5] = m_pPostLightVolumeShader[LightVolumeType_SpotLight]->getShaderVarId("light.endAttenuation");
    m_ShaderLVSLPos[6] = m_pPostLightVolumeShader[LightVolumeType_SpotLight]->getShaderVarId("light.cosCutoff");

    //----AL----------------------------------------------------------------------
    m_pAmbIllShader = NEW Shader();
    m_pAmbIllShader->init(folder + "deferred/post/deferredPostShaderQuad.vert", folder + "deferred/post/deferredPostAmbientIllShader.frag", shaderLayout);
    m_ShaderAmbIllPos[0]  = m_pAmbIllShader->getShaderVarId("ambLight.color");
    m_ShaderAmbIllPos[1]  = m_pAmbIllShader->getShaderVarId("dirLight.color");
    m_ShaderAmbIllPos[2]  = m_pAmbIllShader->getShaderVarId("dirLight.direction");
    m_ShaderAmbIllPos[3]  = m_pAmbIllShader->getShaderVarId("projParams");
    m_ShaderAmbIllPos[4]  = m_pAmbIllShader->getShaderSamplerId("colorIllMap");
    m_ShaderAmbIllPos[5]  = m_pAmbIllShader->getShaderSamplerId("sgMap");
    m_ShaderAmbIllPos[6]  = m_pAmbIllShader->getShaderSamplerId("normalMap");
    m_ShaderAmbIllPos[7]  = m_pAmbIllShader->getShaderSamplerId("depthMap");
    m_ShaderAmbIllPos[8]  = m_pAmbIllShader->getShaderVarId("mtxDirLight0");
    m_ShaderAmbIllPos[9]  = m_pAmbIllShader->getShaderVarId("mtxDirLight1");
    m_ShaderAmbIllPos[10] = m_pAmbIllShader->getShaderVarId("mtxDirLight2");
    m_ShaderAmbIllPos[11] = m_pAmbIllShader->getShaderVarId("mtxDirLight3");
    m_ShaderAmbIllPos[12] = m_pAmbIllShader->getShaderSamplerId("shadowMap0");
    m_ShaderAmbIllPos[13] = m_pAmbIllShader->getShaderSamplerId("shadowMap1");
    m_ShaderAmbIllPos[14] = m_pAmbIllShader->getShaderSamplerId("shadowMap2");
    m_ShaderAmbIllPos[15] = m_pAmbIllShader->getShaderSamplerId("shadowMap3");
    //m_ShaderAmbIllPos[16] = m_pAmbIllShader->getShaderVarId("texelSize");

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
        m_pRandomNormals = CONTENT->asyncLoadTexture("random_normals.png");
        m_ShaderSSAOPos[0] = m_pToneMapShader->getShaderSamplerId("randomNormals");
        m_ShaderSSAOPos[1] = m_pToneMapShader->getShaderVarId("radius");
        m_ShaderSSAOPos[2] = m_pToneMapShader->getShaderVarId("intensity");
        m_ShaderSSAOPos[3] = m_pToneMapShader->getShaderVarId("scale");
        m_ShaderSSAOPos[4] = m_pToneMapShader->getShaderVarId("bias");
        m_ShaderSSAOPos[5] = m_pToneMapShader->getShaderVarId("passes");
        m_ShaderSSAOPos[6] = m_pToneMapShader->getShaderVarId("minIterations");
        m_ShaderSSAOPos[7] = m_pToneMapShader->getShaderVarId("maxIterations");
        m_ShaderSSAOPos[8] = m_pToneMapShader->getShaderVarId("projParams");
        m_ShaderSSAOPos[9] = m_pToneMapShader->getShaderVarId("viewPortSize");
        m_ShaderSSAOPos[10] = m_pToneMapShader->getShaderVarId("farZ");
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

    for (int i = 0; i < LIGHTVOLUME_SHADERS; ++i)
    {
        delete m_pPostLightVolumeShader[i];
    }
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
void Deferred3DRenderer::begin(const Camera* pCamera)
{
    m_pCamera = pCamera;
    GL::heBindFbo(m_CollectionFboId);
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
    //    pLight->getMaterial().begin(pLight.get(), m_pCamera);
    //    GRAPHICS->draw(pLight->getModel());
    //    pLight->getMaterial().end();
    //});
    //std::for_each(m_pLightManager->getSpotLights().cbegin(), m_pLightManager->getSpotLights().cend(), [&](const SpotLight::pointer& pLight)
    //{
    //    pLight->getMaterial().begin(pLight.get(), m_pCamera);
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
    for (int i = 0; i < LIGHTVOLUME_SHADERS; ++i)
    {
        m_pPostLightVolumeShader[i]->bind();
        
        m_pPostLightVolumeShader[i]->setShaderVar(m_ShaderLVColMapPos[i], m_pTexture[0]);
        m_pPostLightVolumeShader[i]->setShaderVar(m_ShaderLVSGMapPos[i], m_pTexture[1]);
        m_pPostLightVolumeShader[i]->setShaderVar(m_ShaderLVNormalMapPos[i], m_pTexture[2]);
        m_pPostLightVolumeShader[i]->setShaderVar(m_ShaderLVDepthMapPos[i], m_pTexture[3]);
        m_pPostLightVolumeShader[i]->setShaderVar(m_ShaderLVProjParams[i], 
            vec4(m_pCamera->getProjection()(0, 0),
                    m_pCamera->getProjection()(1, 1),
                    m_pCamera->getProjection()(2, 2),
                    m_pCamera->getProjection()(2, 3)));
    
        switch (i)
        {
            case LightVolumeType_PointLight: postPointLights(); break;
            case LightVolumeType_SpotLight: postSpotLights(); break;
            default: ASSERT("unkown lighttype"); break;
        }
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
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[0], vec4(pAmbLight->color, pAmbLight->multiplier));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[1], vec4(pDirLight->getColor(), pDirLight->getMultiplier()));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[2], normalize((m_pCamera->getView() * vec4(pDirLight->getDirection(), 0.0f)).xyz()));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[3], vec4(m_pCamera->getProjection()(0, 0),
                                                             m_pCamera->getProjection()(1, 1),
                                                             m_pCamera->getProjection()(2, 2),
                                                             m_pCamera->getProjection()(2, 3)));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[4], m_pTexture[0]);
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[5], m_pTexture[1]);
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[6], m_pTexture[2]);
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[7], m_pTexture[3]);

    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[8], m_pLightManager->getDirectionalLight()->getShadowMatrix(0));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[9], m_pLightManager->getDirectionalLight()->getShadowMatrix(1));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[10], m_pLightManager->getDirectionalLight()->getShadowMatrix(2));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[11], m_pLightManager->getDirectionalLight()->getShadowMatrix(3));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[12], m_pLightManager->getDirectionalLight()->getShadowMap(0));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[13], m_pLightManager->getDirectionalLight()->getShadowMap(1));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[14], m_pLightManager->getDirectionalLight()->getShadowMap(2));
    m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[15], m_pLightManager->getDirectionalLight()->getShadowMap(3));
    //m_pAmbIllShader->setShaderVar(m_ShaderAmbIllPos[16], vec2(1.0f / m_pLightManager->getDirectionalLight()->getShadowMap(0)->getWidth(),
    //                                                          1.0f / m_pLightManager->getDirectionalLight()->getShadowMap(0)->getHeight()));

    GRAPHICS->draw(m_pQuad);
}
void Deferred3DRenderer::postPointLights()
{
    const std::vector<PointLight::pointer>& lights(m_pLightManager->getPointLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const PointLight::pointer& pLight)
    {
        /*if (lengthSqr(pLight->getPosition() - m_pCamera->getPosition()) + pLight->getEndAttenuation() * pLight->getEndAttenuation() 
            < m_Settings.getFogEnd() * m_Settings.getFogEnd())
        {*/
            if ( !(dot(normalize(pLight->getPosition() - m_pCamera->getPosition()), m_pCamera->getLook()) < 0 && 
                   length(pLight->getPosition() - m_pCamera->getPosition()) > pLight->getEndAttenuation())) 
            {
                //RectI scissor(pLight->getScissor(pCamera));
                m_pPostLightVolumeShader[LightVolumeType_PointLight]->setShaderVar(m_ShaderLVPLPos[0], m_pCamera->getView() * pLight->getPosition());
                m_pPostLightVolumeShader[LightVolumeType_PointLight]->setShaderVar(m_ShaderLVPLPos[1], pLight->getMultiplier());
                m_pPostLightVolumeShader[LightVolumeType_PointLight]->setShaderVar(m_ShaderLVPLPos[2], pLight->getColor());
                m_pPostLightVolumeShader[LightVolumeType_PointLight]->setShaderVar(m_ShaderLVPLPos[3], pLight->getBeginAttenuation());
                m_pPostLightVolumeShader[LightVolumeType_PointLight]->setShaderVar(m_ShaderLVPLPos[4], pLight->getEndAttenuation());
                m_pPostLightVolumeShader[LightVolumeType_PointLight]->setShaderVar(m_ShaderLVWVP[LightVolumeType_PointLight], m_pCamera->getViewProjection() * pLight->getWorldMatrix());
                GRAPHICS->draw(pLight->getLightVolume());
                //draw(m_pModel);
            }
        //}
    });
}
void Deferred3DRenderer::postSpotLights()
{
    const std::vector<SpotLight::pointer>& lights(m_pLightManager->getSpotLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const SpotLight::pointer& pLight)
    {
        //RectI scissor(pLight->getScissor(pCamera));
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVSLPos[0], m_pCamera->getView() * pLight->getPosition());
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVSLPos[1], pLight->getMultiplier());
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVSLPos[2], normalize((m_pCamera->getView() * vec4(pLight->getDirection(), 0)).xyz()));
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVSLPos[3], pLight->getBeginAttenuation());
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVSLPos[4], pLight->getColor());
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVSLPos[5], pLight->getEndAttenuation());
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVSLPos[6], pLight->getCosCutoff());
        m_pPostLightVolumeShader[LightVolumeType_SpotLight]->setShaderVar(m_ShaderLVWVP[LightVolumeType_SpotLight], m_pCamera->getViewProjection() * pLight->getWorldMatrix());      
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
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[5], m_SSAOSettings.passes);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[6], m_SSAOSettings.minIterations);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[7], m_SSAOSettings.maxIterations);
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[8], vec4(m_pCamera->getProjection()(0, 0),
                                                                m_pCamera->getProjection()(1, 1),
                                                                m_pCamera->getProjection()(2, 2),
                                                                m_pCamera->getProjection()(2, 3)));
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[9], vec2((float)GRAPHICS->getScreenRect().width, (float)GRAPHICS->getScreenRect().height));
        m_pToneMapShader->setShaderVar(m_ShaderSSAOPos[10], m_pCamera->getFarClip());
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