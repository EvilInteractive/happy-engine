//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
#include "MaterialInstance.h"
#include "DrawContext.h"

#include "ModelMesh.h"
#include "RenderTarget.h"
#include "Texture2D.h"

namespace he {
namespace gfx {

PostProcesser::PostProcesser(): 
    m_PostMaterial(nullptr), 
    m_Bloom(nullptr), 
    m_AutoExposure(nullptr), 
    m_RandomNormals(nullptr),
    m_Quad(nullptr),
    m_FogColor(0.2f, 0.4f, 0.6f),
    m_View(nullptr),
    m_DebugRenderer(nullptr),
    m_WriteRenderTarget(nullptr),
    m_ReadRenderTarget(nullptr),
    m_ShowDebugTextures(false),
    m_AOEnabled(false),
    m_FogEnabled(false)
{
}


PostProcesser::~PostProcesser()
{
    HEDelete(m_PostMaterial);
    HEDelete(m_Bloom);
    HEDelete(m_AutoExposure);

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

    if (settings.postSettings.shaderSettings.enableBloom)
    {
        m_Bloom = HENew(Bloom)();
        m_Bloom->init(m_View, settings.postSettings.shaderSettings.enableHDR);
    }
    if (settings.postSettings.shaderSettings.enableHDR)
    {
        m_AutoExposure = HENew(AutoExposure)();
        m_AutoExposure->init(m_View, settings.postSettings.hdrSettings);
    }
    m_AOEnabled = settings.postSettings.shaderSettings.enableAO;
    m_FogEnabled = settings.postSettings.shaderSettings.enableFog;
    loadMaterial();
}
void PostProcesser::loadMaterial()
{
    he::ObjectList<he::String> postDefines;
    const RenderSettings& renderSettings(GlobalSettings::getInstance()->getRenderSettings());
    const PostSettings::ShaderSettings& settings(renderSettings.postSettings.shaderSettings);

    // Load Material
    {
        HEDelete(m_PostMaterial);
        Material* const postMat(CONTENT->loadMaterial("engine/post/post.hm"));
        m_PostMaterial = postMat->createMaterialInstance(eShaderRenderType_Normal);
        postMat->release();
        m_PostMaterial->calculateMaterialLayout(m_Quad->getVertexLayout());
    }

    m_PostShaderVars[PV_ColorMap]  = m_PostMaterial->findParameter(HEFS::strcolorMap); 
    if (settings.enableAO || settings.enableFog)
        m_PostShaderVars[PV_NormalDepthMap] = m_PostMaterial->findParameter(HEFS::strnormalDepthMap);
    if (settings.enableFog)
        m_PostShaderVars[PV_FogColor] = m_PostMaterial->findParameter(HEFS::strfogColor);
    if (settings.enableHDR)
    {
        m_PostShaderVars[PV_LumMap] = m_PostMaterial->findParameter(HEFS::strlumMap);
    }

    if (settings.enableBloom)
    {
        m_PostShaderVars[PV_Bloom0] = m_PostMaterial->findParameter(HEFS::strblur0);
        m_PostShaderVars[PV_Bloom1] = m_PostMaterial->findParameter(HEFS::strblur1);
        m_PostShaderVars[PV_Bloom2] = m_PostMaterial->findParameter(HEFS::strblur2);
        m_PostShaderVars[PV_Bloom3] = m_PostMaterial->findParameter(HEFS::strblur3);
    }
    if (m_RandomNormals != nullptr)
        m_RandomNormals->release();
    if (settings.enableAO)
    {
        m_RandomNormals = CONTENT->asyncLoadTexture2D("engine/noise.png");

        m_PostShaderVars[PV_SSAORandomNormals] = m_PostMaterial->findParameter(HEFS::strnoiseMap);
        m_PostShaderVars[PV_ViewPortSize] = m_PostMaterial->findParameter(HEFS::strviewPortSize);
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
        m_AutoExposure->calculate(colorMap);
        PROFILER_END();
    }
    
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
    GL::heSetViewport(m_View->getViewport());

    m_PostMaterial->getParameter(m_PostShaderVars[PV_ColorMap]).setTexture2D(colorMap);
    if (nullptr != m_Bloom)
    {
        m_PostMaterial->getParameter(m_PostShaderVars[PV_Bloom0]).setTexture2D(m_Bloom->getBloom(0));
        m_PostMaterial->getParameter(m_PostShaderVars[PV_Bloom1]).setTexture2D(m_Bloom->getBloom(1));
        m_PostMaterial->getParameter(m_PostShaderVars[PV_Bloom2]).setTexture2D(m_Bloom->getBloom(2));
        m_PostMaterial->getParameter(m_PostShaderVars[PV_Bloom3]).setTexture2D(m_Bloom->getBloom(3));
    }
    if (nullptr != m_AutoExposure)
        m_PostMaterial->getParameter(m_PostShaderVars[PV_LumMap]).setTexture2D(m_AutoExposure->getLuminanceMap());

    if (m_AOEnabled || m_FogEnabled)
        m_PostMaterial->getParameter(m_PostShaderVars[PV_NormalDepthMap]).setTexture2D(normalDepthMap); 
    if (m_FogEnabled)
        m_PostMaterial->getParameter(m_PostShaderVars[PV_FogColor]).setFloat3(m_FogColor);

    if (m_AOEnabled)
    {
        m_PostMaterial->getParameter(m_PostShaderVars[PV_SSAORandomNormals]).setTexture2D(m_RandomNormals);
        m_PostMaterial->getParameter(m_PostShaderVars[PV_ViewPortSize]).setFloat2( 
            vec2((float)m_View->getViewport().width, (float)m_View->getViewport().height));
    }

    DrawContext context;
    context.m_VBO = m_Quad->getVBO();
    context.m_IBO = m_Quad->getIBO();
    m_PostMaterial->apply(context);
    m_Quad->draw();
    PROFILER_END();
}

void PostProcesser::draw2D(gui::Canvas2D* canvas)
{
    if (m_ShowDebugTextures)
    {
        float height(128.0f);
        float width(128.0f);
        if (m_Bloom)
        {
            float aspect(m_Bloom->getBloom(0)->getWidth() / (float)m_Bloom->getBloom(0)->getHeight());
            width = aspect * height;
            canvas->blitImage(m_Bloom->getBloom(0), vec2(128 + 12 * 1 + width * 0, 256), false, vec2(width, height));
            canvas->blitImage(m_Bloom->getBloom(1), vec2(128 + 12 * 2 + width * 1, 256), false, vec2(width, height));
            canvas->blitImage(m_Bloom->getBloom(2), vec2(128 + 12 * 3 + width * 2, 256), false, vec2(width, height));
            canvas->blitImage(m_Bloom->getBloom(3), vec2(128 + 12 * 4 + width * 3, 256), false, vec2(width, height));
        }
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
