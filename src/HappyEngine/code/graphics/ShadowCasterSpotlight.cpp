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
//Created: 13/10/2012
#include "HappyPCH.h" 
#include "ShadowCasterSpotlight.h"

#include "CameraPerspective.h"
#include "ContentManager.h"
#include "Drawable.h"
#include "DrawContext.h"
#include "GraphicsEngine.h"
#include "InstancingManager.h"
#include "MaterialInstance.h"
#include "RenderTarget.h"
#include "Scene.h"
#include "ShaderUniformBufferManager.h"
#include "SpotLight.h"
#include "Texture2D.h"
#include "View.h"
#include "Window.h"

namespace he {
namespace gfx {

ShadowCasterSpotLight::ShadowCasterSpotLight()
    : m_RenderTarget(nullptr)
{
}


ShadowCasterSpotLight::~ShadowCasterSpotLight()
{
    HEDelete(m_RenderTarget);
}

void ShadowCasterSpotLight::init(ShadowResolution shadowSize)
{
    m_Resolution = shadowSize;

    he::gfx::GraphicsEngine* const gfxEngine(GRAPHICS);

    m_RenderTarget = HENew(RenderTarget)(gfxEngine->getSharedContext());
    const uint16 size(gfxEngine->getShadowMapSize(shadowSize));
    m_RenderTarget->setSize(size, size);
    m_RenderTarget->setDepthTarget();

    ResourceFactory<Texture2D>* textureFactory(ResourceFactory<Texture2D>::getInstance());
    Texture2D* dummyTexture(textureFactory->get(textureFactory->create()));
    dummyTexture->init(TextureWrapType_Clamp, TextureFilterType_Linear, TextureFormat_R16, false);
    dummyTexture->setData(size, size, nullptr, TextureBufferLayout_R, TextureBufferType_Float);

    m_RenderTarget->addTextureTarget(dummyTexture); // dummy target
    dummyTexture->release();

    m_RenderTarget->init();
}

void ShadowCasterSpotLight::render(Scene* scene, SpotLight* light)
{
    HE_ASSERT(light->getShadowResolution() == m_Resolution, "Rendering shadow with different resolution from light");
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    
    // Prepare Camera
    light->prepareShadowCamera();
    const ICamera* cam(&light->getShadowCamera());
    GRAPHICS->getShaderUniformBufferManager()->updateSpotLightBuffer(light, cam);
      
    // Prepare drawing
    m_RenderTarget->switchTextureTarget(0, light->getShadowMap());
    m_RenderTarget->prepareForRendering();

    // Begin drawing
    {
        m_RenderTarget->clear(Color(0.0f, 0.0f, 0.0f, 0.0f));
        GL::heSetViewport(he::RectI(0, 0, m_RenderTarget->getWidth(), m_RenderTarget->getHeight()));

        DrawContext context;
        context.m_Camera = cam;
        scene->getDrawList().draw(DrawListContainer::BlendFilter_Opac, cam, [&context](Drawable* drawable)
        {
            if (drawable->getCastsShadow() == true)
            {
                ModelMesh* const mesh(drawable->getModelMesh());
                context.m_VBO = mesh->getVBO();
                context.m_IBO = mesh->getIBO();
                context.m_WorldMatrix = drawable->getWorldMatrix();
                drawable->getMaterial()->apply(context, eShaderPassType_Shadow);
                mesh->draw();
            }
        });
    }
}



} } //end namespace
