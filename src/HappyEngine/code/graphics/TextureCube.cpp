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

#include "TextureCube.h"

namespace he {
namespace gfx {

TextureCube::TextureCube()
: m_HasMipMaps(false)
, m_Width(0)
, m_Height(0)
, m_Id(UINT_MAX)
, m_FilterType(TextureFilterType_None)
, m_WrapType(TextureWrapType_Repeat)
, m_TextureFormat(TextureFormat_Compressed_RGBA8_DXT5)
, m_IsLoadDone(false)
{

}

TextureCube::~TextureCube()
{
    if (m_Id != UINT_MAX)
    {
        glDeleteTextures(1, &m_Id);
    }
}

void TextureCube::init( TextureWrapType wrapType, TextureFilterType filter, TextureFormat textureFormat, bool willHaveMipMaps )
{
    HE_IF_ASSERT(m_Id == UINT_MAX, "Texture2D is being initialized twice: %s", getName().c_str())
    {
        // Create
        glGenTextures(1, &m_Id);
        HE_ASSERT(m_Id != UINT_MAX, "Texture create failed");

        m_TextureFormat = textureFormat;
        m_HasMipMaps = willHaveMipMaps;

        // Bind
        GL::heBindTextureCube(0, m_Id);

        //Filter
        m_FilterType = filter;
        details::setTextureFilterToBoundTexture(filter, GL_TEXTURE_CUBE_MAP, m_HasMipMaps);

        // Wrap
        m_WrapType = wrapType;
        GLenum wrap(details::getInternalTextureWrapType(m_WrapType));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);
    }
}

void TextureCube::setData( uint width, uint height, const Face& face, const void* pData, TextureBufferLayout bufferLayout, TextureBufferType bufferType, byte mipLevel /*= 0*/ )
{
    HE_IF_ASSERT(m_Id != UINT_MAX, "TextureCube has not been initialized!: %s", getName().c_str())
    {
        if (mipLevel == 0)
        {
            m_Width = width;
            m_Height = height;
        }

        // Bind
        GL::heBindTextureCube(0, m_Id);

        // Data
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mipLevel, details::getInternalTextureFormat(m_TextureFormat), 
            width, height, 0, details::getInternalTextureBufferLayout(bufferLayout), 
            details::getInternalTextureBufferType(bufferType), pData);
    }
}

void TextureCube::setCompressedData( uint width, uint height, const Face& face, const void* data, uint imageSizeInBytes, byte mipLevel /*= 0*/ )
{
    HE_IF_ASSERT(m_Id != UINT_MAX, "TextureCube has not been initialized!: %s", getName().c_str())
    {
        if (mipLevel == 0)
        {
            m_Width = width;
            m_Height = height;
        }

        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Data
        glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mipLevel, details::getInternalTextureFormat(m_TextureFormat), 
            width, height, 0, imageSizeInBytes, data);
#if _DEBUG
        err::glCheckForErrors(true);
#endif
    }
}

void TextureCube::setLoadFinished()
{
    m_CallbackMutex.lock();
    m_IsLoadDone = true;
    Loaded();
    Loaded.clear();
    m_CallbackMutex.unlock();
}

void TextureCube::callbackOnceIfLoaded( const boost::function<void()>& callback ) const
{
    TextureCube* _this(const_cast<TextureCube*>(this));
    _this->m_CallbackMutex.lock();
    if (m_IsLoadDone)
    {
        _this->m_CallbackMutex.unlock();
        callback();
    }
    else
    {
        eventCallback0<void> handler(callback);
        _this->Loaded += handler;
        _this->m_CallbackMutex.unlock();
    }
}

} } //end namespace