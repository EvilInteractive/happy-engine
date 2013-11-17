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
#include "CameraPerspective.h"
#include "Renderer2D.h"
#include "Canvas2D.h"
#include "View.h"
#include "Window.h"
#include "GlobalSettings.h"

#include "ModelMesh.h"
#include "RenderTarget.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

PostProcesser::PostProcesser(): 
    m_PostShader(nullptr), 
    m_Bloom(nullptr), 
    m_AutoExposure(nullptr), 
    m_RandomNormals(nullptr),
    m_Quad(nullptr),
    m_ShowDebugTextures(true),
    m_FogColor(0.2f, 0.4f, 0.6f),
    m_View(nullptr),
    m_DebugRenderer(nullptr),
    m_ToneMapUniformBuffer(nullptr),
    m_WriteRenderTarget(nullptr),
    m_ReadRenderTarget(nullptr),
    m_AOEnabled(false),
    m_FogEnabled(false)
{
}


PostProcesser::~PostProcesser()
{
    if (m_PostShader != nullptr)
        m_PostShader->release();
    delete m_Bloom;
    delete m_AutoExposure;
    delete m_ToneMapUniformBuffer;

    if (m_RandomNormals != nullptr)
        m_RandomNormals->release();

    if (m_Quad != nullptr)
        m_Quad->release();
}


void PostProcesser::init( View* view, const RenderTarget* writeTarget, const RenderTarget* readTarget )
{
    HE_IF_ASSERT(m_View == nullptr, "PostProcessor is already initialized")
    {
        m_WriteRenderTarget = writeTarget;
        m_ReadRenderTarget = readTarget;
        m_View = view;
                        
        m_Quad = CONTENT->getFullscreenQuad();
        
        initFromSettings();
    }
}

void PostProcesser::initFromSettings()
{    
    const RenderSettings& settings(GlobalSettings::getInstance()->getRenderSettings());

    if (settings.postSettings.shaderSettings.enableHDR)
    {
        m_ToneMapUniformBuffer = NEW UniformBuffer(sizeof(ToneMapData));
    }

    if (settings.postSettings.shaderSettings.enableBloom)
    {
        m_Bloom = NEW Bloom();
        m_Bloom->init(m_View, settings.postSettings.shaderSettings.enableHDR, m_ToneMapUniformBuffer);
    }
    if (settings.postSettings.shaderSettings.enableHDR)
    {
        m_AutoExposure = NEW AutoExposure();
        GRAPHICS->setActiveContext(m_View->getWindow()->getContext());
        m_AutoExposure->init(settings.postSettings.hdrSettings);
    }
    m_AOEnabled = settings.postSettings.shaderSettings.enableAO;
    m_FogEnabled = settings.postSettings.shaderSettings.enableFog;
    compileShader();
}
void PostProcesser::compileShader()
{
    std::set<he::String> postDefines;
    const RenderSettings& renderSettings(GlobalSettings::getInstance()->getRenderSettings());
    const PostSettings::ShaderSettings& settings(renderSettings.postSettings.shaderSettings);

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

    const he::String& folder(CONTENT->getShaderFolderPath().str());

    m_PostShader->initFromFile(folder + "shared/postShaderQuad.vert", 
        folder + "post/postEffects.frag", 
        shaderLayout, postDefines);

    m_PostShaderVars[PV_ColorMap]  = m_PostShader->getShaderSamplerId("colorMap"); 
    if (settings.enableAO || settings.enableFog)
        m_PostShaderVars[PV_NormalDepthMap] = m_PostShader->getShaderSamplerId("normalDepthMap");
    if (settings.enableFog)
        m_PostShaderVars[PV_FogColor] = m_PostShader->getShaderVarId("fogColor");
    if (settings.enableHDR)
    {
        m_PostShaderVars[PV_LumMap] = m_PostShader->getShaderSamplerId("lumMap");
        m_PostShaderVars[PV_ToneMapData] = m_PostShader->getBufferId("SharedToneMapBuffer");
        m_PostShader->setBuffer(m_PostShaderVars[PV_ToneMapData], m_ToneMapUniformBuffer);
    }

    if (settings.enableBloom)
    {
        m_PostShaderVars[PV_Bloom0] = m_PostShader->getShaderSamplerId("blur0");
        m_PostShaderVars[PV_Bloom1] = m_PostShader->getShaderSamplerId("blur1");
        m_PostShaderVars[PV_Bloom2] = m_PostShader->getShaderSamplerId("blur2");
        m_PostShaderVars[PV_Bloom3] = m_PostShader->getShaderSamplerId("blur3");
    }
    if (m_RandomNormals != nullptr)
        m_RandomNormals->release();
    if (settings.enableAO)
    {
        m_RandomNormals = CONTENT->asyncLoadTexture2D("engine/noise.png");

        m_PostShaderVars[PV_SSAORandomNormals] = m_PostShader->getShaderSamplerId("noiseMap");
        m_PostShaderVars[PV_ViewPortSize] = m_PostShader->getShaderVarId("viewPortSize");
    }
}


void PostProcesser::draw()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    const Texture2D* colorMap(m_ReadRenderTarget->getTextureTarget(0));
    const Texture2D* normalDepthMap(m_ReadRenderTarget->getTextureTarget(1));

    if (nullptr != m_AutoExposure)
    {
        PROFILER_BEGIN("Auto Exposure");
        m_ToneMapUniformBuffer->uploadData(&m_ToneMapData, sizeof(ToneMapData));
        m_AutoExposure->calculate(colorMap);
        PROFILER_END();
    }
    
    GL::heBlendEnabled(false);
    if (nullptr != m_Bloom)
    {
        PROFILER_BEGIN("Bloom");
        if (nullptr != m_AutoExposure)
            m_Bloom->render(colorMap, m_AutoExposure->getLuminanceMap());
        else
            m_Bloom->render(colorMap);
        PROFILER_END();
    }
    
    PROFILER_BEGIN("Post");
    m_WriteRenderTarget->prepareForRendering();
    GL::heBlendEnabled(false);
    GL::heSetDepthWrite(false);
    GL::heSetDepthRead(false);
    GL::heSetCullFace(false);
    GL::heSetViewport(m_View->getViewport());

    m_PostShader->bind();

    m_PostShader->setShaderVar(m_PostShaderVars[PV_ColorMap], colorMap);
    if (nullptr != m_Bloom)
    {
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom0], m_Bloom->getBloom(0));
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom1], m_Bloom->getBloom(1));
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom2], m_Bloom->getBloom(2));
        m_PostShader->setShaderVar(m_PostShaderVars[PV_Bloom3], m_Bloom->getBloom(3));
    }
    if (nullptr != m_AutoExposure)
        m_PostShader->setShaderVar(m_PostShaderVars[PV_LumMap], m_AutoExposure->getLuminanceMap());

    if (m_AOEnabled || m_FogEnabled)
        m_PostShader->setShaderVar(m_PostShaderVars[PV_NormalDepthMap], normalDepthMap); 
    if (m_FogEnabled)
        m_PostShader->setShaderVar(m_PostShaderVars[PV_FogColor], m_FogColor);

    if (m_AOEnabled)
    {
        m_PostShader->setShaderVar(m_PostShaderVars[PV_SSAORandomNormals], m_RandomNormals);
        m_PostShader->setShaderVar(m_PostShaderVars[PV_ViewPortSize], 
            vec2((float)m_View->getViewport().width, (float)m_View->getViewport().height));
    }

    GL::heBindVao(m_Quad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_Quad->getNumIndices(), m_Quad->getIndexType(), 0);
    PROFILER_END();
}

void PostProcesser::draw2D(gui::Canvas2D* canvas)
{
    if (m_ShowDebugTextures)
    {
        float height(72.0f);
        float aspect(m_Bloom->getBloom(0)->getWidth() / (float)m_Bloom->getBloom(0)->getHeight());
        float width(aspect * height);
        canvas->blitImage(m_Bloom->getBloom(0), vec2(12 * 1 + width * 0, height), false, vec2(width, height));
        canvas->blitImage(m_Bloom->getBloom(1), vec2(12 * 2 + width * 1, height), false, vec2(width, height));
        canvas->blitImage(m_Bloom->getBloom(2), vec2(12 * 3 + width * 2, height), false, vec2(width, height));
        canvas->blitImage(m_Bloom->getBloom(3), vec2(12 * 4 + width * 3, height), false, vec2(width, height));
        if (nullptr != m_AutoExposure)
            canvas->drawImage(m_AutoExposure->getLuminanceMap(), vec2(12 * 5 + width * 4, 12), vec2(height, height));
    }
}

void PostProcesser::setFogColor( const he::vec3& color )
{
    m_FogColor = color;
}

void PostProcesser::setDebugRenderer( Renderer2D* renderer )
{
    m_DebugRenderer = renderer;
    m_DebugRenderer->attachToRender(this);
}


} } //end namespace
