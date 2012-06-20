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

namespace he {
namespace gfx {
    
Texture2D::Texture2D(): 
    m_Id(UINT_MAX), 
    m_Width(0), 
    m_Height(0), 
    m_IsLoadDone(false),
    m_WrapType(WrapType_Repeat), 
    m_FilterType(FilterType_None),
    m_HasMipMaps(false)
{
}


void Texture2D::setFilter(FilterType filterType)
{
    HE_IF_ASSERT(filterType != FilterType_None, "filter type cannot be FilterType_None!")
    if (m_FilterType != filterType)
    {
        m_FilterType = filterType;
        if (m_FilterType >= FilterType_Anisotropic_2x && GL::getMaxAnisotropicFilteringSupport() > FLT_EPSILON)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            if (m_FilterType > GL::getMaxAnisotropicFilteringSupport())
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL::getMaxAnisotropicFilteringSupport());
            else
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(m_FilterType));
        }
        else
        {
            if (m_FilterType >= FilterType_Anisotropic_2x)
                m_FilterType = FilterType_Linear;
            if (m_FilterType == FilterType_Nearest)
            {     
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                if (m_HasMipMaps)
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                else
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if (m_HasMipMaps)
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                else
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
        }
    }
}

void Texture2D::init( WrapType wrapType, FilterType filter, TextureFormat textureFormat, bool willHaveMipMaps )
{
    HE_IF_ASSERT(m_Id == UINT_MAX, "Texture2D is being initialized twice: %s", getName().c_str())
    {
        // Create
        glGenTextures(1, &m_Id);
        HE_ASSERT(m_Id != UINT_MAX, "Texture create failed");

        m_TextureFormat = textureFormat;
        m_HasMipMaps = willHaveMipMaps;

        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Filter
        setFilter(filter);

        // Wrap
        if (m_WrapType != wrapType)
        {
            m_WrapType = wrapType;
            GLenum wrap(getInternalWrapType(m_WrapType));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        }
    }
}

void Texture2D::setData( uint width, uint height, 
    const void* pData, BufferLayout bufferLayout, BufferType bufferType, byte mipLevel)
{
    HE_IF_ASSERT(m_Id != UINT_MAX, "Texture2D has not been initialized!: %s", getName().c_str())
    {
        if (mipLevel == 0)
        {
            m_Width = width;
            m_Height = height;
        }

        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Data
        glTexImage2D(GL_TEXTURE_2D, mipLevel, getInternalFormat(m_TextureFormat), width, height, 
                                    0, getInternalBufferLayout(bufferLayout), getInternalBufferType(bufferType), 
                                    pData);
    }
}

void Texture2D::setCompressedData(uint width, uint height, const void* data, uint imageSizeInBytes, byte mipLevel)
{
    HE_IF_ASSERT(m_Id != UINT_MAX, "Texture2D has not been initialized!: %s", getName().c_str())
    {
        if (mipLevel == 0)
        {
            m_Width = width;
            m_Height = height;
        }

        // Bind
        GL::heBindTexture2D(0, m_Id);

        //Data
        glCompressedTexImage2D(GL_TEXTURE_2D, mipLevel, getInternalFormat(m_TextureFormat), width, height, 
            0, imageSizeInBytes, data);
#if _DEBUG
        err::glCheckForErrors(true);
#endif
    }
}

void Texture2D::setLoadFinished()
{
    m_CallbackMutex.lock();
    m_IsLoadDone = true;
    Loaded();
    Loaded.clear();
    m_CallbackMutex.unlock();
}

void Texture2D::generateMipMaps() const
{
    HE_IF_ASSERT(m_Id != UINT_MAX, "Texture2D has not been initialized!: %s", getName().c_str())
    HE_IF_ASSERT(m_HasMipMaps == true, "Texture2D has not been initialized with the has mipmap flag!: %s", getName().c_str())
    {
        GL::heBindTexture2D(m_Id);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}


bool Texture2D::isInitialized() const
{
    return m_IsLoadDone;
}

Texture2D::~Texture2D()
{
    if (m_Id != UINT_MAX)
    {
        glDeleteTextures(1, &m_Id);
    }
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

void Texture2D::callbackOnceIfLoaded( const boost::function<void()>& callback ) const
{
    Texture2D* _this(const_cast<Texture2D*>(this));
    _this->m_CallbackMutex.lock();
    if (m_IsLoadDone)
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

GLenum Texture2D::getInternalFormat( TextureFormat format )
{
    switch (format)
    {
    case TextureFormat_R8: return GL_RED;
    case TextureFormat_RG8: return GL_RG;
    case TextureFormat_RGB8: return GL_RGB;
    case TextureFormat_RGBA8: return GL_RGBA;
    case TextureFormat_Compressed_RGB8_DXT1:  return GL::getSupportTextureCompression()? GL_COMPRESSED_RGB_S3TC_DXT1_EXT  : GL_RGB;
    case TextureFormat_Compressed_RGBA8_DXT1: return GL::getSupportTextureCompression()? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT : GL_RGBA;
    case TextureFormat_Compressed_RGBA8_DXT3: return GL::getSupportTextureCompression()? GL_COMPRESSED_RGBA_S3TC_DXT3_EXT : GL_RGBA;
    case TextureFormat_Compressed_RGBA8_DXT5: return GL::getSupportTextureCompression()? GL_COMPRESSED_RGBA_S3TC_DXT5_EXT : GL_RGBA;
    case TextureFormat_RGBA16: return GL_RGBA16;
    case TextureFormat_RGB16: return GL_RGB16;
    case TextureFormat_RG16: return GL_RG16;
    case TextureFormat_R16: return GL_R16;
    case TextureFormat_RGBA16F: return GL_RGBA16F;
    case TextureFormat_RGB16F: return GL_RGB16F;
    case TextureFormat_RG16F: return GL_RG16F;
    case TextureFormat_R16F: return GL_R16F;
    case TextureFormat_Depth32: return GL_DEPTH_COMPONENT32;
    case TextureFormat_Depth24_Stencil8: return GL_DEPTH24_STENCIL8;
    default: HE_ERROR("Unknown Texture2D::TextureFormat"); return GL_RGBA;
    }
}
GLenum Texture2D::getInternalBufferLayout( BufferLayout layout )
{
    switch (layout)
    {
    case BufferLayout_RGBA: return GL_RGBA;
    case BufferLayout_BGRA: return GL_BGRA;
    case BufferLayout_RGB:  return GL_RGB;
    case BufferLayout_BGR:  return GL_BGR;
    case BufferLayout_RG:   return GL_RG;
    case BufferLayout_R:    return GL_RED;
    case BufferLayout_Depth:return GL_DEPTH_COMPONENT;
    case BufferLayout_LA:   return GL_LUMINANCE_ALPHA;
    default:
        HE_ERROR("Unkown Texture2D::BufferLayout");
        return GL_BGRA;
    }
}
GLenum Texture2D::getInternalBufferType( BufferType bufferType )
{
    switch (bufferType)
    {
    case BufferType_Byte: return GL_UNSIGNED_BYTE;
    case BufferType_Float: return GL_FLOAT;
    default: HE_ERROR("Unknown Texture2D::Buffertype"); return GL_UNSIGNED_BYTE;
    }
}


he::uint Texture2D::calculatePixelSize( BufferLayout bufferLayout, BufferType bufferType )
{
    uint size(0);
    switch (bufferType)
    {
    case BufferType_Byte:
        size = sizeof(byte); break;
    case BufferType_Float: 
        size = sizeof(float); break;
    default:
        HE_ERROR("Unknown Texture2D::Buffertype"); break;
    }
    switch (bufferLayout)
    {
    case BufferLayout_RGBA:
    case BufferLayout_BGRA:
        size *= 4; break;
    case BufferLayout_RGB:
    case BufferLayout_BGR:
        size *= 3; break;
    case BufferLayout_RG:
        size *= 2; break;
    case BufferLayout_Depth:
    case BufferLayout_R:
        size *= 1; break;
    default:
        HE_ERROR("Unknown Texture2D::BufferLayout");
    }

    return size;
}

GLenum Texture2D::getInternalWrapType( WrapType type )
{
    switch (type)
    {
    case WrapType_Clamp: return GL_CLAMP_TO_EDGE;
    case WrapType_Mirror: return GL_MIRRORED_REPEAT;
    case WrapType_Repeat: return GL_REPEAT;
    default: HE_ERROR("Unknown Texture2D::WrapType"); return GL_REPEAT;
    }
}

Texture2D::TextureFormat Texture2D::getTextureFormat() const
{
    return m_TextureFormat;
}

Texture2D::WrapType Texture2D::getWrapType() const
{
    return m_WrapType;
}

Texture2D::FilterType Texture2D::getFilterType() const
{
    return m_FilterType;
}



} } //end namespace