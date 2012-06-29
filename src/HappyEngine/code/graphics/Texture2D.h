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
//
// 23/03/2012: shared_ptr be gone!

#ifndef _HE_TEXTURE2D_H_
#define _HE_TEXTURE2D_H_
#pragma once

#include "Resource.h"

namespace he {
namespace gfx {
   
class Texture2D : public Resource<Texture2D>
{
public:
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
    enum BufferLayout
    {
        BufferLayout_R,
        BufferLayout_RG,
        BufferLayout_RGB,
        BufferLayout_RGBA,
        BufferLayout_BGR,
        BufferLayout_BGRA,
        BufferLayout_Depth,
        BufferLayout_LA
    };
    enum BufferType
    {
        BufferType_Byte,
        BufferType_Float
    };
    enum WrapType
    {
        WrapType_Clamp,
        WrapType_Mirror,
        WrapType_Repeat
    };
    enum FilterType
    {
        FilterType_Nearest         = 0,
        FilterType_Linear          = 1,
        FilterType_Anisotropic_2x  = 2,
        FilterType_Anisotropic_4x  = 4,
        FilterType_Anisotropic_8x  = 8,
        FilterType_Anisotropic_16x = 16,
        FilterType_None
    };

    Texture2D();
    virtual ~Texture2D();

    void init(WrapType wrapType, FilterType filter, TextureFormat textureFormat, bool willHaveMipMaps);

    void setData(uint width, uint height, 
        const void* pData, BufferLayout bufferLayout, BufferType bufferType, byte mipLevel = 0);

    void setCompressedData(uint width, uint height, const void* data, uint imageSizeInBytes, byte mipLevel = 0);
    
    void setLoadFinished();

    bool isInitialized() const;

    void generateMipMaps() const;

    uint getID() const;
    uint getWidth() const;
    uint getHeight() const;
    TextureFormat getTextureFormat() const;
    WrapType getWrapType() const;
    FilterType getFilterType() const;
    bool HasMipMaps() const;
        
    void callbackOnceIfLoaded(const boost::function<void()>& callback) const;

    // Static
    static uint calculatePixelSize(BufferLayout bufferLayout, BufferType bufferType);

private:
    void setFilter(FilterType filterType);

    GLenum getInternalFormat(TextureFormat format);
    GLenum getInternalBufferLayout(BufferLayout format);
    GLenum getInternalBufferType(BufferType type);
    GLenum getInternalWrapType(WrapType type);

    boost::mutex m_CallbackMutex;
    event0<void> Loaded;

    uint m_Width, m_Height;

    TextureFormat m_TextureFormat;
    
    WrapType m_WrapType;
    FilterType m_FilterType;
    bool m_HasMipMaps;

    uint m_Id;
    
    bool m_IsLoadDone;

    //Disable default copy constructor and default assignment operator
    Texture2D(const Texture2D&);
    Texture2D& operator=(const Texture2D&);
};

} } //end namespace

#endif
