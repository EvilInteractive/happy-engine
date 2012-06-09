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

namespace he {
    namespace gfx {

AutoExposure::AutoExposure():
    m_LumShader(ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create())), 
    m_FirstBuffer(true),
    m_DTime(0), 
    m_ExposureSpeed(1.0f), 
    m_bOnce(false),
    m_pQuad(nullptr)
{
    ObjectHandle handle1(ResourceFactory<Texture2D>::getInstance()->create());
    ObjectHandle handle2(ResourceFactory<Texture2D>::getInstance()->create());
    m_pLumTexture[0] = ResourceFactory<Texture2D>::getInstance()->get(handle1);
    m_pLumTexture[0]->setName("AutoExposure::m_pLumTexture[0]");
    m_pLumTexture[1] = ResourceFactory<Texture2D>::getInstance()->get(handle2);
    m_pLumTexture[1]->setName("AutoExposure::m_pLumTexture[1]");
}

AutoExposure::~AutoExposure()
{
    m_pLumTexture[0]->release();
    m_pLumTexture[1]->release();
    glDeleteFramebuffers(1, &m_FboID);
    if (GAME != nullptr)
        GAME->removeFromTickList(this);
    if (m_pQuad != nullptr)
        m_pQuad->release();
    m_LumShader->release();
}

void AutoExposure::init(const RenderSettings& settings)
{
    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD RENDER TARGETS                       ///
    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < 2; ++i)
    {
        m_pLumTexture[i]->setData(1, 1, gfx::Texture2D::TextureFormat_R16F, 0, 
            gfx::Texture2D::BufferLayout_R, gfx::Texture2D::BufferType_Float,
            gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Nearest, false, false );
    }

    //////////////////////////////////////////////////////////////////////////
    ///                            LOAD FBO's                              ///
    //////////////////////////////////////////////////////////////////////////
    glGenFramebuffers(1, &m_FboID);
    GL::heBindFbo(m_FboID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pLumTexture[0]->getID(), 0);
    err::checkFboStatus("auto exposure");

    //////////////////////////////////////////////////////////////////////////
    ///                          LOAD SHADERS                              ///
    //////////////////////////////////////////////////////////////////////////
    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    const std::string& folder(CONTENT->getShaderFolderPath().str());
    m_LumShader->initFromFile(folder + "shared/postShaderQuad.vert", 
                               folder + "post/autoLum.frag", shaderLayout);
    m_HDRmapPos = m_LumShader->getShaderSamplerId("hdrMap");
    m_PrevLumMapPos = m_LumShader->getShaderSamplerId("prevLumMap");
    m_DTimePos = m_LumShader->getShaderVarId("dTime");

    m_ExposureSpeed = settings.exposureSpeed;

    //////////////////////////////////////////////////////////////////////////
    ///                         LOAD RENDER QUAD                           ///
    //////////////////////////////////////////////////////////////////////////
    m_pQuad = CONTENT->getFullscreenQuad();

    GAME->addToTickList(this);
}

void AutoExposure::calculate( const Texture2D* pHdrMap)
{
    m_FirstBuffer = !m_FirstBuffer;

    GL::heBlendEnabled(false);
    GL::heBindFbo(m_FboID);
    m_LumShader->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pLumTexture[m_FirstBuffer? 0 : 1]->getID(), 0);
    m_LumShader->setShaderVar(m_HDRmapPos, pHdrMap);
    m_LumShader->setShaderVar(m_PrevLumMapPos, m_pLumTexture[m_FirstBuffer? 1 : 0]);
    m_LumShader->setShaderVar(m_DTimePos, m_DTime);
    GL::heBindVao(m_pQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
}

const Texture2D* AutoExposure::getLuminanceMap() const
{
    return m_pLumTexture[m_FirstBuffer? 0 : 1];
}

void AutoExposure::tick( float dTime )
{
    m_DTime = dTime;
}

} } //end namespace