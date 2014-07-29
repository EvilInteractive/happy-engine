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

#ifndef _HE_Texture_H_
#define _HE_Texture_H_
#pragma once

namespace he {
namespace gfx {

enum TextureFormat
{
    TextureFormat_RGBA8,
    TextureFormat_RGB8,
    TextureFormat_RG8,
    TextureFormat_R8,

    TextureFormat_Compressed_RGBA8_DXT1,
    TextureFormat_Compressed_RGBA8_DXT3,
    TextureFormat_Compressed_RGBA8_DXT5,
    TextureFormat_Compressed_RGB8_DXT1,

    TextureFormat_RGBA16F,
    TextureFormat_RGB16F,
    TextureFormat_RG16F,
    TextureFormat_R16F,

    TextureFormat_RGBA16,
    TextureFormat_RGB16,
    TextureFormat_RG16,
    TextureFormat_R16,

    TextureFormat_Depth32,
    TextureFormat_Depth24_Stencil8,

};
enum TextureBufferLayout
{
    TextureBufferLayout_R,
    TextureBufferLayout_RG,
    TextureBufferLayout_RGB,
    TextureBufferLayout_RGBA,
    TextureBufferLayout_BGR,
    TextureBufferLayout_BGRA,
    TextureBufferLayout_Depth,
    TextureBufferLayout_LA
};
enum TextureBufferType
{
    TextureBufferType_Byte,
    TextureBufferType_Float
};
enum TextureWrapType
{
    TextureWrapType_Clamp,
    TextureWrapType_Mirror,
    TextureWrapType_Repeat
};
enum TextureFilterType
{
    TextureFilterType_Nearest         = 0,
    TextureFilterType_Linear          = 1,
    TextureFilterType_Anisotropic_2x  = 2,
    TextureFilterType_Anisotropic_4x  = 4,
    TextureFilterType_Anisotropic_8x  = 8,
    TextureFilterType_Anisotropic_16x = 16,
    TextureFilterType_None
};

namespace details {

GLenum getInternalTextureFormat(TextureFormat format);
GLenum getInternalTextureBufferLayout(TextureBufferLayout format);
GLenum getInternalTextureBufferType(TextureBufferType type);
GLenum getInternalTextureWrapType(TextureWrapType type);
void setTextureFilterToBoundTexture(TextureFilterType filterType, GLenum target, bool hasMipmaps);
size_t calculatePixelSize(TextureBufferLayout bufferLayout, TextureBufferType bufferType);

}

} } //end namespace

#endif
