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
//Created: 11/08/2011
#include "HappyPCH.h" 

#include "Texture2D.h"
#include "GL/glew.h"

namespace he {
namespace gfx {

uint Texture2D::s_Count = 0;

Texture2D::Texture2D(): m_Id(UINT_MAX), m_Width(0), m_Height(0), m_Format(0), m_isInitialized(false), m_pPixels(nullptr)
{
    ++s_Count;
}

void Texture2D::init(uint tex, uint width, uint height, uint format, void* pixels, uint bufferSize)
{
    HE_ASSERT(width != 0 && height != 0, "Loaded texture with width = 0 or height = 0 !");

    if (m_Id != UINT_MAX)
    {
        glDeleteTextures(1, &m_Id);
    }
    he_free(m_pPixels);
    m_pPixels = nullptr;
    if (bufferSize > 0)
    {
        m_pPixels = he_malloc(bufferSize);
        he_memcpy(m_pPixels, pixels, bufferSize);
    }
    m_Id = tex;
    m_Width = width;
    m_Height = height;
    m_Format = format;
    m_CallbackMutex.lock();
    m_isInitialized = true;
    Loaded();
    m_CallbackMutex.unlock();
}

bool Texture2D::isInitialized() const
{
    return m_isInitialized;
}

Texture2D::~Texture2D()
{
    he_free(m_pPixels);
    if (m_Id != UINT_MAX)
    {
        glDeleteTextures(1, &m_Id);
    }
    --s_Count;
}

uint Texture2D::getTextureCount()
{
    return s_Count;
}


uint Texture2D::getID() const
{
    return m_Id;
}

uint Texture2D::getWidth() const
{
    return m_Width;
}
uint Texture2D::getHeight() const
{
    return m_Height;
}

void Texture2D::callbackIfLoaded( const boost::function<void()>& callback ) const
{
    Texture2D* _this(const_cast<Texture2D*>(this));
    _this->m_CallbackMutex.lock();
    if (m_isInitialized)
    {
        _this->m_CallbackMutex.unlock();
        callback();
    }
    else
    {
        _this->Loaded += callback;
        _this->m_CallbackMutex.unlock();
    }
}

uint Texture2D::getFormat() const
{
    return m_Format;
}

void* Texture2D::getPixelsIfAvailable() const
{
    return m_pPixels;
}

} } //end namespace