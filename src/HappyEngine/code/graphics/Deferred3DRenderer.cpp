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
#include "HappyEngine.h"
#include "GL/glew.h"
#include "VertexLayout.h"
#include "Vertex.h"
#include "Assert.h"
#include "Light.h"
#include "HappyNew.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"

#include <vector>

namespace he {
namespace gfx {

// Textures:
//      - Color.rgb, ill       GL_RGBA8
//      - spec, gloss,         GL_RGBA8
//      - Normal.xy            GL_RG16F
//      - Depth                GL_DEPTH24_STENCIL8
const int Deferred3DRenderer::TEXTURE_FORMAT[TEXTURES] = { GL_RGBA, GL_RGBA, GL_RGBA, GL_DEPTH_COMPONENT };
const int Deferred3DRenderer::TEXTURE_INTERNALFORMAT[TEXTURES] = {GL_RGBA8, GL_RGBA8, GL_RG16F, GL_DEPTH_COMPONENT32F};
const int Deferred3DRenderer::TEXTURE_ATTACHMENT[TEXTURES] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT};
VertexLayout Deferred3DRenderer::s_VertexLayoutLightVolume = VertexLayout();

Deferred3DRenderer::Deferred3DRenderer(): m_pModel(NEW ModelMesh("deferred3DRenderer_QUAD")), m_pLightManager(NEW LightManager())
{
    /*------------------------------------------------------------------------------*/
    /*                         LOAD FBO AND RENDER TARGETS                          */
    /*------------------------------------------------------------------------------*/
    int width = GRAPHICS->getViewport().width, 
       height = GRAPHICS->getViewport().height;

    //Textures
    glGenTextures(TEXTURES, m_TextureId);
    for (int i = 0; i < TEXTURES; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, m_TextureId[i]);
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
    glBindTexture(GL_TEXTURE_2D, 0);

    //Renderbuffers
    //glGenRenderbuffers(1, &m_DepthBufferId);
    //glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferId);
    //glRenderbufferStorage(GL_RENDERBUFFER, TEXTURE_INTERNALFORMAT[3], width, height);
    
    //Framebuffer
    glGenFramebuffers(1, &m_FboId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);

    for (int i = 0; i < TEXTURES; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, TEXTURE_ATTACHMENT[i], GL_TEXTURE_2D, m_TextureId[i], 0);
    }
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferId);
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (err != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Woops something went wrong with the framebuffer\n";
        switch (err)
        {
            case GL_FRAMEBUFFER_UNDEFINED: std::cout << "GL_FRAMEBUFFER_UNDEFINED\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: std::cout << "GL_FRAMEBUFFER_UNSUPPORTED\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n"; break;
            default: std::cout << "GL_UNKOWN_ERROR\n"; break;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE); //disable enable writing to depth buffer
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);


    /*------------------------------------------------------------------------------*/
    /*                               LOAD SHADERS                                   */
    /*----------------------------------------------------------------------------- */
    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    s_VertexLayoutLightVolume.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, 12, 0));

    for (int i = 0; i < SHADERS; ++i)
    {
        m_pPostShader[i] = NEW Shader();
    }

    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());
    m_pPostShader[0]->init(folder + "deferred/post/deferredPostShaderQuad.vert", folder + "deferred/post/deferredPostALShader.frag", shaderLayout);
    m_pPostShader[1]->init(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostPLShader.frag", shaderLayout);
    m_pPostShader[2]->init(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostSLShader.frag", shaderLayout);
    m_pPostShader[3]->init(folder + "deferred/post/deferredPostShader.vert", folder + "deferred/post/deferredPostDLShader.frag", shaderLayout);
    
    for (int i = 0; i < SHADERS; ++i)
    {
        m_ShaderColMapPos[i] = m_pPostShader[i]->getShaderSamplerId("colorMap");
        if (i != LightType_AmbientLight)
        {
            m_ShaderWVP[i] = m_pPostShader[i]->getShaderVarId("mtxWVP");
            //m_ShaderWV[i] = m_pPostShader[i]->getShaderVarId("mtxWV");
            m_ShaderNormalMapPos[i] = m_pPostShader[i]->getShaderSamplerId("normalMap");
            m_ShaderSGIMapPos[i] = m_pPostShader[i]->getShaderSamplerId("sgiMap");
            m_ShaderDepthMapPos[i] = m_pPostShader[i]->getShaderSamplerId("depthMap");
            m_ShaderProjParams[i] = m_pPostShader[i]->getShaderVarId("projParams");
            //m_ShaderNearFar[i] = m_pPostShader[i]->getShaderVarId("nearFar");
            //m_ShaderTopRight[i] = m_pPostShader[i]->getShaderVarId("topRight");
            //m_ShaderInvMtxProj[i] = m_pPostShader[i]->getShaderVarId("invMtxProj");
        }
    }
    //----AL----------------------------------------------------------------------
    m_ShaderALPos[0] = m_pPostShader[LightType_AmbientLight]->getShaderVarId("light.multiplier");
    m_ShaderALPos[1] = m_pPostShader[LightType_AmbientLight]->getShaderVarId("light.color");
    //----PL----------------------------------------------------------------------
    m_ShaderPLPos[0] = m_pPostShader[LightType_PointLight]->getShaderVarId("light.position");
    m_ShaderPLPos[1] = m_pPostShader[LightType_PointLight]->getShaderVarId("light.multiplier");
    m_ShaderPLPos[2] = m_pPostShader[LightType_PointLight]->getShaderVarId("light.color");
    m_ShaderPLPos[3] = m_pPostShader[LightType_PointLight]->getShaderVarId("light.beginAttenuation");
    m_ShaderPLPos[4] = m_pPostShader[LightType_PointLight]->getShaderVarId("light.endAttenuation");
    //----SL----------------------------------------------------------------------
    m_ShaderSLPos[0] = m_pPostShader[LightType_SpotLight]->getShaderVarId("light.position");
    m_ShaderSLPos[1] = m_pPostShader[LightType_SpotLight]->getShaderVarId("light.multiplier");
    m_ShaderSLPos[2] = m_pPostShader[LightType_SpotLight]->getShaderVarId("light.direction");
    m_ShaderSLPos[3] = m_pPostShader[LightType_SpotLight]->getShaderVarId("light.beginAttenuation");
    m_ShaderSLPos[4] = m_pPostShader[LightType_SpotLight]->getShaderVarId("light.color");
    m_ShaderSLPos[5] = m_pPostShader[LightType_SpotLight]->getShaderVarId("light.endAttenuation");
    m_ShaderSLPos[6] = m_pPostShader[LightType_SpotLight]->getShaderVarId("light.cosCutoff");
    //----DL----------------------------------------------------------------------
    m_ShaderDLPos[0] = m_pPostShader[LightType_DirectionalLight]->getShaderVarId("light.direction");
    m_ShaderDLPos[1] = m_pPostShader[LightType_DirectionalLight]->getShaderVarId("light.color");
    m_ShaderDLPos[2] = m_pPostShader[LightType_DirectionalLight]->getShaderVarId("light.multiplier");

    /*------------------------------------------------------------------------------*/
    /*                             LOAD RENDER QUAD                                 */
    /*----------------------------------------------------------------------------- */
    std::vector<VertexPos> vertices;
    vertices.push_back(VertexPos(vec3(-1, 1, 0.5f)));
    vertices.push_back(VertexPos(vec3(1, 1, 0.5f)));
    vertices.push_back(VertexPos(vec3(-1, -1, 0.5f)));
    vertices.push_back(VertexPos(vec3(1, -1, 0.5f)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    m_pModel->init();
    m_pModel->setVertices(&vertices[0], 4, getVertexLayoutLightVolume());
    m_pModel->setIndices(&indices[0], 6, IndexStride_Byte);
}


Deferred3DRenderer::~Deferred3DRenderer()
{
    glDeleteFramebuffers(1, &m_FboId);
    glDeleteRenderbuffers(1, &m_DepthBufferId);

    for (int i = 0; i < SHADERS; ++i)
    {
        delete m_pPostShader[i];
    }

    delete m_pLightManager;
}

const VertexLayout& Deferred3DRenderer::getVertexLayoutLightVolume() 
{
    return s_VertexLayoutLightVolume; 
}
void Deferred3DRenderer::begin(const Camera* pCamera)
{
    m_pCamera = pCamera;
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    const static GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, buffers);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
void Deferred3DRenderer::end()
{    
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
    const static GLenum buffers[1] = { GL_BACK_LEFT };
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffers(1, buffers);

    glEnable(GL_BLEND);
    //glEnable(GL_SCISSOR_TEST);
    glBlendFunc(GL_ONE, GL_ONE);
    glDisable(GL_DEPTH_TEST);

    for (int i = 0; i < SHADERS; ++i)
    {
		if (i == 1)
			glCullFace(GL_FRONT);

        m_pPostShader[i]->begin();
        
        m_pPostShader[i]->setShaderVar(m_ShaderColMapPos[i], m_pTexture[0]);
        if (i != LightType_AmbientLight)
        {
            m_pPostShader[i]->setShaderVar(m_ShaderSGIMapPos[i], m_pTexture[1]);
            m_pPostShader[i]->setShaderVar(m_ShaderNormalMapPos[i], m_pTexture[2]);
            m_pPostShader[i]->setShaderVar(m_ShaderDepthMapPos[i], m_pTexture[3]);
            //float zfar(m_pCamera->getFarClip()), znear(m_pCamera->getNearClip());
            m_pPostShader[i]->setShaderVar(m_ShaderProjParams[i], 
                vec4(m_pCamera->getProjection()(0, 0),
                     m_pCamera->getProjection()(1, 1),
                     m_pCamera->getProjection()(2, 2),
                     m_pCamera->getProjection()(2, 3)));
			//m_pPostShader[i]->setShaderVar(m_ShaderProjParams[i], vec2(m_pCamera->getProjection()(2, 2), m_pCamera->getProjection()(2, 3)));
        }
    
        switch (i)
        {
            case LightType_AmbientLight: postAmbientLights(); break;
            case LightType_PointLight: postPointLights(); break;
            case LightType_SpotLight: postSpotLights(); break;
            case LightType_DirectionalLight: postDirectionalLights(); break;
            default: ASSERT("unkown lighttype"); break;
        }

        m_pPostShader[i]->end();
    }
	glCullFace(GL_BACK);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
}
void Deferred3DRenderer::postAmbientLights()
{
    glScissor(0, 0, GRAPHICS->getViewport().width, GRAPHICS->getViewport().height);
    const std::vector<AmbientLight::pointer>& lights(m_pLightManager->getAmbientLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const AmbientLight::pointer& pLight)
    {
        m_pPostShader[LightType_AmbientLight]->setShaderVar(m_ShaderALPos[0], pLight->multiplier);
        m_pPostShader[LightType_AmbientLight]->setShaderVar(m_ShaderALPos[1], pLight->color);
        draw(m_pModel);
    });
}
void Deferred3DRenderer::postPointLights()
{
    const std::vector<PointLight::pointer>& lights(m_pLightManager->getPointLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const PointLight::pointer& pLight)
    {
        /*if (lengthSqr(pLight->position - pCamera->getPosition()) + pLight->endAttenuation * pLight->endAttenuation 
            < m_Settings.getFogEnd() * m_Settings.getFogEnd())
        {*/
            if ( !(dot(normalize(pLight->getPosition() - m_pCamera->getPosition()), m_pCamera->getLook()) < 0 && 
                   length(pLight->getPosition() - m_pCamera->getPosition()) > pLight->getEndAttenuation())) 
            {
                //RectI scissor(pLight->getScissor(pCamera));
                //glScissor(scissor.x, scissor.y, scissor.width, scissor.height);
                m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[0], m_pCamera->getView() * pLight->getPosition());
                m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[1], pLight->getMultiplier());
                m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[2], pLight->getColor());
                m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[3], pLight->getBeginAttenuation());
                m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[4], pLight->getEndAttenuation());
                m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderWVP[LightType_PointLight], m_pCamera->getViewProjection() * pLight->getWorldMatrix());
                //m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderInvMtxProj[LightType_PointLight], m_pCamera->getProjection().inverse());
                //m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderWV[LightType_PointLight], m_pCamera->getView() * pLight->getWorldMatrix());
                //m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderNearFar[LightType_PointLight], vec2(m_pCamera->getNearClip(), m_pCamera->getFarClip()));
                //vec2 topRight(m_pCamera->getNearClip() * tan(0.5f * m_pCamera->getFov()), m_pCamera->getAspectRatio());
                //topRight.y *= topRight.x;
                //m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderTopRight[LightType_PointLight], topRight);
                //m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderWorldPos[LightType_PointLight], mat44::Identity);
                draw(pLight->getLightVolume());
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
        //glScissor(scissor.x, scissor.y, scissor.width, scissor.height);
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[0], m_pCamera->getView() * pLight->getPosition());
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[1], pLight->getMultiplier());
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[2], (m_pCamera->getView() * vec4(pLight->getDirection(), 0)).xyz());
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[3], pLight->getBeginAttenuation());
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[4], pLight->getColor());
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[5], pLight->getEndAttenuation());
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[6], pLight->getCosCutoff());
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderWVP[LightType_SpotLight], m_pCamera->getViewProjection() * pLight->getWorldMatrix());
        //m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderWV[LightType_SpotLight], m_pCamera->getView() * pLight->getWorldMatrix());
        draw(pLight->getLightVolume());
    });
}
void Deferred3DRenderer::postDirectionalLights()
{
    glScissor(0, 0, GRAPHICS->getViewport().width, GRAPHICS->getViewport().height);
    const std::vector<DirectionalLight::pointer>& lights(m_pLightManager->getDirectionalLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const DirectionalLight::pointer& pLight)
    {
        m_pPostShader[LightType_DirectionalLight]->setShaderVar(m_ShaderDLPos[0], pLight->direction);
        m_pPostShader[LightType_DirectionalLight]->setShaderVar(m_ShaderDLPos[1], pLight->color);
        m_pPostShader[LightType_DirectionalLight]->setShaderVar(m_ShaderDLPos[2], pLight->multiplier);
        draw(m_pModel);
    });
}
void Deferred3DRenderer::draw(const Model::pointer& pModel)//, const Camera* pCamera)
{
    if (pModel->isComplete() == false)
        return;
    std::for_each(pModel->cbegin(), pModel->cend(), [&](const ModelMesh::pointer& pMesh)
    {  
        draw(pMesh);
    });
}
void Deferred3DRenderer::draw(const ModelMesh::pointer& pMesh)//, const Camera* pCamera)
{
    if (pMesh->isComplete()) //possible async load
    {
        //if (viewDistanceCheck(pMesh->getBoundingSphere(), pCamera))
        //{
            glBindVertexArray(pMesh->getVertexArraysID());

            glDrawElements(GL_TRIANGLES, pMesh->getNumIndices(), pMesh->getIndexType(), 0);

            glBindVertexArray(0);
        //}
    }
}

LightManager* Deferred3DRenderer::getLightManager() const
{
    return m_pLightManager;
}

} } //end namespace