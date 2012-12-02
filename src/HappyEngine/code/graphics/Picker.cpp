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
//Author:  Sebastiaan Sprengers
//Created: 11/11/2011

#include "HappyPCH.h" 

#include "Picker.h"
#include "IPickable.h"

#include "GraphicsEngine.h"
#include "Renderer2D.h"
#include "CameraManager.h"
#include "ICamera.h"
#include "CameraPerspective.h"

#include "ExternalError.h"
#include "Texture2D.h"
#include "PickEffect.h"
#include "RenderTarget.h"
#include "View.h"
#include "Scene.h"
#include "Window.h"
#include "IDrawable.h"
#include "Canvas2D.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Picker::Picker() :	m_PickEffect(NEW PickEffect()),
                    m_Initialized(false),
                    m_RenderTexture(nullptr),
                    m_Scene(nullptr),
                    m_RenderTarget(nullptr)
{
    m_RenderTexture = ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create());
    m_RenderTexture->setName("Picker::m_pIDTexture");
}

Picker::~Picker()
{
    delete m_PickEffect;
    m_RenderTexture->release();
    delete m_RenderTarget;
}

/* GENERAL */
void Picker::init(View* view, Scene* scene)
{
    m_PickEffect->load();

    m_View = view;
    m_Scene = scene;

    int width = view->getViewport().width,
         height = view->getViewport().height;

    m_RenderTexture->init(
        TextureWrapType_Clamp,
        TextureFilterType_Nearest,
        TextureFormat_RGBA8,
        false);
    m_RenderTexture->setData(
        width, height, 0,
        TextureBufferLayout_BGRA,
        TextureBufferType_Byte, 0);

    m_RenderTarget = NEW RenderTarget(m_View->getWindow()->getContext());
    m_RenderTarget->addTextureTarget(m_RenderTexture);
    m_RenderTarget->setDepthTarget();
    m_RenderTarget->init();

    m_Initialized = true;
}
uint32 Picker::pick(const vec2& screenPoint)
{
    HE_ASSERT(m_Initialized, "Initialize picker before using!");

    PrimitiveList<uint32> ID1;
 
    // cull drawlist
    PrimitiveList<IDrawable*> culledDrawList;
 
    uint32 i(0);

    const PrimitiveList<IDrawable*>* drawList(m_Scene->getDrawList().getList());
    for (uint8 ii(0); ii < DrawListContainer::BlendFilter_MAX; ++ii)
    {
        drawList[ii].forEach([&](IDrawable* drawable)
        {
            if (m_View->getCamera()->intersect(drawable->getBound()) != IntersectResult_Outside)
            {
                culledDrawList.add(drawable);
                ID1.add(i);
            }

            ++i;
        });
    }
 
    // create list with items to be picked
    he::PrimitiveList<IDrawable*> pickList;
    he::PrimitiveList<uint32> ID2;
 
    i = 0;
    std::sort(culledDrawList.begin(), culledDrawList.end());
    std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](IDrawable* drawable)
    {
        const gfx::IPickable* pick(static_cast<gfx::IPickable*>(drawable));
        if (pick != nullptr && pick->isPickable())
        {
            pickList.add(drawable);
            ID2.add(ID1[i]);
        }
 
        ++i;
    });

    int height(m_View->getViewport().height);
    int width(m_View->getViewport().width);

    m_RenderTarget->prepareForRendering();
 
    GL::heBlendEnabled(false);
    GL::heSetDepthWrite(true);
    GL::heSetDepthRead(true);
    GL::heSetViewport(RectI(0, 0, width, height));

    m_RenderTarget->clear(Color(0.0f,0.0f,0.0f,0.0f));

    GL::heScissorEnabled(true);
    GL::heScissorRect(RectI(
        static_cast<int>(screenPoint.x - 5),
        static_cast<int>(height - screenPoint.y - 5),
        10,10));
     
    i = 1;
    std::for_each(pickList.cbegin(), pickList.cend(), [&](IDrawable* pDrawable)
    {
        byte packedID[4];
        he_memcpy(packedID, &i, 4);
 
        m_PickEffect->setID(vec4(packedID[2] / 255.0f, packedID[1] / 255.0f, packedID[0] / 255.0f, packedID[3] / 255.0f)); //BGRA
        pDrawable->applyMaterial(m_PickEffect->getMaterial(), m_View->getCamera());
        pDrawable->drawShadow();
 
        ++i;
    });

    GL::heScissorEnabled(false);
 
    byte packedID[4];
    glReadPixels(	static_cast<int>(screenPoint.x),
                    height - static_cast<int>(screenPoint.y),
                    1, 1, GL_BGRA, GL_UNSIGNED_BYTE, &packedID);
 
    uint32 id(0);
    he_memcpy(&id, packedID, 4);
 
    if (id == 0)
        return UINT32_MAX;
    else
        return ID2[(id - 1)];
}

uint32 Picker::pick(const vec2& screenPoint, const PrimitiveList<IDrawable*>& drawList)
{
    PrimitiveList<uint32> ID1;
 
    // cull drawlist
    PrimitiveList<IDrawable*> culledDrawList;
 
    uint32 i(0);

    drawList.forEach([&](IDrawable* drawable)
    {
        if (m_View->getCamera()->intersect(drawable->getBound()) != IntersectResult_Outside)
        {
            culledDrawList.add(drawable);
            ID1.add(i);
        }

        ++i;
    });
 
    // create list with items to be picked
    he::PrimitiveList<IDrawable*> pickList;
    he::PrimitiveList<uint32> ID2;
 
    i = 0;
    std::sort(culledDrawList.begin(), culledDrawList.end());
    std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](IDrawable* drawable)
    {
        const gfx::IPickable* pick(static_cast<gfx::IPickable*>(drawable));
        if (pick != nullptr && pick->isPickable())
        {
            pickList.add(drawable);
            ID2.add(ID1[i]);
        }
 
        ++i;
    });

    int height(m_View->getViewport().height);
    int width(m_View->getViewport().width);

    m_RenderTarget->prepareForRendering();
 
    GL::heBlendEnabled(false);
    GL::heSetDepthWrite(true);
    GL::heSetDepthRead(true);
    GL::heSetViewport(RectI(0, 0, width, height));

    m_RenderTarget->clear(Color(0.0f,0.0f,0.0f,0.0f));

    GL::heScissorEnabled(true);
    GL::heScissorRect(RectI(
        static_cast<int>(screenPoint.x - 5),
        static_cast<int>(height - screenPoint.y - 5),
        10,10));
     
    i = 1;
    std::for_each(pickList.cbegin(), pickList.cend(), [&](IDrawable* pDrawable)
    {
        byte packedID[4];
        he_memcpy(packedID, &i, 4);
 
        m_PickEffect->setID(vec4(packedID[2] / 255.0f, packedID[1] / 255.0f, packedID[0] / 255.0f, packedID[3] / 255.0f)); //BGRA
        pDrawable->applyMaterial(m_PickEffect->getMaterial(), m_View->getCamera());
        pDrawable->drawShadow();
 
        ++i;
    });

    GL::heScissorEnabled(false);
 
    byte packedID[4];
    glReadPixels(	static_cast<int>(screenPoint.x),
                    height - static_cast<int>(screenPoint.y),
                    1, 1, GL_BGRA, GL_UNSIGNED_BYTE, &packedID);
 
    uint32 id(0);
    he_memcpy(&id, packedID, 4);
 
    if (id == 0)
        return UINT32_MAX;
    else
        return ID2[(id - 1)];
}

void Picker::drawDebug(Canvas2D* canvas) const
{
    canvas->getRenderer2D()->drawTexture2DToScreen(m_RenderTexture, vec2(0,0), false, vec2(m_View->getViewport().width * 1.0f, m_View->getViewport().height * 1.0f));
}

/* SETTERS */

} } //end namespace