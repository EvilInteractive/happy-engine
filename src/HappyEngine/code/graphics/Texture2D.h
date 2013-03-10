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
#include "Texture.h"

namespace he {
namespace gfx {
   
class Texture2D : public Resource<Texture2D>
{
public:
    Texture2D();
    virtual ~Texture2D();

    void init(TextureWrapType wrapType, TextureFilterType filter, TextureFormat textureFormat, bool willHaveMipMaps);

    void setData(uint32 width, uint32 height, 
        const void* pData, TextureBufferLayout bufferLayout, TextureBufferType bufferType, uint8 mipLevel = 0);

    void setCompressedData(uint32 width, uint32 height, const void* data, uint32 imageSizeInBytes, uint8 mipLevel = 0);
    
    void setLoadFinished();
    bool isLoaded() const { return m_IsLoadDone; }

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


    he::Mutex m_CallbackMutex;
    event0<void> Loaded;

    uint32 m_Width, m_Height;

    TextureFormat m_TextureFormat;    
    TextureWrapType m_WrapType;
    TextureFilterType m_FilterType;
    bool m_HasMipMaps;

    uint32 m_Id;
    
    bool m_IsLoadDone;

    //Disable default copy constructor and default assignment operator
    Texture2D(const Texture2D&);
    Texture2D& operator=(const Texture2D&);
};

} } //end namespace

#endif
