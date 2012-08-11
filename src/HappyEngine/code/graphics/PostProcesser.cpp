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
#include "Renderer2D.h"
#include "View.h"

#include "ModelMesh.h"
#include "RenderTarget.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

PostProcesser::PostProcesser(): 
    m_PostShader(nullptr), 
    m_pBloom(nullptr), 
    m_pAutoExposure(nullptr), 
    m_pRandomNormals(nullptr),
    m_pQuad(nullptr),
    m_ShowDebugTextures(false),
    m_FogColor(0.2f, 0.4f, 0.6f)
{
}


PostProcesser::~PostProcesser()
{
    if (m_PostShader != nullptr)
        m_PostShader->release();
    delete m_pBloom;
    delete m_pAutoExposure;

    if (m_pRandomNormals != nullptr)
        m_pRandomNormals->release();

    if (m_pQuad != nullptr)
        m_pQuad->release();

    m_View->get2DRenderer()->detachFromRender(this);
}


void PostProcesser::init( View* view, const RenderTarget* writeTarget, const RenderTarget* readTarget )
{
    HE_IF_ASSERT(m_View == nullptr, "PostProcessor is already initialized")
    {
        m_WriteRenderTarget = writeTarget;
        m_ReadRenderTarget = readTarget;
        m_View = view;

        eventCallback0<void> handler([&](){ onSettingsChanged(m_View->getSettings(), false); });
        m_View->SettingsChanged += handler;  // this is safe because PostProcessor is a member of View

        m_View->get2DRenderer()->attachToRender(this);
        
        m_pQuad = CONTENT->getFullscreenQuad();
        
        onSettingsChanged(m_View->getSettings(), true);

        CONSOLE->registerVar(&m_ShowDebugTextures, "debugPostTex");
    }
}

void PostProcesser::onSettingsChanged( const RenderSettings& settings, bool force )
{
    bool recompileShader(force || settings.postSettings.shaderSettings != m_Settings.postSettings.shaderSettings);
    
    if (force || settings.postSettings.shaderSettings.enableBloom != m_Settings.postSettings.shaderSettings.enableBloom)
    {
        delete m_pBloom;
        if (settings.postSettings.shaderSettings.enableBloom)
        {
            m_pBloom = NEW Bloom();
            m_pBloom->init(m_View, settings.postSettings.shaderSettings.enableHDR);
        }
        else
            m_pBloom = nullptr;
    }
    if (force || settings.postSettings.shaderSettings.enableHDR != m_Settings.postSettings.shaderSettings.enableHDR)
    {
        delete m_pAutoExposure;
        if (settings.postSettings.shaderSettings.enableHDR)
        {
            m_pAutoExposure = NEW AutoExposure();
            m_pAutoExposure->init(settings.postSettings.hdrSettings);
        }
        else 
            m_pAutoExposure = nullptr;
    }

    m_Settings = settings;

    if (recompileShader == true)
        compileShader();
}
void PostProcesser::compileShader()
{
    std::set<std::string> postDefines;
    const PostSettings::ShaderSettings& settings(m_Settings.postSettings.shaderSettings);

    if (settings.enableBloom)
        postDefines.insert("BLOOM");

    if (settings.enableAO)
        postDefines.insert("AO");

    if (settings.enableDepthEdgeDetect)
        postDefines.insert("DEPTH_EDGE");

    if (settings.enableNormalEdgeDetect)
        postDefines.insert("NORMAL_EDGE");

    if (settings.enableHDR)
        postDefines.insert("HDR");

    if (settings.enableFog)
        postDefines.insert("FOG");

    if (settings.enableVignette)
        postDefines.insert("VIGNETTE");

    if (m_PostShader != nullptr)
        m_PostShader->release();
    m_PostShader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());

    ShaderLayout shaderLayout;
    shaderLayout.addElement(ShaderLayoutElement(0, "inPosition"));

    const std::string& folder(CONTENT->getShaderFolderPath().str());

    m_PostShader->initFromFile(folder + "shared/postShaderQuad.vert", 
        folder + "post/postEffects.frag", 
        shaderLayout, postDefines);

    m_PostShaderVars[PV_ColorMap]  = m_PostShader->getShaderSamplerId("colorMap"); 
    if (settings.enableNormalEdgeDetect)
        m_PostShaderVars[PV_NormalMap] = m_PostShader->getShaderSamplerId("normalMap");
    if (settings.enableFog)
        m_PostShaderVars[PV_FogColor] = m_PostShader->getShaderVarId("fogColor");
    if (settings.enableFog || settings.enableAO || settings.enableDepthEdgeDetect)
        m_PostShaderVars[PV_DepthMap] = m_PostShader->getShaderSamplerId("depthMap");
    if (settings.enableHDR)
        m_PostShaderVars[PV_LumMap] = m_PostShader->getShaderSamplerId("lumMap");

    if (settings.enableBloom)
    {
        m_PostShaderVars[PV_Bloom0] = m_PostShader->getShaderSamplerId("blur0");
        m_PostShaderVars[PV_Bloom1] = m_PostShader->getShaderSamplerId("blur1");
        m_PostShaderVars[PV_Bloom2] = m_PostShader->getShaderSamplerId("blur2");
        m_PostShaderVars[PV_Bloom3] = m_PostShader->getShaderSamplerId("blur3");
    }
    if (m_pRandomNormals != nullptr)
        m_pRandomNormals->release();
    if (settings.enableAO)
    {
        m_pRandomNormals = CONTENT->asyncLoadTexture("engine/noise.png");

        m_PostShaderVars[PV_SSAORandomNormals] = m_PostShader->getShaderSamplerId("noiseMap");
        //m_PostShaderVars[PV_SSAORadius] = m_pPostShader->getShaderVarId("radius");
        //m_PostShaderVars[PV_SSAOIntensity] = m_pPostShader->getShaderVarId("intensity");
        //m_PostShaderVars[PV_SSAOScale] = m_pPostShader->getShaderVarId("scale");
        //m_PostShaderVars[PV_SSAOBias] = m_pPostShader->getShaderVarId("bias");
        //m_PostShaderVars[PV_ProjParams] = m_pPostShader->getShaderVarId("projParams");
        m_PostShaderVars[PV_ViewPortSize] = m_PostShader->getShaderVarId("viewPortSize");
    }
}


void PostProcesser::draw()
{    
    const Texture2D* colorMap(m_ReadRenderTarget->getTextureTarget(0));
    const Texture2D* normalMap(m_ReadRenderTarget->getTextureTarget(1));
    const Texture2D* depthMap(m_ReadRenderTarget->getDepthTarget());

    const PostSettings::ShaderSettings& settings(m_Settings.postSettings.shaderSettings);
    if (settings.enableHDR)
    {
        PROFILER_BEGIN("Auto Exposure");
        m_pAutoExposure->calculate(colorMap);
        PROFILER_END();
    }
    
    GL::heBlendEnabled(false);
    if (settings.enableBloom)
    {
        PROFILER_BEGIN("Bloom");
        if (settings.enableHDR)
            m_pBloom->render(colorMap, m_pAutoExposure->getLuminanceMap());
        else
            m_pBloom->render(colorMap);
        PROFILER_END();
    }

    m_WriteRenderTarget->prepareForRendering();
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);
    GL::heSetCullFace(false);

    m_PostShader->bind();

    m_PostShader->setShaderVar(m_PostShaderVars[PV_ColorMap], colorMap);
    if (settings.enableBloom)
    {
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom0], m_pBloom->getBloom(0));
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom1], m_pBloom->getBloom(1));
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom2], m_pBloom->getBloom(2));
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom3], m_pBloom->getBloom(3));
    }
    if (settings.enableHDR)
        m_PostShader->setShaderVar(m_PostShaderVars[PV_LumMap], m_pAutoExposure->getLuminanceMap());

    if (settings.enableNormalEdgeDetect)
        m_PostShader->setShaderVar(m_PostShaderVars[PV_NormalMap], normalMap); 
    if (settings.enableFog)
        m_PostShader->setShaderVar(m_PostShaderVars[PV_FogColor], m_FogColor);
    if (settings.enableFog || settings.enableAO || settings.enableDepthEdgeDetect)
        m_PostShader->setShaderVar(m_PostShaderVars[PV_DepthMap], depthMap);

    if (settings.enableAO)
    {
        m_PostShader->setShaderVar(m_PostShaderVars[PV_SSAORandomNormals], m_pRandomNormals);
        //m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAORadius], m_Settings.ssaoSettings.radius);
        //m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAOIntensity], m_Settings.ssaoSettings.intensity);
        //m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAOScale], m_Settings.ssaoSettings.scale);
        //m_pPostShader->setShaderVar(m_PostShaderVars[PV_SSAOBias], m_Settings.ssaoSettings.bias);

        /*m_pPostShader->setShaderVar(m_PostShaderVars[PV_ProjParams], vec4(
            CAMERAMANAGER->getActiveCamera()->getProjection()(0, 0),
            CAMERAMANAGER->getActiveCamera()->getProjection()(1, 1),
            CAMERAMANAGER->getActiveCamera()->getProjection()(2, 2),
            CAMERAMANAGER->getActiveCamera()->getProjection()(2, 3)));*/
        m_PostShader->setShaderVar(m_PostShaderVars[PV_ViewPortSize], 
            vec2((float)m_View->getViewport().width, (float)m_View->getViewport().height));
    }

    GL::heBindVao(m_pQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pQuad->getNumIndices(), m_pQuad->getIndexType(), 0);
}

void PostProcesser::draw2D(Renderer2D* renderer)
{
    if (m_ShowDebugTextures)
    {
        renderer->drawTexture2DToScreen(m_pBloom->getBloom(0), vec2(12 * 1 + 256 * 0, 12), false, vec2(256, 144));
        renderer->drawTexture2DToScreen(m_pBloom->getBloom(1), vec2(12 * 2 + 256 * 1, 12), false, vec2(256, 144));
        renderer->drawTexture2DToScreen(m_pBloom->getBloom(2), vec2(12 * 3 + 256 * 2, 12), false, vec2(256, 144));
        renderer->drawTexture2DToScreen(m_pBloom->getBloom(3), vec2(12 * 4 + 256 * 3, 12), false, vec2(256, 144));
    }
}

void PostProcesser::setFogColor( const he::vec3& color )
{
    m_FogColor = color;
}


} } //end namespace
