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
//Created: 14/10/2011

#include "StdAfx.h" 

#include "DownSampler.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "ContentManager.h"
#include "GraphicsEngine.h"
#include "Vertex.h"

#include "GL/glew.h"

namespace he {
namespace gfx {

DownSampler::DownSampler(): m_pMesh(NEW ModelMesh("DownSamplerQuad"))
{
}


DownSampler::~DownSampler()
{
}

void DownSampler::init(byte downScales)
{
    //////////////////////////////////////////////////////////////////////////
    ///                             Textures                               ///
    //////////////////////////////////////////////////////////////////////////
    m_TextureId.resize(downScales);
    m_Texture.resize(downScales);
    m_FboId.resize(downScales);


    glGenTextures(downScales, &m_TextureId[0]);
    for (int i = 0; i < downScales; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, m_TextureId[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 
            GRAPHICS->getScreenWidth() / ((i+1) * 2), GRAPHICS->getScreenHeight() / ((i+1) * 2), 
            0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        m_Texture[i] = Texture2D::pointer(NEW Texture2D());
        m_Texture[i]->init(m_TextureId[i], GRAPHICS->getScreenWidth() / ((i+1) * 2), GRAPHICS->getScreenHeight() / ((i+1) * 2), GL_RGBA16F);
    }
    glBindTexture(GL_TEXTURE_2D, 0);


    //////////////////////////////////////////////////////////////////////////
    ///                               Fbo                                  ///
    //////////////////////////////////////////////////////////////////////////
    glGenFramebuffers(downScales, &m_FboId[0]);

    for (int i = 0; i < downScales; ++i)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FboId[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId[i], 0);
        
        GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (err != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Woops something went wrong with a downSampler framebuffer\n";
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
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //////////////////////////////////////////////////////////////////////////
    ///                             Shaders                                ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));
    m_pDownSamplerShader = CONTENT->loadShader("deferred/post/deferredPostShaderQuad.vert", 
                                               "deferred/post/downScale.frag", layout,
                                                std::vector<std::string>());
    m_TexMapPos = m_pDownSamplerShader->getShaderSamplerId("texMap");
    m_InvScalePos = m_pDownSamplerShader->getShaderVarId("invScale");

    //////////////////////////////////////////////////////////////////////////
    ///                             Quad                                   ///
    //////////////////////////////////////////////////////////////////////////
    std::vector<VertexPos> vertices;
    vertices.push_back(VertexPos(vec3(-1, 1, 1.0f)));
    vertices.push_back(VertexPos(vec3(1, 1, 1.0f)));
    vertices.push_back(VertexPos(vec3(-1, -1, 1.0f)));
    vertices.push_back(VertexPos(vec3(1, -1, 1.0f)));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(1); indices.push_back(3); indices.push_back(2);

    VertexLayout vLayout;
    vLayout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, 12, 0));

    m_pMesh->init();
    m_pMesh->setVertices(&vertices[0], 4, vLayout);
    m_pMesh->setIndices(&indices[0], 6, IndexStride_Byte);
}

byte DownSampler::getDownSampleLevels() const
{
    return static_cast<byte>(m_FboId.size());
}

const Texture2D::pointer& DownSampler::getSample( byte downSampleLevel )
{
    ASSERT(downSampleLevel < m_FboId.size());
    return m_Texture[downSampleLevel];
}

void DownSampler::downSample( const Texture2D::pointer& pTexture )
{
    glBindTexture(GL_TEXTURE_2D, pTexture->getID());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_pDownSamplerShader->begin();
    glBindVertexArray(m_pMesh->getVertexArraysID());
    for (uint fboId = 0; fboId < m_FboId.size(); ++fboId)
    {
         /*if (fboId != 0)
         {
             glBindTexture(GL_TEXTURE_2D, m_Texture[fboId - 1]->getID());
             glGenerateMipmap(GL_TEXTURE_2D);
             glBindTexture(GL_TEXTURE_2D, 0);
         }*/

        glBindFramebuffer(GL_FRAMEBUFFER, m_FboId[fboId]);
        if (fboId == 0)
            m_pDownSamplerShader->setShaderVar(m_TexMapPos, pTexture);
        else
            m_pDownSamplerShader->setShaderVar(m_TexMapPos, m_Texture[fboId - 1]);
        m_pDownSamplerShader->setShaderVar(m_InvScalePos, (fboId + 1) * 2.0f);

        glDrawElements(GL_TRIANGLES, m_pMesh->getNumIndices(), m_pMesh->getIndexType(), 0);
    }
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_pDownSamplerShader->end();
}



} } //end namespace