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
//Created: 18/12/2011
#include "HappyPCH.h" 

#include "PostProcesser.h"
#include "Shader.h"
#include "Bloom.h"
#include "AutoExposure.h"
#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "CameraManager.h"
#include "Camera.h"

#include "ModelMesh.h"

namespace he {
namespace gfx {

PostProcesser::PostProcesser(): 
    m_pPostShader(nullptr), 
    m_pBloom(nullptr), 
    m_pAutoExposure(nullptr), 
    m_pRandomNormals(nullptr),
    m_pQuad(nullptr)
{
}


PostProcesser::~PostProcesser()
{
    delete m_pPostShader;
    delete m_pBloom;
    delete m_pAutoExposure;

    if (m_pRandomNormals != nullptr)
        m_pRandomNormals->release();

    if (m_pQuad != nullptr)
        m_pQuad->release();
}


void PostProcesser::init( const RenderSettings& settings )
{
    m_pQuad = CONTENT->getFullscreenQuad();
    setSettings(settings);
}

void PostProcesser::setSettings( const RenderSettings& settings )
{
    m_Settings = settings;

    std::set<std::string> postDefines;
    if (m_Settings.enableBloom)
        postDefines.insert("BLOOM");

    if (m_Settings.enableSSAO)
        postDefines.insert("SSAO");

    if (m_Settings.enableDepthEdgeDetect)
        postDefines.insert("DEPTH_EDGE");

    if (m_Settings.enableNormalEdgeDetect)
        postDefines.insert("NORMAL_EDGE");

    if (m_Settings.enableHDR)
        postDefines.insert("HDR");

    if (m_Settings.enableFog)
        postDefines.insert("FOG");

    if (m_Settings.enableVignette)
        postDefines.insert("VIGNETTE");

    delete m_pPostShader;
    m_pPostShader = NEW Shader();

    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    std::string folder(CONTENT->getRootDir() + CONTENT->getShaderFolder());

    m_pPostShader->initFromFile(folder + "shared/postShaderQuad.vert", 
                                folder + "post/postEffects.frag", 
                                shaderLayout, postDefines);

    m_PostShaderVars[PV_ColorMap]  = m_pPostShader->getShaderSamplerId("colorMap"); 
    if (m_Settings.enableSSAO || m_Settings.enableNormalEdgeDetect)
        m_PostShaderVars[PV_NormalMap] = m_pPostShader->getShaderSamplerId("normalMap");
    if (m_Settings.enableFog || m_Settings.enableSSAO || m_Settings.enableDepthEdgeDetect)
        m_PostShaderVars[PV_DepthMap] = m_pPostShader->getShaderSamplerId("depthMap");
    if (m_Settings.enableHDR)
        m_PostShaderVars[PV_LumMap] = m_pPostShader->getShaderSamplerId("lumMap");

    if (m_Settings.enableBloom)
    {
        m_PostShaderVars[PV_Bloom0] = m_pPostShader->getShaderSamplerId("blur0");
        m_PostShaderVars[PV_Bloom1] = m_pPostShader->getShaderSamplerId("blur1");
        m_PostShaderVars[PV_Bloom2] = m_pPostShader->getShaderSamplerId("blur2");
        m_PostShaderVars[PV_Bloom3] = m_pPostShader->getShaderSamplerId("blur3");
    }
    if (m_pRandomNormals != nullptr)
        m_pRandomNormals->release();
    if (m_Settings.enableSSAO)
    {
        m_pRandomNormals = CONTENT->asyncLoadTexture("engine/random_normals.png");

        m_PostShaderVars[PV_SSAORandomNormals] = m_pPostShader->getShaderSamplerId("randomNormals");
        m_PostShaderVars[PV_SSAORadius] = m_pPostShader->getShaderVarId("radius");
        m_PostShaderVars[PV_SSAOIntensity] = m_pPostShader->getShaderVarId("intensity");
        m_PostShaderVars[PV_SSAOScale] = m_pPostShader->getShaderVarId("scale");
        m_PostShaderVars[PV_SSAOBias] = m_pPostShader->getShaderVarId("bias");
        m_PostShaderVars[PV_ProjParams] = m_pPostShader->getShaderVarId("projParams");
        m_PostShaderVars[PV_ViewPortSize] = m_pPostShader->getShaderVarId("viewPortSize");
    }

    if (m_Settings.enableBloom)
    {
        delete m_pBloom;
        m_pBloom = NEW Bloom();
        m_pBloom->init(m_Settings.enableHDR);
    }
    if (m_Settings.enableHDR)
    {
        delete m_pAutoExposure;
        m_pAutoExposure = NEW AutoExposure();
        m_pAutoExposure->init(settings);
    }
}

void PostProcesser::draw(const Texture2D* pColorMap, const Texture2D* pNormalMap, const Texture2D* pDepthMap)
{    
    if (m_Settings.enableHDR)
    {
        PROFILER_BEGIN("Auto Exposure");
        m_pAutoExposure->calculate(pColorMap);
        PROFILER_END();
    }
    
    GL::heBlendEnabled(false);
    if (m_Settings.enableBloom)
    {
        PROFILER_BEGIN("Bloom");
        if (m_Settings.enableHDR)
            m_pBloom->render(pColorMap, m_pAutoExposure->getLuminanceMap());
        else
            m_pBloom->render(pColorMap);
        PROFILER_END();
    }

    GL::heBindFbo(0);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);
    GL::heSetCullFace(false);
    const static GLenum buffers[1] = { GL_BACK_LEFT };
    glDrawBuffers(1, buffers);

    m_pPostShader->bind();

    m_pPostShader->setShaderVar(m_PostShaderVars[PV_ColorMap], pColorMap);
    if (m_Settings.enableBloom)
    {
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_Bloom0], m_pBloom->getBloom(0));
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_Bloom1], m_pBloom->getBloom(1));
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_Bloom2], m_pBloom->getBloom(2));
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_Bloom3], m_pBloom->getBloom(3));
    }
    if (m_Settings.enableHDR)
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_LumMap], m_pAutoExposure->getLuminanceMap());

    if (m_Settings.enableSSAO || m_Settings.enableNormalEdgeDetect)
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_NormalMap], pNormalMap); 
    if (m_Settings.enableFog || m_Settings.enableSSAO || m_Settings.enableDepthEdgeDetect)
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_DepthMap], pDepthMap);

    if (m_Settings.enableSSAO)
    {
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAORandomNormals], m_pRandomNormals);
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAORadius], m_Settings.ssaoSettings.radius);
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAOIntensity], m_Settings.ssaoSettings.intensity);
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAOScale], m_Settings.ssaoSettings.scale);
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAOBias], m_Settings.ssaoSettings.bias);

        m_pPostShader->setShaderVar(m_PostShaderVars[PV_ProjParams], vec4(
            CAMERAMANAGER->getActiveCamera()->getProjection()(0, 0),
            CAMERAMANAGER->getActiveCamera()->getProjection()(1, 1),
            CAMERAMANAGER->getActiveCamera()->getProjection()(2, 2),
            CAMERAMANAGER->getActiveCamera()->getProjection()(2, 3)));
        m_pPostShader->setShaderVar(m_PostShaderVars[PV_ViewPortSize], 
            vec2((float)GRAPHICS->getViewport().width, (float)GRAPHICS->getViewport().height));
    }

    GL::heBindVao(m_pQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
}

void PostProcesser::onScreenResized()
{
    if (m_pBloom != nullptr)
        m_pBloom->resize();
}


} } //end namespace
