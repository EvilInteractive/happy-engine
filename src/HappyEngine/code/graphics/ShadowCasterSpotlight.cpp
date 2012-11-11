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
//Created: 13/10/2012
#include "HappyPCH.h" 

#include "ShadowCasterSpotlight.h"

#include "GraphicsEngine.h"
#include "InstancingManager.h"
#include "ContentManager.h"
#include "ShaderVar.h"
#include "View.h"
#include "Scene.h"
#include "Window.h"
#include "RenderTarget.h"
#include "CameraPerspective.h"
#include "SpotLight.h"

namespace he {
namespace gfx {

ShadowCasterSpotLight::ShadowCasterSpotLight()
    : m_MatInstanced(nullptr)
    , m_MatSingle(nullptr)
    , m_MatSkinned(nullptr)
    , m_RenderTarget(nullptr)
{
}


ShadowCasterSpotLight::~ShadowCasterSpotLight()
{
    if (m_MatSingle != nullptr)
        m_MatSingle->release();
    if (m_MatSkinned != nullptr)
        m_MatSkinned->release();
    if (m_MatInstanced != nullptr)
        m_MatInstanced->release();

    delete m_RenderTarget;
}

void ShadowCasterSpotLight::init(ShadowResolution shadowSize)
{
    m_Resolution = shadowSize;

    m_RenderTarget = NEW RenderTarget(GRAPHICS->getDefaultContext());
    uint16 size(GRAPHICS->getShadowMapSize(shadowSize));
    m_RenderTarget->setSize(size, size);
    m_RenderTarget->setDepthTarget();

    ResourceFactory<Texture2D>* textureFactory(ResourceFactory<Texture2D>::getInstance());
    Texture2D* dummyTexture(textureFactory->get(textureFactory->create()));
    dummyTexture->init(TextureWrapType_Clamp, TextureFilterType_Linear, TextureFormat_R16, false);
    dummyTexture->setData(size, size, nullptr, TextureBufferLayout_R, TextureBufferType_Float);

    m_RenderTarget->addTextureTarget(dummyTexture); // dummy target
    dummyTexture->release();

    m_RenderTarget->init();

    ResourceFactory<Material>* matFactory(ResourceFactory<Material>::getInstance());
    m_MatSingle = matFactory->get(CONTENT->loadMaterial("engine/light/linShadowmap.material"));
    m_MatSkinned = matFactory->get(CONTENT->loadMaterial("engine/light/linShadowmapSkinned.material"));
    m_MatInstanced = matFactory->get(CONTENT->loadMaterial("engine/light/linShadowmapInstanced.material"));
}

void ShadowCasterSpotLight::render(Scene* scene, SpotLight* light)
{
    HE_ASSERT(light->getShadowResolution() == m_Resolution, "Rendering shadow with different resolution from light");
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    
    // Prepare Camera
    light->prepareShadowCamera();
    const ICamera* cam(&light->getShadowCamera());
      
    // Cull
    {
        m_SingleDrawables.clear();
        m_InstancedDrawables.clear();
        m_SkinnedDrawables.clear();
        scene->getDrawList().draw(DrawListContainer::BlendFilter_Opac, cam, [&](IDrawable* drawable)
        {
            if (drawable->getCastsShadow() == true)
            {
                if (drawable->isSkinned())
                    m_SkinnedDrawables.add(drawable);
                else if (drawable->isSingle())
                    m_SingleDrawables.add(drawable);
                else
                    m_InstancedDrawables.add(drawable);
            }
        });
    }

    // Prepare drawing
    m_RenderTarget->switchTextureTarget(0, light->getShadowMap());
    m_RenderTarget->prepareForRendering();

    // Begin drawing
    {
        m_RenderTarget->clear(Color(0.0f, 0.0f, 0.0f, 0.0f));
        GL::heSetViewport(he::RectI(0, 0, m_RenderTarget->getWidth(), m_RenderTarget->getHeight()));
        GL::heBlendEnabled(false);
        GL::heSetCullFace(false);
        GL::heSetDepthFunc(DepthFunc_LessOrEqual);
        GL::heSetDepthWrite(true);
        GL::heSetDepthRead(true);

        if (m_InstancedDrawables.empty() == false)
        {
            m_InstancedDrawables.forEach([&](IDrawable* drawable)
            {
                m_MatInstanced->apply(static_cast<InstancedDrawable*>(drawable), cam);
                drawable->drawShadow();
            });
        }
        if (m_SingleDrawables.empty() == false)
        {
            m_SingleDrawables.forEach([&](IDrawable* drawable)
            {
                m_MatSingle->apply(static_cast<SingleDrawable*>(drawable), cam);
                drawable->drawShadow();
            });
        }
        if (m_SkinnedDrawables.empty() == false)
        {
            m_SkinnedDrawables.forEach([&](IDrawable* drawable)
            {
                m_MatSkinned->apply(static_cast<SkinnedDrawable*>(drawable), cam);
                drawable->drawShadow();
            });
        }
    }
}



} } //end namespace
