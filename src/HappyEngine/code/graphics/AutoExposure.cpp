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
//Created: 26/10/2011

#include "HappyPCH.h" 

#include "AutoExposure.h"

#include "ContentManager.h"
#include "GraphicsEngine.h"
#include "Game.h"
#include "ModelMesh.h"
#include "Texture2D.h"
#include "ExternalError.h"

namespace he {
    namespace gfx {

AutoExposure::AutoExposure():
    m_LumShader(nullptr), 
    m_FirstBuffer(true),
    m_DTime(0), 
    m_ExposureSpeed(1.0f), 
    m_bOnce(false),
    m_Quad(nullptr)
{
    ObjectHandle handle1(ResourceFactory<Texture2D>::getInstance()->create());
    ObjectHandle handle2(ResourceFactory<Texture2D>::getInstance()->create());
    m_LumTexture[0] = ResourceFactory<Texture2D>::getInstance()->get(handle1);
    m_LumTexture[0]->setName("AutoExposure::m_LumTexture[0]");
    m_LumTexture[0]->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest,
            gfx::TextureFormat_R16F, false);
    m_LumTexture[1] = ResourceFactory<Texture2D>::getInstance()->get(handle2);
    m_LumTexture[1]->setName("AutoExposure::m_LumTexture[1]");
    m_LumTexture[1]->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Nearest,
            gfx::TextureFormat_R16F, false);
}

AutoExposure::~AutoExposure()
{
    m_LumTexture[0]->release();
    m_LumTexture[1]->release();
    glDeleteFramebuffers(1, &m_FboID);
    if (GAME != nullptr)
        GAME->removeFromTickList(this);
    if (m_Quad != nullptr)
        m_Quad->release();
    m_LumShader->release();
}

void AutoExposure::init(const PostSettings::HdrSettings& settings)
{
    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 2; ++i)
    {
        m_LumTexture[i]->setData(1, 1, 0, 
            gfx::TextureBufferLayout_R, gfx::TextureBufferType_Float, 0);
    }

    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////
    glGenFramebuffers(1, &m_FboID);
    GL::heBindFbo(m_FboID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LumTexture[0]->getID(), 0);
    err::checkFboStatus("auto exposure");

    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD SHADERS                              ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout shaderLayout;
    shaderLayout.addAttribute(ShaderLayoutAttribute("inPosition", BufferElement::Usage_Position));

    he::ObjectList<he::String> shaderOutputs;
    shaderOutputs.add("outColor");

    m_LumShader = ResourceFactory<gfx::Shader>::getInstance()->get(
        CONTENT->loadShader("shared/postShaderQuad.vert", 
                            "post/autoLum.frag", shaderLayout, shaderOutputs));
    m_HDRmapPos = m_LumShader->getShaderSamplerId(HEFS::strhdrMap);
    m_PrevLumMapPos = m_LumShader->getShaderSamplerId(HEFS::strprevLumMap);
    m_DTimePos = m_LumShader->getShaderVarId(HEFS::strdTime);

    m_ExposureSpeed = settings.exposureSpeed;

    //////////////////////////////////////////////////////////////////////////
    ///                         LOAD RENDER QUAD                           ///
    //////////////////////////////////////////////////////////////////////////
    m_Quad = CONTENT->getFullscreenQuad();

    GAME->addToTickList(this);
}

void AutoExposure::calculate( const Texture2D* hdrMap)
{
    m_FirstBuffer = !m_FirstBuffer;

    GL::heBlendEnabled(false);
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);
    GL::heBindFbo(m_FboID);
    GL::heSetViewport(RectI(0, 0, 1, 1));
    GL::heSetCullFace(false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LumTexture[m_FirstBuffer? 0 : 1]->getID(), 0);
    m_LumShader->bind();
    m_LumShader->setShaderVar(m_HDRmapPos, hdrMap);
    m_LumShader->setShaderVar(m_PrevLumMapPos, m_LumTexture[m_FirstBuffer? 1 : 0]);
    m_LumShader->setShaderVar(m_DTimePos, m_DTime * m_ExposureSpeed);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_Quad->getVBOID());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(12)); 
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Quad->getVBOIndexID());

    glDrawElements(GL_TRIANGLES, m_Quad->getNumIndices(), m_Quad->getIndexType(), 0);
}

const Texture2D* AutoExposure::getLuminanceMap() const
{
    return m_LumTexture[m_FirstBuffer? 0 : 1];
}

void AutoExposure::tick( float dTime )
{
    m_DTime = dTime;
}

} } //end namespace
