//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 25/11/2012

#include "HappyPCH.h" 

#include "Sprite.h"
#include "Texture2D.h"

namespace he {
namespace gui {

/* STATIC */
uint16 Sprite::s_SpriteCount = 0;

/* CONSTRUCTOR - DESTRUCTOR */
Sprite::Sprite() : 
    m_Render(nullptr),
    m_Rendered(false),
    m_Size(vec2(0,0)),
    m_Center(RectF(0,0,0,0))
{
    m_ID = s_SpriteCount;
    ++s_SpriteCount;
}

Sprite::~Sprite()
{
    m_Render->release();
}

/* GENERAL */
void Sprite::init(const vec2& size)
{
    m_Size = size;

    // create renderbuffer
    ResourceFactory<gfx::Texture2D>* factory(ResourceFactory<gfx::Texture2D>::getInstance());
    ObjectHandle handle(factory->create());
    m_Render = factory->get(handle);

    m_Render->init(
        gfx::TextureWrapType_Clamp,
        gfx::TextureFilterType_Linear,
        gfx::TextureFormat_RGBA8,
        false);
    m_Render->setData(
        static_cast<uint32>(size.x),
        static_cast<uint32>(size.y),
        nullptr, // reserve data
        gfx::TextureBufferLayout_RGBA,
        gfx::TextureBufferType_Byte,
        0);
    m_Render->setLoadFinished();

    float thirdX(size.x / 3);
    float thirdY(size.y / 3);
    m_Center.x = thirdX;
    m_Center.y = thirdY;
    m_Center.width = thirdX;
    m_Center.height = thirdY;
}
void Sprite::invalidate(const vec2& newSize)
{
    vec2 size(m_Size);

    if (newSize.x > 0 && newSize.y > 0)
    {
        m_Size = newSize;
        size = newSize;
    }

    m_Render->setData(
        static_cast<uint32>(size.x),
        static_cast<uint32>(size.y),
        nullptr,
        gfx::TextureBufferLayout_RGBA,
        gfx::TextureBufferType_Byte,
        0);

    /*
    float thirdX(size.x / 3);
    float thirdY(size.y / 3);
    m_Center.x = thirdX;
    m_Center.y = thirdY;
    m_Center.width = thirdX;
    m_Center.height = thirdY;*/
}

/* GETTERS */
gfx::Texture2D* Sprite::getRenderTexture() const
{
    return m_Render;
}
uint16 Sprite::getID() const
{
    return m_ID;
}
vec2 Sprite::getSize() const
{
    return m_Size;
}
RectF Sprite::getCenter() const
{
    return m_Center;
}

/* SETTERS */
void Sprite::setCenter(const RectF& center)
{
    m_Center = center;
}

}} //end namespace
