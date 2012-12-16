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
//Extended:	Sebastiaan Sprengers
//Removed concurrency queue because not cross platform: Bastian Damman - 29/10/2011

#ifndef _HE_TEXTURE_LOADER_H_
#define _HE_TEXTURE_LOADER_H_
#pragma once

#include "AssetContainer.h"
#include "Texture.h"

namespace he {
    class IResourceFactory;

namespace gfx{
    class Texture2D;
    class TextureCube;
}
namespace ct {

class TextureLoader
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    TextureLoader();
    virtual ~TextureLoader();
    
    /* GENERAL */
    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

    const gfx::Texture2D* asyncLoadTexture2D(const std::string& path);
    const gfx::TextureCube* asyncLoadTextureCube(const std::string& path);
    const gfx::Texture2D* asyncMakeTexture2D(const Color& color);

    const gfx::Texture2D* loadTexture2D(const std::string& path);
    const gfx::TextureCube* loadTextureCube(const std::string& path);
    const gfx::Texture2D* makeTexture2D(const Color& color);

    /* GETTERS */
    bool isLoading() const;

private:
    ObjectHandle asyncLoadTexture(const std::string& path, IResourceFactory* factory);
    ObjectHandle loadTexture(const std::string& path, IResourceFactory* factory);

    struct TextureLoadMipData
    {
        ~TextureLoadMipData() {}
        uint32 width;
        uint32 height;
        uint8* data;
        uint32 bufferSize;
        uint8 mipLevel;
        bool isDataDirty;
        bool isCompressed;
        gfx::TextureBufferLayout format;
        gfx::TextureBufferType type;
    };
    struct TextureLoadData
    {
        const static int MAX_CUBE_FACES = 6;

        TextureLoadData()
        {
        }
        TextureLoadData(const TextureLoadData& other)
        {
            this->operator=(other);
        }
        TextureLoadData& operator=(const TextureLoadData& other)
        {
            path = other.path;
            faces = other.faces;
            for (uint8 i(0); i < MAX_CUBE_FACES; ++i)
            {
                mipData[i].clear();
                mipData[i].append(other.mipData[i]);
            }
            textureFormat = other.textureFormat;
            ilImageId = other.ilImageId;
            isILimage = other.isILimage;
            color = other.color;
            tex = other.tex;

            return *this;
        }
        ~TextureLoadData() {}

        std::string path;
        uint8 faces;
        he::ObjectList<TextureLoadMipData> mipData[MAX_CUBE_FACES];
        gfx::TextureFormat textureFormat;
        uint32 ilImageId;
        bool isILimage;
        Color color;
        ObjectHandle tex;
    };

    bool m_isLoadThreadRunning;
    void TextureLoadThread();

    bool loadData(TextureLoadData& data);
    bool makeData(TextureLoadData& data);
    bool createTexture(const TextureLoadData& data);
    bool createTexture2D(const TextureLoadData& data);
    bool createTextureCube(const TextureLoadData& data);

    /* DATAMEMBERS */
    std::queue<TextureLoadData> m_TextureLoadQueue;
    boost::mutex m_TextureLoadQueueMutex;

    std::queue<TextureLoadData> m_TextureInvokeQueue;
    boost::mutex m_TextureInvokeQueueMutex;

    boost::thread m_TextureLoadThread;

    AssetContainer<ObjectHandle> m_AssetContainer;

    float m_GCTimer;

    /* DEFAULT COPY & ASSIGNMENT */
    TextureLoader(const TextureLoader&);
    TextureLoader& operator=(const TextureLoader&);
};

} } //end namespace

#endif
