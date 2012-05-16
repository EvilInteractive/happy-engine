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
#include "HappyNew.h"

#include "GraphicsEngine.h"
#include "Renderer2D.h"
#include "OpenGL.h"
#include "HappyEngine.h"
#include "DrawManager.h"
#include "CameraManager.h"
#include "ICamera.h"
#include "Camera.h"

#include "ExternalError.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
Picker::Picker() :	m_pPickEffect(NEW PickEffect()),
                    m_RenderFboID(0),
                    m_bInitialized(false),
                    m_pIDTexture(ResourceFactory<Texture2D>::getInstance()->get(ResourceFactory<Texture2D>::getInstance()->create()))
{
    m_pIDTexture->setName("Picker::m_pIDTexture");
}

Picker::~Picker()
{
    delete m_pPickEffect;
    m_pIDTexture->release();

    glDeleteRenderbuffers(1, &m_DepthRenderBuffer);
    glDeleteFramebuffers(1, &m_RenderFboID);
}

/* GENERAL */
void Picker::initialize()
{
    m_pPickEffect->load();

    int width = GRAPHICS->getScreenWidth(), 
        height = GRAPHICS->getScreenHeight();

    m_pIDTexture->setData(width, height, 
        gfx::Texture2D::TextureFormat_RGBA8, 0, 
        gfx::Texture2D::BufferLayout_BGRA, gfx::Texture2D::BufferType_Byte,
        gfx::Texture2D::WrapType_Clamp,  gfx::Texture2D::FilterType_Nearest, false, false );

    glGenRenderbuffers(1, &m_DepthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);

    glGenFramebuffers(1, &m_RenderFboID);
    GL::heBindFbo(m_RenderFboID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pIDTexture->getID(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);
    err::checkFboStatus("picker");

    m_bInitialized = true;
}
uint Picker::pick(const vec2& screenPoint)
{
    HE_ASSERT(m_bInitialized, "Initialize picker before using!");

    std::vector<uint> ID1;

    // cull drawlist
    std::vector<IDrawable*> culledDrawList;

    uint i(0);
    GRAPHICS->getDrawManager()->getDrawList().for_each(DrawListContainer::F_Loc_BeforePost   | 
                                                      DrawListContainer::F_Loc_AfterPost    |
                                                      DrawListContainer::F_Main_Opac        |
                                                      DrawListContainer::F_Sub_Single       ,//|
                                                      //DrawListContainer::F_Sub_Skinned,                                                     
                                                      [&](IDrawable* pDrawable)
    {
        if (pDrawable->isInCamera(CAMERAMANAGER->getActiveCamera()))
        {
            culledDrawList.push_back(pDrawable);
            ID1.push_back(i);
        }

        ++i;
    });

    // create list with items to be picked
    std::vector<IDrawable*> pickList;
    std::vector<uint> ID2;

    i = 0;
    std::sort(culledDrawList.begin(), culledDrawList.end());
    std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](IDrawable* pDrawable)
    {
        const gfx::IPickable* pPick(static_cast<gfx::IPickable*>(pDrawable));
        if (pPick != nullptr && pPick->isPickable())
        {
            pickList.push_back(pDrawable);
            ID2.push_back(ID1[i]);
        }

        ++i;
    });

    GL::heBlendEnabled(false);
    GL::heSetDepthWrite(true);
    GL::heSetDepthRead(true);

    GL::heBindFbo(m_RenderFboID);
    const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    GRAPHICS->setViewport(he::RectI(0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    i = 1;
    std::for_each(pickList.cbegin(), pickList.cend(), [&](IDrawable* pDrawable)
    {
        byte packedID[4];
        he_memcpy(packedID, &i, 4);

        m_pPickEffect->setID(vec4(packedID[2] / 255.0f, packedID[1] / 255.0f, packedID[0] / 255.0f, packedID[3] / 255.0f)); //BGRA
        pDrawable->applyMaterial(m_pPickEffect->getMaterial(), CAMERAMANAGER->getActiveCamera());
        pDrawable->drawShadow();

        ++i;
    });

    //glGetError();

    byte packedID[4];
    glReadPixels(	GRAPHICS->getScreenWidth() - static_cast<int>(screenPoint.x),
                    GRAPHICS->getScreenHeight() - static_cast<int>(screenPoint.y),
                    1, 1, GL_BGRA, GL_UNSIGNED_BYTE, &packedID);

    uint id(0);
    he_memcpy(&id, packedID, 4);

    if (id == 0)
        return UINT_MAX;
    else
        return ID2[(id - 1)];
}

uint Picker::pick(const vec2& screenPoint, const std::vector<IDrawable*>& drawList)
{
    HE_ASSERT(m_bInitialized, "Initialize picker before using!");

    std::vector<uint> ID1;

    // cull drawlist
    std::vector<IDrawable*> culledDrawList;

    uint i(0);                                                    
    std::for_each(drawList.cbegin(),drawList.cend(), [&](IDrawable* pDrawable)
    {
        /*if (pDrawable->isInCamera(CAMERAMANAGER->getActiveCamera()))
        {*/
            culledDrawList.push_back(pDrawable);
            ID1.push_back(i);
        //}

        ++i;
    });

    // create list with items to be picked
    std::vector<IDrawable*> pickList;
    std::vector<uint> ID2;

    i = 0;
    std::sort(culledDrawList.begin(), culledDrawList.end());
    std::for_each(culledDrawList.cbegin(), culledDrawList.cend(), [&](IDrawable* pDrawable)
    {
        const gfx::IPickable* pPick(static_cast<gfx::IPickable*>(pDrawable));
        if (pPick != nullptr && pPick->isPickable())
        {
            pickList.push_back(pDrawable);
            ID2.push_back(ID1[i]);
        }

        ++i;
    });

    GL::heBlendEnabled(false);
    GL::heSetDepthWrite(true);
    GL::heSetDepthRead(true);

    GL::heBindFbo(m_RenderFboID);
    const static GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);

    GRAPHICS->setViewport(he::RectI(0, 0, GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight()));
    GL::heClearColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    i = 1;
    std::for_each(pickList.cbegin(), pickList.cend(), [&](IDrawable* pDrawable)
    {
        byte packedID[4];
        he_memcpy(packedID, &i, 4);

        m_pPickEffect->setID(vec4(packedID[2] / 255.0f, packedID[1] / 255.0f, packedID[0] / 255.0f, packedID[3] / 255.0f)); //BGRA
        pDrawable->applyMaterial(m_pPickEffect->getMaterial(), CAMERAMANAGER->getActiveCamera());
        pDrawable->draw();

        ++i;
    });

    //glGetError();

    byte packedID[4];
    glReadPixels(	GRAPHICS->getScreenWidth() - static_cast<int>(screenPoint.x),
        GRAPHICS->getScreenHeight() - static_cast<int>(screenPoint.y),
        1, 1, GL_BGRA, GL_UNSIGNED_BYTE, &packedID);

    uint id(0);
    he_memcpy(&id, packedID, 4);

    if (id == 0)
        return UINT_MAX;
    else
        return ID2[(id - 1)];
}

void Picker::drawDebug() const
{
    //GUI->drawTexture2D(m_pIDTexture, vec2(12, 12), vec2(640, 320));
}

} } //end namespace