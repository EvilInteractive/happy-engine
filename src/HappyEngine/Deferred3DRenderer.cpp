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

#include "Deferred3DRenderer.h"
#include "HappyEngine.h"
#include "GL/glew.h"
#include "VertexLayout.h"
#include "Vertex.h"
#include "Assert.h"
#include "Light.h"

#include <vector>

namespace happyengine {
namespace graphics {

const int Deferred3DRenderer::TEXTURE_FORMAT[TEXTURES] = { GL_RGBA, GL_RGBA, GL_RGBA, GL_DEPTH_COMPONENT };
const int Deferred3DRenderer::TEXTURE_INTERNALFORMAT[TEXTURES] = {GL_RGBA8, GL_RGBA16F, GL_RGBA16F, GL_DEPTH_COMPONENT16};
const int Deferred3DRenderer::TEXTURE_ATTACHMENT[TEXTURES] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT};

Deferred3DRenderer::Deferred3DRenderer(): m_pModel(new Model()), m_pLightManager(new LightManager())
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
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, TEXTURE_INTERNALFORMAT[i], 
                     width, height,
                     0, TEXTURE_FORMAT[i], GL_UNSIGNED_BYTE, 0);
        m_pTexture[i] = Texture2D::pointer(new Texture2D(m_TextureId[i], width, height, TEXTURE_FORMAT[i]));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    //Renderbuffers
    glGenRenderbuffers(1, &m_DepthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, TEXTURE_INTERNALFORMAT[3], width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //Framebuffer
    glGenFramebuffers(1, &m_FboId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);

    for (int i = 0; i < TEXTURES; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, TEXTURE_ATTACHMENT[i], GL_TEXTURE_2D, m_TextureId[i], 0);
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferId);

    GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (error != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Woops something went wrong with the framebuffer\n";
        switch (error)
        {
            case GL_FRAMEBUFFER_UNDEFINED: std::cout << "GL_FRAMEBUFFER_UNDEFINED\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: std::cout << "GL_FRAMEBUFFER_UNSUPPORTED\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n"; break;
        }
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE); //disable enable writing to depth buffer
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*------------------------------------------------------------------------------*/
    /*                               LOAD SHADERS                                   */
    /*----------------------------------------------------------------------------- */
    VertexLayout layout;
    layout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, 12, 0, "inPosition"));
    layout.addElement(VertexElement(1, VertexElement::Type_Vector2, VertexElement::Usage_TextureCoordinate, 8, 12, "inTexCoord"));

    for (int i = 0; i < SHADERS; ++i)
    {
        m_pPostShader[i] = new Shader();
    }

    m_pPostShader[0]->init("../data/shaders/deferredPostShader.vert", "../data/shaders/deferredPostALShader.frag", layout);
    m_pPostShader[1]->init("../data/shaders/deferredPostShader.vert", "../data/shaders/deferredPostPLShader.frag", layout);
    m_pPostShader[2]->init("../data/shaders/deferredPostShader.vert", "../data/shaders/deferredPostSLShader.frag", layout);
    m_pPostShader[3]->init("../data/shaders/deferredPostShader.vert", "../data/shaders/deferredPostDLShader.frag", layout);

    for (int i = 0; i < SHADERS; ++i)
    {
        m_ShaderColIllMapPos[i] = m_pPostShader[i]->getShaderSamplerId("colorIllMap");
        m_ShaderPosSpecMapPos[i] = m_pPostShader[i]->getShaderSamplerId("posSpecMap");
        if (i != LightType_AmbientLight)
        {
            m_ShaderNormGlossMapPos[i] = m_pPostShader[i]->getShaderSamplerId("normGlossMap");
            m_ShaderCamPos[i] = m_pPostShader[i]->getShaderVarId("vCamPos");
        }
    }
    //----AL----------------------------------------------------------------------
    m_ShaderALPos[0] = m_pPostShader[LightType_AmbientLight]->getShaderVarId("light.position");
    m_ShaderALPos[1] = m_pPostShader[LightType_AmbientLight]->getShaderVarId("light.multiplier");
    m_ShaderALPos[2] = m_pPostShader[LightType_AmbientLight]->getShaderVarId("light.color");
    m_ShaderALPos[3] = m_pPostShader[LightType_AmbientLight]->getShaderVarId("light.range");
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
    std::vector<VertexPosTex> vertices;
    vertices.push_back(VertexPosTex(math::Vector3(-1, -1, 0.5f), math::Vector2(0, 0)));
    vertices.push_back(VertexPosTex(math::Vector3(1, -1, 0.5f), math::Vector2(1, 0)));
    vertices.push_back(VertexPosTex(math::Vector3(-1, 1, 0.5f), math::Vector2(0, 1)));
    vertices.push_back(VertexPosTex(math::Vector3(1, 1, 0.5f), math::Vector2(1, 1)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    m_pModel->setVertices(&vertices[0], 4, layout);
    m_pModel->setIndices(&indices[0], 6, IndexType_Byte);
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

void Deferred3DRenderer::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
    const static GLenum buffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, buffers);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Deferred3DRenderer::end(const math::Vector3& vCamPos)
{
    const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffers(1, buffers);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    for (int i = 0; i < SHADERS; ++i)
    {
        m_pPostShader[i]->begin();
        
        m_pPostShader[i]->setShaderVar(m_ShaderColIllMapPos[i], m_pTexture[0]);
        m_pPostShader[i]->setShaderVar(m_ShaderPosSpecMapPos[i], m_pTexture[1]);
        if (i != LightType_AmbientLight)
        {
            m_pPostShader[i]->setShaderVar(m_ShaderNormGlossMapPos[i], m_pTexture[2]);
            m_pPostShader[i]->setShaderVar(m_ShaderCamPos[i], vCamPos);
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

    glDisable(GL_BLEND);
}
void Deferred3DRenderer::postAmbientLights()
{
    const std::vector<AmbientLight::pointer>& lights(m_pLightManager->getAmbientLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const AmbientLight::pointer& pLight)
    {
        m_pPostShader[LightType_AmbientLight]->setShaderVar(m_ShaderALPos[0], pLight->position);
        m_pPostShader[LightType_AmbientLight]->setShaderVar(m_ShaderALPos[1], pLight->multiplier);
        m_pPostShader[LightType_AmbientLight]->setShaderVar(m_ShaderALPos[2], pLight->color);
        m_pPostShader[LightType_AmbientLight]->setShaderVar(m_ShaderALPos[3], pLight->range);
        draw(m_pModel);
    });
}
void Deferred3DRenderer::postPointLights()
{
    const std::vector<PointLight::pointer>& lights(m_pLightManager->getPointLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const PointLight::pointer& pLight)
    {
        m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[0], pLight->position);
        m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[1], pLight->multiplier);
        m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[2], pLight->color);
        m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[3], pLight->beginAttenuation);
        m_pPostShader[LightType_PointLight]->setShaderVar(m_ShaderPLPos[4], pLight->endAttenuation);
        draw(m_pModel);
    });
}
void Deferred3DRenderer::postSpotLights()
{
    const std::vector<SpotLight::pointer>& lights(m_pLightManager->getSpotLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const SpotLight::pointer& pLight)
    {
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[0], pLight->position);
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[1], pLight->multiplier);
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[2], pLight->direction);
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[3], pLight->beginAttenuation);
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[4], pLight->color);
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[5], pLight->endAttenuation);
        m_pPostShader[LightType_SpotLight]->setShaderVar(m_ShaderSLPos[6], pLight->cosCutoff);
        draw(m_pModel);
    });
}
void Deferred3DRenderer::postDirectionalLights()
{
    const std::vector<DirectionalLight::pointer>& lights(m_pLightManager->getDirectionalLights());
    std::for_each(lights.cbegin(), lights.cend(), [&](const DirectionalLight::pointer& pLight)
    {
        m_pPostShader[LightType_DirectionalLight]->setShaderVar(m_ShaderDLPos[0], pLight->direction);
        m_pPostShader[LightType_DirectionalLight]->setShaderVar(m_ShaderDLPos[1], pLight->color);
        m_pPostShader[LightType_DirectionalLight]->setShaderVar(m_ShaderDLPos[2], pLight->multiplier);
        draw(m_pModel);
    });
}
void Deferred3DRenderer::draw(const Model::pointer& pModel)
{
    glBindVertexArray(pModel->getVertexArraysID());

    glDrawElements(GL_TRIANGLES, pModel->getNumIndices(), pModel->getIndexType(), 0);

    glBindVertexArray(0);
}

LightManager* Deferred3DRenderer::getLightManager() const
{
    return m_pLightManager;
}

} } //end namespace