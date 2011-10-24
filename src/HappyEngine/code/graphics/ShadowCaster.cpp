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
    m_ShadowSize = 1024;
    //////////////////////////////////////////////////////////////////////////
    ///                             Textures                               ///
    //////////////////////////////////////////////////////////////////////////
    uint texId[COUNT];
    glGenTextures(COUNT, texId);

    for (int i = 0; i < COUNT; ++i)
    {
        GL::heBindTexture2D(texId[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
    //    0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
    float wFar = farClip * tan(pCamera->getFov()),   //half width
          hFar = wFar / pCamera->getAspectRatio();          //half height
    float wNear = nearClip * tan(pCamera->getFov()), //half width
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

    vec3 minP(mtxShadowView * pCamera->getPosition()), maxP(mtxShadowView * pCamera->getPosition());
    std::for_each(frustumPoints.cbegin(), frustumPoints.cend(), [&](const vec3& point)
    {
        vec3 p(mtxShadowView * (point + pCamera->getPosition()));
        minP = minPerComponent(minP, p);
        maxP = maxPerComponent(maxP, p);
    });

    return mat44::createOrthoLH(minP.x, maxP.x, maxP.y, minP.y, min<float>(minP.z, 10), maxP.z);
}
void ShadowCaster::render(const std::vector<DrawManager::DrawElement>& elements,  const Camera* pCamera, const DirectionalLight::pointer& pDirectionalLight )
{
    vec3 shadowLook(-normalize(pDirectionalLight->getDirection()));
    vec3 up(vec3::up);
    if (dot(up, shadowLook) > 0.99f)
        up = vec3::forward;
    vec3 right(normalize(cross(shadowLook, up)));
    up = normalize(cross(shadowLook, right));

    mat44 mtxShadowView(mat44::createLookAtLH(pCamera->getPosition() - shadowLook, pCamera->getPosition(), up));
    mat44 mtxShadowProjection[COUNT];
    mtxShadowProjection[0] = getProjection(pCamera, mtxShadowView, pCamera->getNearClip(), 60);
    mtxShadowProjection[1] = getProjection(pCamera, mtxShadowView, 60, 110);
    mtxShadowProjection[2] = getProjection(pCamera, mtxShadowView, 110, pCamera->getFarClip());
    


    //Begin drawing
    GL::heBindFbo(m_FboId);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    glViewport(0, 0, m_ShadowSize, m_ShadowSize);
    GL::heSetCullFace(true);

    m_pShadowShader->bind();

    for (int i = 0; i < COUNT; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pShadowTexture[i]->getID(), 0);
        glClear(GL_DEPTH_BUFFER_BIT);
        mat44 mtxShadowViewProjection(mtxShadowProjection[i] * mtxShadowView);

        std::for_each(elements.cbegin(), elements.cend(), [&](const DrawManager::DrawElement& e)
        {
            m_pShadowShader->setShaderVar(m_shaderWVPpos, mtxShadowViewProjection * e.pDrawable->getWorldMatrix());
            std::for_each(e.pDrawable->getModel()->cbegin(), e.pDrawable->getModel()->cend(), [&](const ModelMesh::pointer& m)
            {   
                GL::heBindVao(m->getVertexShadowArraysID());
                glDrawElements(GL_TRIANGLES, m->getNumIndices(), m->getIndexType(), 0);   
            });
        });
        pDirectionalLight->setShadowMap(i, m_pShadowTexture[i]);
        pDirectionalLight->setShadowMatrix(i, mtxShadowViewProjection * pCamera->getView().inverse()); //multiply by inverse view, because everything in shader is in viewspace
    }

    GL::heSetCullFace(false);
    glViewport(0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight());
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

}
    

} } //end namespace