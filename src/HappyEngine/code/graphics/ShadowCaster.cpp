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
#include "Renderer2D.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ShaderVar.h"
#include "View.h"
#include "Scene.h"
#include "RenderTarget.h"
#include "CameraPerspective.h"
#include "CameraOrtho.h"

namespace he {
namespace gfx {

ShadowCaster::ShadowCaster(): m_ShowShadowDebug(false), m_ShadowSize(0), m_pQuad(nullptr),
    m_MatInstanced(nullptr), m_MatSingle(nullptr), m_MatSkinned(nullptr), m_RenderTarget(NEW RenderTarget())
{
    CONSOLE->registerVar(&m_ShowShadowDebug, "b_shadowtex");
    for (int i = 0; i < COUNT; ++i)
    {
        m_ShadowTexture[i] = nullptr;
    }
    for (uint i(0); i < s_ShadowBlurPasses; ++i)
    {
    	m_pShadowBlurShaderPass[i] = nullptr;
    }
    he_memset(&m_Settings, 0, sizeof(ShadowSettings));
}


ShadowCaster::~ShadowCaster()
{
    m_View->get2DRenderer()->detachFromRender(this);
    for (int i = 0; i < COUNT; ++i)
    {
        if (m_ShadowTexture[i] != nullptr)
            m_ShadowTexture[i]->release();
    }
    if (m_pQuad != nullptr)
        m_pQuad->release();

    for (uint i(0); i < s_ShadowBlurPasses; ++i)
    {
        if (m_pShadowBlurShaderPass[i] != nullptr)
    	    m_pShadowBlurShaderPass[i]->release();
    }
    if (m_MatSingle != nullptr)
        m_MatSingle->release();
    if (m_MatSkinned != nullptr)
        m_MatSkinned->release();
    if (m_MatInstanced != nullptr)
        m_MatInstanced->release();

    delete m_RenderTarget;
}

void ShadowCaster::init(View* view)
{
    m_View = view;
    m_View->get2DRenderer()->attachToRender(this);

    onSettingsChanged();
 
    //////////////////////////////////////////////////////////////////////////
    ///                             Shaders                                ///
    //////////////////////////////////////////////////////////////////////////
    ResourceFactory<gfx::Shader>* shaderFactory(ResourceFactory<gfx::Shader>::getInstance());
    Shader* pShadowShaderSingle(shaderFactory->get(shaderFactory->create()));
    Shader* pShadowShaderSkinned(shaderFactory->get(shaderFactory->create()));
    Shader* pShadowShaderInstanced(shaderFactory->get(shaderFactory->create()));

    // Single
    ShaderLayout shaderSingleLayout;
    shaderSingleLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    BufferLayout vertexSingleLayout, noInstancingLayout;
    vertexSingleLayout.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));

    std::vector<std::string> outputs;
    const std::string& folder(CONTENT->getShaderFolderPath().str());
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
        m_pShadowBlurShaderPass[pass] = shaderFactory->get(shaderFactory->create());

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
    if (m_MatSingle != nullptr)
        m_MatSingle->release();
    if (m_MatSkinned != nullptr)
        m_MatSkinned->release();
    if (m_MatInstanced != nullptr)
        m_MatInstanced->release();
    ResourceFactory<Material>* materialFactory(ResourceFactory<Material>::getInstance());
    m_MatSingle = materialFactory->get(materialFactory->create());
    m_MatSingle->setShader(pShadowShaderSingle->getHandle(), vertexSingleLayout, noInstancingLayout);
    m_MatSingle->registerVar(NEW ShaderUserVar<vec2>(pShadowShaderSingle->getShaderVarId("inLightNearFar"), "inLightNearFar", vec2(1, 500)));
    m_MatSingle->registerVar(NEW ShaderGlobalVar(pShadowShaderSingle->getShaderVarId("matWV"), "matWV", ShaderVarType_WorldView));
    m_MatSingle->registerVar(NEW ShaderGlobalVar(pShadowShaderSingle->getShaderVarId("matWVP"), "matWVP", ShaderVarType_WorldViewProjection));

    m_MatSkinned = materialFactory->get(materialFactory->create());
    m_MatSkinned->setShader(pShadowShaderSkinned->getHandle(), vertexSkinnedLayout, noInstancingLayout);
    m_MatSkinned->registerVar(NEW ShaderUserVar<vec2>(pShadowShaderSkinned->getShaderVarId("inLightNearFar"), "inLightNearFar", vec2(1, 500)));
    m_MatSkinned->registerVar(NEW ShaderGlobalVar(pShadowShaderSkinned->getShaderVarId("matWV"), "matWV", ShaderVarType_WorldView));
    m_MatSkinned->registerVar(NEW ShaderGlobalVar(pShadowShaderSkinned->getShaderVarId("matWVP"), "matWVP", ShaderVarType_WorldViewProjection));
    m_MatSkinned->registerVar(NEW ShaderGlobalVar(pShadowShaderSkinned->getShaderVarId("matBones"), "matBones", ShaderVarType_BoneTransforms));

    m_MatInstanced = materialFactory->get(materialFactory->create());
    m_MatInstanced->setShader(pShadowShaderInstanced->getHandle(), vertexSingleLayout, instancingLayout);
    m_MatInstanced->registerVar(NEW ShaderUserVar<vec2>(pShadowShaderInstanced->getShaderVarId("inLightNearFar"), "inLightNearFar", vec2(1, 500)));
    m_MatInstanced->registerVar(NEW ShaderGlobalVar(pShadowShaderInstanced->getShaderVarId("matV"), "matV", ShaderVarType_View));
    m_MatInstanced->registerVar(NEW ShaderGlobalVar(pShadowShaderInstanced->getShaderVarId("matVP"), "matVP", ShaderVarType_ViewProjection));

    m_pQuad = CONTENT->getFullscreenQuad();

    pShadowShaderSingle->release();
    pShadowShaderSkinned->release();
    pShadowShaderInstanced->release();
}
void ShadowCaster::onSettingsChanged()
{
    ushort newShadowSize(static_cast<short>(512 * pow(2.0f, m_Settings.shadowMult - 1.0f)));
    HE_ASSERT(m_ShadowSize <= 2048, "shadowmap size must be <= 2048");

    bool reload(newShadowSize != m_ShadowSize);

    if (reload == true)
    {
        //////////////////////////////////////////////////////////////////////////
        ///                             Textures                               ///
        //////////////////////////////////////////////////////////////////////////
        for (int i = 0; i < COUNT; ++i)
        {
            if (m_ShadowTexture[i] != nullptr)
                m_ShadowTexture[i]->release();
            ObjectHandle handle(ResourceFactory<Texture2D>::getInstance()->create());
            m_ShadowTexture[i] = ResourceFactory<Texture2D>::getInstance()->get(handle);
            m_ShadowTexture[i]->setName("ShadowCaster::m_ShadowTexture[i]");
            m_ShadowTexture[i]->init(gfx::Texture2D::WrapType_Clamp, gfx::Texture2D::FilterType_Linear, gfx::Texture2D::TextureFormat_RG16, true);
            m_ShadowTexture[i]->setData(m_ShadowSize, m_ShadowSize, 0, 
                gfx::Texture2D::BufferLayout_RG, gfx::Texture2D::BufferType_Float, 0 );
        }
        //////////////////////////////////////////////////////////////////////////
        ///                            LOAD FBO's                              ///
        //////////////////////////////////////////////////////////////////////////
        m_RenderTarget->removeAllTargets();
        m_RenderTarget->setDepthTarget();
        for (int i = 0; i < COUNT; ++i)
        {
            m_RenderTarget->addTextureTarget(m_ShadowTexture[i]);
        }
        m_RenderTarget->init();
    }
}

void ShadowCaster::setShadowCamLens(float nearClip, float farClip, CameraOrtho& inoutCam)
{
    const CameraPerspective& camera(*m_View->getScene()->getCameraManager()->getActiveCamera());
    const mat44& mtxShadowView(inoutCam.getView());

    float wFar = farClip * tan(camera.getFov()),          //half width
          hFar = wFar * camera.getAspectRatio();          //half height
    float wNear = nearClip * tan(camera.getFov()),        //half width
          hNear = wNear * camera.getAspectRatio();        //half height

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
    float res(max(maxP.x - minP.x, maxP.y - minP.y));
    inoutCam.setLens(minP.x, minP.x + res, minP.y + res, minP.y, 1, 500);
}

void ShadowCaster::render()
{
    const Scene* scene(m_View->getScene());
    DirectionalLight* directionalLight(scene->getLightManager()->getDirectionalLight());

    vec3 shadowLook(-normalize(directionalLight->getDirection()));
    vec3 up(vec3::up);
    if (dot(up, shadowLook) > 0.99f)
        up = vec3::forward;
    vec3 right(normalize(cross(shadowLook, up)));
    up = normalize(cross(shadowLook, right));

    const CameraPerspective& camera(*m_View->getScene()->getCameraManager()->getActiveCamera());

    CameraOrtho shadowCam[4];
    for (int i(0); i < COUNT-1; ++i) //begin at 1, first is blur temp
    {
        shadowCam[i].lookAt(camera.getPosition() - shadowLook*250, camera.getPosition(), up);
    }
    setShadowCamLens(1, 20, shadowCam[0]);
    setShadowCamLens(1, 40, shadowCam[1]); 
    setShadowCamLens(20, 60, shadowCam[2]);
    setShadowCamLens(40, camera.getFarClip(), shadowCam[3]);
    

    //Begin drawing
    GL::heClearColor(Color(1.0f, 1.0f, 1.0f, 1.0f));   
    GL::heSetViewport(he::RectI(0, 0, m_ShadowSize, m_ShadowSize));
    GL::heBlendEnabled(false);
    GL::heSetCullFace(false);
    GL::heSetDepthFunc(DepthFunc_LessOrEqual);
    GL::heSetDepthWrite(true);
    GL::heSetDepthRead(true);


    for (int i(1); i < COUNT; ++i) //begin at 1, first is blur temp
    {   
        m_RenderTarget->prepareForRendering(1, i);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        //////////////////////////////////////////////////////////////////////////
        ///                                 Draw                               ///
        //////////////////////////////////////////////////////////////////////////
        scene->getDrawList().draw(DrawListContainer::BlendFilter_Opac, &shadowCam[i-1], [&](IDrawable* drawable)
        {
            if (drawable->getCastsShadow() == true)
            {
                if (drawable->isSkinned())
                    drawable->applyMaterial(m_MatSkinned, &shadowCam[i-1]);
                else if (drawable->isSingle())
                    drawable->applyMaterial(m_MatSingle, &shadowCam[i-1]);
                else
                    drawable->applyMaterial(m_MatInstanced, &shadowCam[i-1]);

                drawable->drawShadow();
            }
        });

        directionalLight->setShadowMatrix(i - 1, shadowCam[i-1].getViewProjection() * camera.getView().inverse()); //multiply by inverse view, because everything in shader is in viewspace
    }
    directionalLight->setShadowPosition(shadowCam[0].getPosition());
    directionalLight->setShadowNearFar(shadowCam[0].getNearClip(), shadowCam[COUNT - 1].getFarClip());
    //////////////////////////////////////////////////////////////////////////
    //                                 Blur                                 //
    //////////////////////////////////////////////////////////////////////////
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);
    m_pShadowBlurShaderPass[0]->bind();
    GL::heBindVao(m_pQuad->getVertexShadowArraysID());
    for (int i(1); i < COUNT; ++i)
    {      
        m_RenderTarget->prepareForRendering(1, i - 1);
        m_pShadowBlurShaderPass[0]->setShaderVar(m_BlurShaderTexPosPass[0], m_ShadowTexture[i]);
        glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
    }
    m_pShadowBlurShaderPass[1]->bind();
    for (int i(COUNT - 1); i >= 1; --i)
    {         
        m_RenderTarget->prepareForRendering(1, i);
        m_pShadowBlurShaderPass[1]->setShaderVar(m_BlurShaderTexPosPass[1], m_ShadowTexture[i - 1]);
        glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
    }

    for (int i(0); i < COUNT - 1; ++i)
    {
        m_ShadowTexture[i+1]->generateMipMaps();
        directionalLight->setShadowMap(i, m_ShadowTexture[i+1]);
    }

    GL::heSetViewport(m_View->getViewport());

}

void ShadowCaster::draw2D( Renderer2D* renderer )
{
    const Scene* scene(m_View->getScene());
    DirectionalLight* directionalLight(scene->getLightManager()->getDirectionalLight());
    if (m_ShowShadowDebug)
    {
        if (directionalLight->getShadowMap(0) != nullptr)
            renderer->drawTexture2DToScreen(directionalLight->getShadowMap(0), vec2(12 * 1 + 256 * 0, 12*3 + 144*2), false, vec2(256, 256));
        if (directionalLight->getShadowMap(1) != nullptr)
            renderer->drawTexture2DToScreen(directionalLight->getShadowMap(1), vec2(12 * 2 + 256 * 1, 12*3 + 144*2), false, vec2(256, 256));
        if (directionalLight->getShadowMap(2) != nullptr)
            renderer->drawTexture2DToScreen(directionalLight->getShadowMap(2), vec2(12 * 3 + 256 * 2, 12*3 + 144*2), false, vec2(256, 256));
        if (directionalLight->getShadowMap(3) != nullptr)
            renderer->drawTexture2DToScreen(directionalLight->getShadowMap(3), vec2(12 * 4 + 256 * 3, 12*3 + 144*2), false, vec2(256, 256));
    }
}


} } //end namespace