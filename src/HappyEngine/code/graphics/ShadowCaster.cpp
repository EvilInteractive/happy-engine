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
//Created: 22/10/2011
#include "HappyPCH.h" 

#include "ShadowCaster.h"

#include "GraphicsEngine.h"
#include "InstancingManager.h"
#include "ContentManager.h"
#include "Happy2DRenderer.h"
#include "CameraManager.h"
#include "Camera.h"
#include "LightManager.h"

namespace he {
namespace gfx {

ShadowCaster::ShadowCaster(): m_ShowShadowDebug(false), m_ShadowSize(0), m_pQuad(nullptr)
{
    CONSOLE->registerVar(&m_ShowShadowDebug, "b_shadowtex");
    for (int i = 0; i < COUNT; ++i)
    {
        m_pShadowTexture[i] = nullptr;
    }
}


ShadowCaster::~ShadowCaster()
{
    for (int i = 0; i < COUNT; ++i)
    {
        if (m_pShadowTexture[i] != nullptr)
            m_pShadowTexture[i]->release();
    }
    if (m_pQuad != nullptr)
        m_pQuad->release();
    glDeleteRenderbuffers(1, &m_DepthRenderbuff);
    glDeleteFramebuffers(1, &m_FboId);
}

void ShadowCaster::init(const RenderSettings& settings)
{
    m_ShadowSize = 512 * settings.shadowMult;
    HE_ASSERT(m_ShadowSize <= 2048, "shadowmap size must be <= 2048");
    //////////////////////////////////////////////////////////////////////////
    ///                             Textures                               ///
    //////////////////////////////////////////////////////////////////////////
    uint texId[COUNT];
    glGenTextures(COUNT, texId);

    for (int i = 0; i < COUNT; ++i)
    {
        GL::heBindTexture2D(0, texId[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 
            m_ShadowSize, m_ShadowSize, 
            0, GL_RG, GL_FLOAT, 0);
        if (m_pShadowTexture[i] != nullptr)
            m_pShadowTexture[i]->release();
        ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
        m_pShadowTexture[i] = ResourceFactory<Texture2D>::getInstance()->get(handle);
        m_pShadowTexture[i]->setName("ShadowCaster::m_pShadowTexture[i]");
        m_pShadowTexture[i]->init(texId[i], m_ShadowSize, m_ShadowSize, GL_RG16F);
    }
    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////
    glGenRenderbuffers(1, &m_DepthRenderbuff);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderbuff);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_ShadowSize, m_ShadowSize);

    glGenFramebuffers(1, &m_FboId);
    GL::heBindFbo(m_FboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId[0], 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderbuff);
    err::checkFboStatus("shadow");

    GL::heBindFbo(0);


    //////////////////////////////////////////////////////////////////////////
    ///                             Shaders                                ///
    //////////////////////////////////////////////////////////////////////////
    Shader::pointer pShadowShaderSingle(NEW Shader());
    Shader::pointer pShadowShaderSkinned(NEW Shader());
    Shader::pointer pShadowShaderInstanced(NEW Shader());

    // Single
    ShaderLayout shaderSingleLayout;
    shaderSingleLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    BufferLayout vertexSingleLayout, noInstancingLayout;
    vertexSingleLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));

    std::vector<std::string> outputs;
    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    pShadowShaderSingle->initFromFile(folder + "shadow/preShadowShader.vert", 
                                      folder + "shadow/preShadowShader.frag", 
                                      shaderSingleLayout, outputs);
    
    // Skinned
    ShaderLayout shaderSkinnedLayout;
    shaderSkinnedLayout.addElement(ShaderLayoutElement(0, "inPosition"));
    shaderSkinnedLayout.addElement(ShaderLayoutElement(1, "inBoneId"));
    shaderSkinnedLayout.addElement(ShaderLayoutElement(2, "inBoneWeight"));

    BufferLayout vertexSkinnedLayout;
    vertexSkinnedLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position,    12, 0));
    vertexSkinnedLayout.addElement(BufferElement(1, BufferElement::Type_Vec4, BufferElement::Usage_BoneIDs,     16, 12));
    vertexSkinnedLayout.addElement(BufferElement(2, BufferElement::Type_Vec4, BufferElement::Usage_BoneWeights, 16, 12+16));

    pShadowShaderSkinned->initFromFile(folder + "shadow/preShadowShaderSkinned.vert", 
                                       folder + "shadow/preShadowShader.frag", 
                                       shaderSkinnedLayout, outputs);

    // Instanced
    pShadowShaderInstanced->initFromFile(folder + "shadow/preShadowShaderInstanced.vert", 
                                         folder + "shadow/preShadowShader.frag", 
                                         shaderSingleLayout, outputs);

    ShaderLayout blurLayout;
    blurLayout.addElement(ShaderLayoutElement(0, "inPosition"));
    for (int pass(0); pass < 2; ++pass)
    {
        m_pShadowBlurShaderPass[pass] = Shader::pointer(NEW Shader());

        std::set<std::string> definePass;
        if (pass == 0)
            definePass.insert("PASS1");
        else
            definePass.insert("PASS2");

        m_pShadowBlurShaderPass[pass]->initFromFile(folder + "shared/postShaderQuad.vert", 
                                                    folder + "shadow/shadowBlur.frag", blurLayout, definePass);
        m_BlurShaderTexPosPass[pass] = m_pShadowBlurShaderPass[pass]->getShaderSamplerId("map");
    }
    BufferLayout instancingLayout;
    instancingLayout.addElement(BufferElement(0, BufferElement::Type_Vec4, BufferElement::Usage_Instancing, sizeof(vec4), sizeof(vec4) * 0));
    instancingLayout.addElement(BufferElement(1, BufferElement::Type_Vec4, BufferElement::Usage_Instancing, sizeof(vec4), sizeof(vec4) * 1));
    instancingLayout.addElement(BufferElement(2, BufferElement::Type_Vec4, BufferElement::Usage_Instancing, sizeof(vec4), sizeof(vec4) * 2));
    instancingLayout.addElement(BufferElement(3, BufferElement::Type_Vec4, BufferElement::Usage_Instancing, sizeof(vec4), sizeof(vec4) * 3));

    //////////////////////////////////////////////////////////////////////////
    ///                             Materials                              ///
    //////////////////////////////////////////////////////////////////////////
    m_MatSingle.setShader(pShadowShaderSingle, vertexSingleLayout, noInstancingLayout);
    m_MatSingle.addVar(ShaderVar::pointer(NEW ShaderGlobalVar(pShadowShaderSingle->getShaderVarId("matWVP"), "matWVP", ShaderVarType_WorldViewProjection)));
    
    m_MatSkinned.setShader(pShadowShaderSkinned, vertexSkinnedLayout, noInstancingLayout);
    m_MatSkinned.addVar(ShaderVar::pointer(NEW ShaderGlobalVar(pShadowShaderSkinned->getShaderVarId("matWVP"), "matWVP", ShaderVarType_WorldViewProjection)));
    m_MatSkinned.addVar(ShaderVar::pointer(NEW ShaderGlobalVar(pShadowShaderSkinned->getShaderVarId("matBones"), "matBones", ShaderVarType_BoneTransforms)));
    
    m_MatInstanced.setShader(pShadowShaderInstanced, vertexSingleLayout, instancingLayout);
    m_MatInstanced.addVar(ShaderVar::pointer(NEW ShaderGlobalVar(pShadowShaderInstanced->getShaderVarId("matVP"), "matVP", ShaderVarType_ViewProjection)));

    m_pQuad = CONTENT->getFullscreenQuad();
}
void ShadowCaster::setSettings( const RenderSettings& settings )
{
    if (m_ShadowSize != settings.shadowMult * 512)
    {
        m_ShadowSize = 512 * settings.shadowMult;
        //////////////////////////////////////////////////////////////////////////
        ///                             Clean                                  ///
        //////////////////////////////////////////////////////////////////////////
        if (m_DepthRenderbuff != UINT_MAX)
            glDeleteRenderbuffers(1, &m_DepthRenderbuff);
        if (m_FboId != UINT_MAX)
            glDeleteFramebuffers(1, &m_FboId);

        init(settings);
    }
}

mat44 getProjection(const mat44& mtxShadowView, float nearClip, float farClip)
{
    const Camera& camera(*CAMERAMANAGER->getActiveCamera());

    float wFar = farClip * tan(camera.getFov()),          //half width
          hFar = wFar / camera.getAspectRatio();          //half height
    float wNear = nearClip * tan(camera.getFov()),        //half width
          hNear = wNear / camera.getAspectRatio();        //half height

    std::vector<vec3> frustumPoints;
    frustumPoints.reserve(8);
    //Far plane
    frustumPoints.push_back(camera.getLook() * farClip + camera.getUp() * hFar - camera.getRight() * wFar);
    frustumPoints.push_back(camera.getLook() * farClip + camera.getUp() * hFar + camera.getRight() * wFar);
    frustumPoints.push_back(camera.getLook() * farClip - camera.getUp() * hFar - camera.getRight() * wFar);
    frustumPoints.push_back(camera.getLook() * farClip - camera.getUp() * hFar + camera.getRight() * wFar);
    //Near plane
    frustumPoints.push_back(camera.getLook() * nearClip + camera.getUp() * hNear - camera.getRight() * wNear);
    frustumPoints.push_back(camera.getLook() * nearClip + camera.getUp() * hNear + camera.getRight() * wNear);
    frustumPoints.push_back(camera.getLook() * nearClip - camera.getUp() * hNear - camera.getRight() * wNear);
    frustumPoints.push_back(camera.getLook() * nearClip - camera.getUp() * hNear + camera.getRight() * wNear);

    vec3 minP(FLT_MAX, FLT_MAX, FLT_MAX), maxP(FLT_MIN, FLT_MIN, FLT_MIN);
    std::for_each(frustumPoints.cbegin(), frustumPoints.cend(), [&](const vec3& point)
    {
        vec3 p(mtxShadowView * (point + camera.getPosition()));
        minP = minPerComponent(minP, p);
        maxP = maxPerComponent(maxP, p);
    });
    return mat44::createOrthoLH(minP.x, maxP.x, maxP.y, minP.y, min<float>(minP.z, 10), maxP.z);
}
struct ShadowCam : public ICamera
{
    mat44 viewProjection;
    mat44 view;
    mat44 projection;

    float nearClip;
    float farClip;

    vec3 position;
    vec3 look;

    virtual const mat44& getView() const { return view; }
    virtual const mat44& getProjection() const { return projection; }
    virtual const mat44& getViewProjection() const { return viewProjection; };

    virtual float getNearClip() const { return nearClip; };
    virtual float getFarClip() const { return farClip; };
    virtual vec3 getPosition() const { return position; };
    virtual const vec3& getLook() const { return look; };
};
void ShadowCaster::render(const DrawListContainer& drawables, const DirectionalLight::pointer& pDirectionalLight )
{
    vec3 shadowLook(-normalize(pDirectionalLight->getDirection()));
    vec3 up(vec3::up);
    if (dot(up, shadowLook) > 0.99f)
        up = vec3::forward;
    vec3 right(normalize(cross(shadowLook, up)));
    up = normalize(cross(shadowLook, right));

    const Camera& camera(*CAMERAMANAGER->getActiveCamera());

    mat44 mtxShadowView(mat44::createLookAtLH(camera.getPosition() - shadowLook, camera.getPosition(), up));
    mat44 mtxShadowProjection[COUNT-1];
    mtxShadowProjection[0] = getProjection(mtxShadowView, camera.getNearClip(), 30);//25
    mtxShadowProjection[1] = getProjection(mtxShadowView, 20, 80);//75
    mtxShadowProjection[2] = getProjection(mtxShadowView, 70, 155);//150
    mtxShadowProjection[3] = getProjection(mtxShadowView, 145, camera.getFarClip());
    

    //Begin drawing
    GL::heBindFbo(m_FboId);

    const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    GRAPHICS->setViewport(he::RectI(0, 0, m_ShadowSize, m_ShadowSize));


    GL::heClearColor(Color(1.0f, 1.0f, 1.0f, 1.0f));   
    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthWrite(true);
    GL::heSetDepthRead(true);

    for (int i(1); i < COUNT; ++i) //begin at 1, first is blur temp
    {   
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pShadowTexture[i]->getID(), 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        ShadowCam shadowCam;
        shadowCam.projection = mtxShadowProjection[i - 1];
        shadowCam.view = mtxShadowView;
        shadowCam.viewProjection = shadowCam.projection * shadowCam.view;
        shadowCam.look = shadowLook;
        shadowCam.position = camera.getPosition() - shadowLook * 500;       // HACK: hard coded values  TODO !!
        shadowCam.farClip = 1000;
        
        //////////////////////////////////////////////////////////////////////////
        ///                          Build DrawLists                           ///
        //////////////////////////////////////////////////////////////////////////
        std::vector<IDrawable*> culledDrawList;
        std::vector<IDrawable*> culledSkinnedDrawList;

        drawables.for_each(DrawListContainer::F_Loc_BeforePost | DrawListContainer::F_Main_Blended | DrawListContainer::F_Main_Opac | DrawListContainer::F_Sub_Single | DrawListContainer::F_Sub_Skinned, [&](IDrawable* pDrawable)
        {
            if (pDrawable->isVisible() && pDrawable->getCastsShadow() && pDrawable->getModelMesh()->isLoaded())
            {
                if (pDrawable->isInCamera(&shadowCam))
                {
                    if (pDrawable->isSkinned())
                        culledSkinnedDrawList.push_back(pDrawable);
                    else
                        culledDrawList.push_back(pDrawable);
                }
            }
        });

        mat44 mtxShadowViewProjection(mtxShadowProjection[i-1] * mtxShadowView);

        // Sort
        std::sort(culledDrawList.begin(), culledDrawList.end(), [](const IDrawable* d1, const IDrawable* d2)
        {
            return d1->getDrawPriority(CAMERAMANAGER->getActiveCamera()) > d2->getDrawPriority(CAMERAMANAGER->getActiveCamera());
        });
        std::sort(culledSkinnedDrawList.begin(), culledSkinnedDrawList.end(),  [](const IDrawable* d1, const IDrawable* d2)
        {
            return d1->getDrawPriority(CAMERAMANAGER->getActiveCamera()) > d2->getDrawPriority(CAMERAMANAGER->getActiveCamera());
        });

        //////////////////////////////////////////////////////////////////////////
        ///                                 Draw                               ///
        //////////////////////////////////////////////////////////////////////////
        std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](IDrawable* pDrawable)
        {
            pDrawable->applyMaterial(m_MatSingle, &shadowCam);
            pDrawable->drawShadow();
        });

        std::for_each(culledSkinnedDrawList.cbegin(), culledSkinnedDrawList.cend(), [&](IDrawable* pDrawable)
        {
            pDrawable->applyMaterial(m_MatSkinned, &shadowCam);
            pDrawable->drawShadow();
        });

        drawables.for_each(DrawListContainer::F_Loc_BeforePost | DrawListContainer::F_Main_Blended | DrawListContainer::F_Main_Opac | DrawListContainer::F_Sub_Instanced, [&](IDrawable* pDrawable)
        {
            if (pDrawable->getCastsShadow() && pDrawable->isVisible() && pDrawable->isInCamera(&shadowCam))
            {
                pDrawable->applyMaterial(m_MatInstanced, &shadowCam);
                pDrawable->drawShadow();
            }
        });

        pDirectionalLight->setShadowMatrix(i - 1, mtxShadowViewProjection * camera.getView().inverse()); //multiply by inverse view, because everything in shader is in viewspace
    }

    //////////////////////////////////////////////////////////////////////////
    //                                 Blur                                 //
    //////////////////////////////////////////////////////////////////////////
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);
    m_pShadowBlurShaderPass[0]->bind();
    GL::heBindVao(m_pQuad->getVertexShadowArraysID());
    for (int i(1); i < COUNT; ++i)
    {      
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pShadowTexture[i - 1]->getID(), 0); 
        m_pShadowBlurShaderPass[0]->setShaderVar(m_BlurShaderTexPosPass[0], m_pShadowTexture[i]);
        glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
    }
    m_pShadowBlurShaderPass[1]->bind();
    for (int i(COUNT - 1); i >= 1; --i)
    {         
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pShadowTexture[i]->getID(), 0); 
        m_pShadowBlurShaderPass[1]->setShaderVar(m_BlurShaderTexPosPass[1], m_pShadowTexture[i - 1]);
        glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0); 
    for (int i(0); i < COUNT - 1; ++i)
    {
        GL::heBindTexture2D(0, m_pShadowTexture[i+1]->getID());
        glGenerateMipmap(GL_TEXTURE_2D);
        pDirectionalLight->setShadowMap(i, m_pShadowTexture[i+1]);
    }

    GRAPHICS->setViewport(he::RectI(0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));

    if (m_ShowShadowDebug)
    {
        if (GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(0) != nullptr)
            GUI->drawTexture2D(GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(0), vec2(12 * 1 + 256 * 0, 12*3 + 144*2), vec2(256, 256));
        if (GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(1) != nullptr)
            GUI->drawTexture2D(GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(1), vec2(12 * 2 + 256 * 1, 12*3 + 144*2), vec2(256, 256));
        if (GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(2) != nullptr)
            GUI->drawTexture2D(GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(2), vec2(12 * 3 + 256 * 2, 12*3 + 144*2), vec2(256, 256));
        if (GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(3) != nullptr)
            GUI->drawTexture2D(GRAPHICS->getLightManager()->getDirectionalLight()->getShadowMap(3), vec2(12 * 4 + 256 * 3, 12*3 + 144*2), vec2(256, 256));
    }
}


} } //end namespace