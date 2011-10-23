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

void ShadowCaster::init()
{
    //////////////////////////////////////////////////////////////////////////
    ///                             Textures                               ///
    //////////////////////////////////////////////////////////////////////////
    uint texId[2];
    glGenTextures(2, texId);

    GL::heBindTexture2D(texId[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 
        1280, 720, 
        0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    m_pShadowTexture = Texture2D::pointer(NEW Texture2D());
    m_pShadowTexture->init(texId[0], 1280, 720, GL_DEPTH_COMPONENT32F);

    GL::heBindTexture2D(texId[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 
        1280, 720,  
        0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    m_pTempTexture = Texture2D::pointer(NEW Texture2D());
    m_pTempTexture->init(texId[1], 1280, 720, GL_RGBA8);

    GL::heBindTexture2D(0);

    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////
    glGenFramebuffers(1, &m_FboId);
    GL::heBindFbo(m_FboId);
    /*glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);*/
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId[1], 0);
    err::checkFboStatus("deferred collection");

    GL::heBindFbo(0);


    //////////////////////////////////////////////////////////////////////////
    ///                             Shaders                                ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    std::vector<std::string> outputs;
    outputs.push_back("outColor");
    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    m_pShadowShader->init(folder + "deferred/pre/deferredPreShadowShader.vert", 
                          folder + "deferred/pre/deferredPreShadowShader.frag", 
                          shaderLayout, outputs);
    m_shaderWVPpos = m_pShadowShader->getShaderVarId("matWVP");
}

void ShadowCaster::render(const std::vector<DrawManager::DrawElement>& elements,  const Camera* pCamera, const DirectionalLight::pointer& pDirectionalLight )
{
    vec3 shadowLook(-normalize(pDirectionalLight->getDirection()));
    vec3 up(vec3::up);
    if (dot(up, shadowLook) > 0.99f)
        up = vec3::forward;
    vec3 right(normalize(cross(shadowLook, up)));
    up = normalize(cross(shadowLook, right));

    mat44 mtxShadowView(mat44::createLookAtLH(pCamera->getPosition() - shadowLook * 20, pCamera->getPosition(), up));
    //mat44 mtxShadowView(mat44::createLookAtLH(pCamera->getPosition() + pCamera->getUp() * 20, pCamera->getPosition(), pCamera->getLook()));
    //mat44 mtxShadowView(mat44::createLookAtLH(pCamera->getPosition(), pCamera->getPosition() + pCamera->getLook(), pCamera->getUp()));

    //transform camera frustum to mtxShadowView
    //extract min, max
    float wFar = pCamera->getFarClip() * tan(pCamera->getFov() / 2.0f), //half width
          hFar = wFar / pCamera->getAspectRatio();                      //half height
    float wNear = pCamera->getNearClip() * tan(pCamera->getFov() / 2.0f), //half width
          hNear = wNear / pCamera->getAspectRatio();                       //half height
    
    std::vector<vec3> frustumPoints;
    frustumPoints.reserve(8);
    //Far plane
    frustumPoints.push_back(pCamera->getLook() * pCamera->getFarClip() + pCamera->getUp() * hFar - pCamera->getRight() * wFar);
    frustumPoints.push_back(pCamera->getLook() * pCamera->getFarClip() + pCamera->getUp() * hFar + pCamera->getRight() * wFar);
    frustumPoints.push_back(pCamera->getLook() * pCamera->getFarClip() - pCamera->getUp() * hFar - pCamera->getRight() * wFar);
    frustumPoints.push_back(pCamera->getLook() * pCamera->getFarClip() - pCamera->getUp() * hFar + pCamera->getRight() * wFar);
    //Near plane
    frustumPoints.push_back(pCamera->getLook() * pCamera->getNearClip() + pCamera->getUp() * hNear - pCamera->getRight() * wNear);
    frustumPoints.push_back(pCamera->getLook() * pCamera->getNearClip() + pCamera->getUp() * hNear + pCamera->getRight() * wNear);
    frustumPoints.push_back(pCamera->getLook() * pCamera->getNearClip() - pCamera->getUp() * hNear - pCamera->getRight() * wNear);
    frustumPoints.push_back(pCamera->getLook() * pCamera->getNearClip() - pCamera->getUp() * hNear + pCamera->getRight() * wNear);

    vec3 minP(FLT_MAX, FLT_MAX, FLT_MAX), maxP(FLT_MIN, FLT_MIN, FLT_MIN);
    std::for_each(frustumPoints.cbegin(), frustumPoints.cend(), [&](const vec3& point)
    {
        vec3 p(mtxShadowView * (point + pCamera->getPosition()));
        minP = minPerComponent(minP, p);
        maxP = maxPerComponent(maxP, p);
    });

    mat44 mtxShadowProjection(mat44::createOrthoLH(minP.x, maxP.x, maxP.y, minP.y, 10, maxP.z));
    //mat44 mtxShadowProjection(mat44::createPerspectiveLH(piOverFour, 2048, 2048, 10, 1000));
    //mat44 mtxShadowProjection(pCamera->getProjection());

    mat44 mtxShadowViewProjection(mtxShadowProjection * mtxShadowView);

    GL::heBindFbo(m_FboId);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pShadowShader->bind();

    std::for_each(elements.cbegin(), elements.cend(), [&](const DrawManager::DrawElement& e)
    {
        m_pShadowShader->setShaderVar(m_shaderWVPpos, mtxShadowViewProjection * e.pDrawable->getWorldMatrix());
        std::for_each(e.pDrawable->getModel()->cbegin(), e.pDrawable->getModel()->cend(), [&](const ModelMesh::pointer& m)
        {   
            GL::heBindVao(m->getVertexShadowArraysID());
            glDrawElements(GL_TRIANGLES, m->getNumIndices(), m->getIndexType(), 0);   
        });
    });
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    pDirectionalLight->setShadowMap(m_pShadowTexture);
    pDirectionalLight->setShadowMatrix(mtxShadowViewProjection * pCamera->getView().inverse());
}

const Texture2D::pointer ShadowCaster::getShadowMap() const
{
    return m_pShadowTexture;
}

} } //end namespace