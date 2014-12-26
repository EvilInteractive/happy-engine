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
//Created: 11/08/2011
#include "HappyPCH.h" 

#include "Texture2D.h"
#include "ExternalError.h"
#include "ContentManager.h"

namespace he {
namespace gfx {
    
Texture2D::Texture2D(): 
    m_Id(UINT32_MAX), 
    m_Width(0), 
    m_Height(0), 
    m_TextureFormat(TextureFormat_Compressed_RGBA8_DXT5),
    m_WrapType(TextureWrapType_Repeat), 
    m_FilterType(TextureFilterType_None),
    m_HasMipMaps(false),
    m_IsDefault(true)
{
    const Texture2D* fallback(CONTENT->getFallbackTexture());
    if (fallback)
    {
        m_Id = fallback->m_Id;
        m_Width = fallback->m_Width;
        m_Height = fallback->m_Height;
        m_TextureFormat = fallback->m_TextureFormat;
        m_WrapType = fallback->m_WrapType;
        m_FilterType = fallback->m_FilterType;
        m_HasMipMaps = fallback->m_HasMipMaps;
    }
}

Texture2D::~Texture2D()
{
    if (!m_IsDefault && m_Id != UINT32_MAX)
    {
        glDeleteTextures(1, &m_Id);
    }
}

void Texture2D::init( TextureWrapType wrapType, TextureFilterType filter, TextureFormat textureFormat, bool willHaveMipMaps )
{
    HE_IF_ASSERT(m_IsDefault || m_Id == UINT32_MAX, "Texture2D is being initialized twice: %s", getName().c_str())
    {
        m_IsDefault = false;

        // Create
        glGenTextures(1, &m_Id);
        HE_ASSERT(m_Id != UINT32_MAX, "Texture create failed");

        m_TextureFormat = textureFormat;
        m_HasMipMaps = willHaveMipMaps;

        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Filter
        details::setTextureFilterToBoundTexture(filter, GL_TEXTURE_2D, m_HasMipMaps);

        // Wrap
        m_WrapType = wrapType;
        GLenum wrap(details::getInternalTextureWrapType(m_WrapType));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    }
}

void Texture2D::setData( uint32 width, uint32 height, 
    const void* pData, TextureBufferLayout bufferLayout, TextureBufferType bufferType, uint8 mipLevel)
{
    HE_IF_ASSERT(!m_IsDefault && m_Id != UINT32_MAX, "Texture2D has not been initialized!: %s", getName().c_str())
    {
        if (mipLevel == 0)
        {
            m_Width = width;
            m_Height = height;
        }

        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Data
        glTexImage2D(GL_TEXTURE_2D, mipLevel, details::getInternalTextureFormat(m_TextureFormat), width, height, 
                                    0, details::getInternalTextureBufferLayout(bufferLayout), details::getInternalTextureBufferType(bufferType), 
                                    pData);
    }
}

void Texture2D::setSubData(uint32 xOffset, uint32 yOffset, uint32 width, uint32 height, 
    const void* pData, TextureBufferLayout bufferLayout, TextureBufferType bufferType, uint8 mipLevel)
{
    HE_IF_ASSERT(!m_IsDefault && m_Id != UINT32_MAX, "Texture2D has not been initialized!: %s", getName().c_str())
    {
        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Data
        glTexSubImage2D(GL_TEXTURE_2D, mipLevel, xOffset, yOffset, width, height, 
            details::getInternalTextureBufferLayout(bufferLayout), details::getInternalTextureBufferType(bufferType), 
            pData);
    }
}

void Texture2D::setCompressedData(uint32 width, uint32 height, const void* data, uint32 imageSizeInBytes, uint8 mipLevel)
{
    HE_IF_ASSERT(!m_IsDefault && m_Id != UINT32_MAX, "Texture2D has not been initialized!: %s", getName().c_str())
    {
        if (mipLevel == 0)
        {
            m_Width = width;
            m_Height = height;
        }

        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Data
        glCompressedTexImage2D(GL_TEXTURE_2D, mipLevel, details::getInternalTextureFormat(m_TextureFormat), width, height, 
            0, imageSizeInBytes, data);
    }
}

void Texture2D::generateMipMaps() const
{
    HE_IF_ASSERT(!m_IsDefault && m_Id != UINT32_MAX, "Texture2D has not been initialized!: %s", getName().c_str())
    HE_IF_ASSERT(m_HasMipMaps == true, "Texture2D has not been initialized with the has mipmap flag!: %s", getName().c_str())
    {
        GL::heBindTexture2D(m_Id);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

} } //end namespace
