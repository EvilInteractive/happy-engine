//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
#include "Happy2DRenderer.h"
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
                    m_pIDTexture(NEW Texture2D())
{
}

Picker::~Picker()
{
    delete m_pPickEffect;

    glDeleteRenderbuffers(1, &m_DepthRenderBuffer);
    glDeleteFramebuffers(1, &m_RenderFboID);
}

/* GENERAL */
void Picker::initialize()
{
    m_pPickEffect->load();

    int width = GRAPHICS->getScreenWidth(), 
        height = GRAPHICS->getScreenHeight();

    uint renderTexture;
    glGenTextures(1, &renderTexture);

    GL::heBindTexture2D(0, renderTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    m_pIDTexture->init(renderTexture, width, height, GL_R32I);

    glGenRenderbuffers(1, &m_DepthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);

    glGenFramebuffers(1, &m_RenderFboID);
    GL::heBindFbo(m_RenderFboID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);
    err::checkFboStatus("picker");

    m_bInitialized = true;
}
uint Picker::pick(const vec2& screenPoint)
{
    ASSERT(m_bInitialized, "Initialize picker before using!");

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
        const gfx::IPickable* pPick(dynamic_cast<gfx::IPickable*>(pDrawable));
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
        memcpy(packedID, &i, 4);

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
    memcpy(&id, packedID, 4);

    if (id == 0)
        return UINT_MAX;
    else
        return ID2[(id - 1)];
}

void Picker::drawDebug() const
{
    GUI->drawTexture2D(m_pIDTexture, vec2(12, 12), vec2(640, 320));
}

} } //end namespace