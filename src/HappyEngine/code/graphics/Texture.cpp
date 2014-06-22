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
//Created: 01/11/2012
#include "HappyPCH.h" 

#include "Texture.h"

namespace he {
namespace gfx {
namespace details {  

void setTextureFilterToBoundTexture(TextureFilterType filterType, GLenum target, bool hasMipmaps)
{
    HE_IF_ASSERT(filterType != TextureFilterType_None, "filter type cannot be FilterType_None!")
    {
        if (filterType >= TextureFilterType_Anisotropic_2x && GL::getMaxAnisotropicFilteringSupport() > FLT_EPSILON)
        {
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            if (filterType > GL::getMaxAnisotropicFilteringSupport())
                glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL::getMaxAnisotropicFilteringSupport());
            else
                glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(filterType));
        }
        else
        {
            if (filterType >= TextureFilterType_Anisotropic_2x)
                filterType = TextureFilterType_Linear;
            if (filterType == TextureFilterType_Nearest)
            {     
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                if (hasMipmaps)
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                else
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
            else
            {
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if (hasMipmaps)
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                else
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
        }
    }
}

GLenum getInternalTextureFormat( TextureFormat format )
{
    GLenum result(GL_RED);
    switch (format)
    {
    case TextureFormat_R8: result = GL_RED; break;
    case TextureFormat_RG8: result = GL_RG; break;
    case TextureFormat_RGB8: result = GL_RGB; break;
    case TextureFormat_RGBA8: result = GL_RGBA; break;
    case TextureFormat_Compressed_RGB8_DXT1:  result = GL::getSupportTextureCompression()? GL_COMPRESSED_RGB_S3TC_DXT1_EXT  : GL_RGB; break;
    case TextureFormat_Compressed_RGBA8_DXT1: result = GL::getSupportTextureCompression()? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT : GL_RGBA; break;
    case TextureFormat_Compressed_RGBA8_DXT3: result = GL::getSupportTextureCompression()? GL_COMPRESSED_RGBA_S3TC_DXT3_EXT : GL_RGBA; break;
    case TextureFormat_Compressed_RGBA8_DXT5: result = GL::getSupportTextureCompression()? GL_COMPRESSED_RGBA_S3TC_DXT5_EXT : GL_RGBA; break;
    case TextureFormat_RGBA16: result = GL_RGBA16; break;
    case TextureFormat_RGB16: result = GL_RGB16; break;
    case TextureFormat_RG16: result = GL_RG16; break;
    case TextureFormat_R16: result = GL_R16; break;
    case TextureFormat_RGBA16F: result = GL_RGBA16F; break;
    case TextureFormat_RGB16F: result = GL_RGB16F; break;
    case TextureFormat_RG16F: result = GL_RG16F; break;
    case TextureFormat_R16F: result = GL_R16F; break;
    case TextureFormat_Depth32: result = GL_DEPTH_COMPONENT32; break;
    case TextureFormat_Depth24_Stencil8: result = GL_DEPTH24_STENCIL8; break;
    default: HE_ERROR("Unknown Texture2D::TextureFormat"); break;
    }
    return result;
}
GLenum getInternalTextureBufferLayout( TextureBufferLayout layout )
{
    GLenum result(GL_RGBA);
    switch (layout)
    {
    case TextureBufferLayout_RGBA: result = GL_RGBA; break;
    case TextureBufferLayout_BGRA: result = GL_BGRA; break;
    case TextureBufferLayout_RGB:  result = GL_RGB; break;
    case TextureBufferLayout_BGR:  result = GL_BGR; break;
    case TextureBufferLayout_RG:   result = GL_RG; break;
    case TextureBufferLayout_R:    result = GL_RED; break;
    case TextureBufferLayout_Depth:result = GL_DEPTH_COMPONENT; break;
    case TextureBufferLayout_LA:   result = GL_LUMINANCE_ALPHA; break;
    default: HE_ERROR("Unkown Texture2D::BufferLayout"); break;
    }
    return result;
}
GLenum getInternalTextureBufferType( TextureBufferType bufferType )
{
    switch (bufferType)
    {
    case TextureBufferType_Byte: return GL_UNSIGNED_BYTE;
    case TextureBufferType_Float: return GL_FLOAT;
    default: HE_ERROR("Unknown Texture2D::Buffertype"); return GL_UNSIGNED_BYTE;
    }
}
GLenum getInternalTextureWrapType( TextureWrapType type )
{
    switch (type)
    {
    case TextureWrapType_Clamp: return GL_CLAMP_TO_EDGE;
    case TextureWrapType_Mirror: return GL_MIRRORED_REPEAT;
    case TextureWrapType_Repeat: return GL_REPEAT;
    default: HE_ERROR("Unknown Texture2D::WrapType"); return GL_REPEAT;
    }
}


size_t calculatePixelSize( TextureBufferLayout bufferLayout, TextureBufferType bufferType )
{
    size_t size(0);
    switch (bufferType)
    {
    case TextureBufferType_Byte:
        size = sizeof(uint8); break;
    case TextureBufferType_Float: 
        size = sizeof(float); break;
    default:
        HE_ERROR("Unknown Texture2D::Buffertype"); break;
    }
    switch (bufferLayout)
    {
    case TextureBufferLayout_RGBA:
    case TextureBufferLayout_BGRA:
        size *= 4; break;
    case TextureBufferLayout_RGB:
    case TextureBufferLayout_BGR:
        size *= 3; break;
    case TextureBufferLayout_RG:
        size *= 2; break;
    case TextureBufferLayout_Depth:
    case TextureBufferLayout_R:
        size *= 1; break;
    default:
        HE_ERROR("Unknown Texture2D::BufferLayout"); break;
    }

    return size;
}


} } } //end namespace
