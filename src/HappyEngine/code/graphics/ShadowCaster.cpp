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
//Created: 22/10/2011
#include "StdAfx.h" 

#include "ShadowCaster.h"
#include "HappyNew.h"
#include "OpenGL.h"
#include "ExternalError.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "Happy2DRenderer.h"

namespace he {
namespace gfx {

ShadowCaster::ShadowCaster(): m_pShadowShader(NEW Shader())
{
}


ShadowCaster::~ShadowCaster()
{
    glDeleteFramebuffers(1, &m_FboId);
}

void ShadowCaster::init(const DrawSettings& settings)
{
    m_ShadowSize = 512 * settings.getShadowMapMultiplier();
    ASSERT(m_ShadowSize <= 2048, "shadowmap size must be <= 2048");
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 
            m_ShadowSize, m_ShadowSize, 
            0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
        m_pShadowTexture[i] = Texture2D::pointer(NEW Texture2D());
        m_pShadowTexture[i]->init(texId[i], m_ShadowSize, m_ShadowSize, GL_DEPTH_COMPONENT32F);
    }

    //GL::heBindTexture2D(texId[1]);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 
    //    2048, 2048,  
    //    0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    //m_pTempTexture = Texture2D::pointer(NEW Texture2D());
    //m_pTempTexture->init(texId[1], 2048, 2048, GL_RGBA8);

    //GL::heBindTexture2D(0);

    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////
    glGenFramebuffers(1, &m_FboId);
    GL::heBindFbo(m_FboId);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId[0], 0);
    err::checkFboStatus("deferred collection");

    GL::heBindFbo(0);


    //////////////////////////////////////////////////////////////////////////
    ///                             Shaders                                ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    std::vector<std::string> outputs;
    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    m_pShadowShader->init(folder + "deferred/pre/deferredPreShadowShader.vert", 
                          folder + "deferred/pre/deferredPreShadowShader.frag", 
                          shaderLayout, outputs);
    m_shaderWVPpos = m_pShadowShader->getShaderVarId("matWVP");
}


mat44 getProjection(const Camera* pCamera, const mat44& mtxShadowView, float nearClip, float farClip)
{
    float wFar = farClip * tan(pCamera->getFov()),          //half width
          hFar = wFar / pCamera->getAspectRatio();          //half height
    float wNear = nearClip * tan(pCamera->getFov()),        //half width
          hNear = wNear / pCamera->getAspectRatio();        //half height

    std::vector<vec3> frustumPoints;
    frustumPoints.reserve(8);
    //Far plane
    frustumPoints.push_back(pCamera->getLook() * farClip + pCamera->getUp() * hFar - pCamera->getRight() * wFar);
    frustumPoints.push_back(pCamera->getLook() * farClip + pCamera->getUp() * hFar + pCamera->getRight() * wFar);
    frustumPoints.push_back(pCamera->getLook() * farClip - pCamera->getUp() * hFar - pCamera->getRight() * wFar);
    frustumPoints.push_back(pCamera->getLook() * farClip - pCamera->getUp() * hFar + pCamera->getRight() * wFar);
    //Near plane
    frustumPoints.push_back(pCamera->getLook() * nearClip + pCamera->getUp() * hNear - pCamera->getRight() * wNear);
    frustumPoints.push_back(pCamera->getLook() * nearClip + pCamera->getUp() * hNear + pCamera->getRight() * wNear);
    frustumPoints.push_back(pCamera->getLook() * nearClip - pCamera->getUp() * hNear - pCamera->getRight() * wNear);
    frustumPoints.push_back(pCamera->getLook() * nearClip - pCamera->getUp() * hNear + pCamera->getRight() * wNear);

    vec3 minP(FLT_MAX, FLT_MAX, FLT_MAX), maxP(FLT_MIN, FLT_MIN, FLT_MIN);
    std::for_each(frustumPoints.cbegin(), frustumPoints.cend(), [&](const vec3& point)
    {
        vec3 p(mtxShadowView * (point + pCamera->getPosition()));
        minP = minPerComponent(minP, p);
        maxP = maxPerComponent(maxP, p);
    });
    return mat44::createOrthoLH(minP.x, maxP.x, maxP.y, minP.y, min<float>(minP.z, 10), maxP.z);
}

void ShadowCaster::render(const std::vector<const IDrawable*>& drawables,  const Camera* pCamera, const DirectionalLight::pointer& pDirectionalLight )
{
    PROFILER_BEGIN("ShadowCaster::render");
    vec3 shadowLook(-normalize(pDirectionalLight->getDirection()));
    vec3 up(vec3::up);
    if (dot(up, shadowLook) > 0.99f)
        up = vec3::forward;
    vec3 right(normalize(cross(shadowLook, up)));
    up = normalize(cross(shadowLook, right));

    mat44 mtxShadowView(mat44::createLookAtLH(pCamera->getPosition() - shadowLook, pCamera->getPosition(), up));
    mat44 mtxShadowProjection[COUNT];
    mtxShadowProjection[0] = getProjection(pCamera, mtxShadowView, pCamera->getNearClip(), 25);
    mtxShadowProjection[1] = getProjection(pCamera, mtxShadowView, 25, 50);
    mtxShadowProjection[2] = getProjection(pCamera, mtxShadowView, 50, 100);
    mtxShadowProjection[3] = getProjection(pCamera, mtxShadowView, 100, pCamera->getFarClip());
    

    //Begin drawing
    GL::heBindFbo(m_FboId);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glDrawBuffers(0, 0);

    glViewport(0, 0, m_ShadowSize, m_ShadowSize);
    GL::heSetCullFace(false);

    m_pShadowShader->bind();

    for (int i = 0; i < COUNT; ++i)
    {
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pShadowTexture[i]->getID(), 0);
        glClear(GL_DEPTH_BUFFER_BIT);
        mat44 mtxShadowViewProjection(mtxShadowProjection[i] * mtxShadowView);

        //////////////////////////////////////////////////////////////////////////
        ///                         `Build DrawList                            ///
        //////////////////////////////////////////////////////////////////////////
        std::vector<DrawManager::DrawElement> culledDrawList;
        culledDrawList.reserve(drawables.size());
        std::for_each(drawables.cbegin(), drawables.cend(), [&](const IDrawable* pDrawable)
        {
            if (pDrawable->isVisible() && pDrawable->getCastsShadow() && pDrawable->getModel()->isLoaded())
            {
                shapes::Sphere bS(pDrawable->getModel()->getBoundingSphere().getPosition() + pDrawable->getWorldMatrix().getTranslation(), 
                    pDrawable->getModel()->getBoundingSphere().getRadius() * pDrawable->getWorldMatrix()(0, 0)); // HACK: only uniform scales

                if (DrawManager::viewClip(pCamera, bS) == false)
                {
                    DrawManager::DrawElement e;
                    e.pDrawable = pDrawable;
                    e.sorter = lengthSqr(pCamera->getPosition() - e.pDrawable->getWorldMatrix().getTranslation());
                    culledDrawList.push_back(e);
                }
            }
        });

        std::sort(culledDrawList.begin(), culledDrawList.end());

        //////////////////////////////////////////////////////////////////////////
        ///                                 Draw                               ///
        //////////////////////////////////////////////////////////////////////////
        std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](const DrawManager::DrawElement& e)
        {
            m_pShadowShader->setShaderVar(m_shaderWVPpos, mtxShadowViewProjection * e.pDrawable->getWorldMatrix());
            GL::heBindVao(e.pDrawable->getModel()->getVertexShadowArraysID());
            glDrawElements(GL_TRIANGLES, e.pDrawable->getModel()->getNumIndices(), e.pDrawable->getModel()->getIndexType(), 0);
        });
        pDirectionalLight->setShadowMap(i, m_pShadowTexture[i]);
        pDirectionalLight->setShadowMatrix(i, mtxShadowViewProjection * pCamera->getView().inverse()); //multiply by inverse view, because everything in shader is in viewspace
    }

    GL::heSetCullFace(false);
    glViewport(0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight());
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    PROFILER_END("ShadowCaster::render");
}
    

} } //end namespace