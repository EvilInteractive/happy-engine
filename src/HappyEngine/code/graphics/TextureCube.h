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

#ifndef _HE_TEXTURE_CUBE_H_
#define _HE_TEXTURE_CUBE_H_
#pragma once

#include "Resource.h"
#include "Texture.h"

namespace he {
namespace gfx {
    

class TextureCube : public Resource<TextureCube>
{
public:
    enum Face
    {
        Face_PositiveX,
        Face_NegativeX,
        Face_PositiveY,
        Face_NegativeY,
        Face_PositiveZ,
        Face_NegativeZ
    };

    TextureCube();
    virtual ~TextureCube();

    void init(TextureWrapType wrapType, TextureFilterType filter, TextureFormat textureFormat, bool willHaveMipMaps);

    void setData(uint32 width, uint32 height, const Face& face,
        const void* pData, TextureBufferLayout bufferLayout, TextureBufferType bufferType, uint8 mipLevel = 0);

    void setCompressedData(uint32 width, uint32 height, const Face& face, 
        const void* data, uint32 imageSizeInBytes, uint8 mipLevel = 0);

    void setLoadFinished();
    bool isInitialized() const { return m_Id != UINT_MAX; }

    void generateMipMaps() const;

    uint32 getID() const { return m_Id; }  
    uint32 getWidth() const { return m_Width; }
    uint32 getHeight() const { return m_Height; }
    TextureFormat getTextureFormat() const { return m_TextureFormat; }
    TextureWrapType getWrapType() const { return m_WrapType; }
    TextureFilterType getFilterType() const { return m_FilterType; }
    bool HasMipMaps() const { return m_HasMipMaps; }

    void callbackOnceIfLoaded(const boost::function<void()>& callback) const;

private:
    boost::mutex m_CallbackMutex;
    event0<void> Loaded;
    bool m_IsLoadDone;
   
    uint32 m_Id;
    uint32 m_Width, m_Height;
    TextureFormat m_TextureFormat;
    TextureWrapType m_WrapType;
    TextureFilterType m_FilterType;
    bool m_HasMipMaps;

    //Disable default copy constructor and default assignment operator
    TextureCube(const TextureCube&);
    TextureCube& operator=(const TextureCube&);
};

} } //end namespace

#endif
