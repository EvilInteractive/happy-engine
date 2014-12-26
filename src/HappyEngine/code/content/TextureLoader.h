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
//Extended:	Sebastiaan Sprengers
//Removed concurrency queue because not cross platform: Bastian Damman - 29/10/2011

#ifndef _HE_TEXTURE_LOADER_H_
#define _HE_TEXTURE_LOADER_H_
#pragma once

#include "AssetContainer.h"
#include "Texture.h"
#include "ThreadSafeQueueMP1C.h"
#include "Image.h"

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
    void tick(float dTime);
    bool loadTick();
    void glThreadInvoke();  //needed for all of the gl operations

    const gfx::Texture2D* asyncLoadTexture2D(const he::String& path);
    const gfx::TextureCube* asyncLoadTextureCube(const he::String& path);
    const gfx::Texture2D* asyncMakeTexture2D(const Color& color);

    const gfx::Texture2D* loadTexture2D(const he::String& path);
    const gfx::TextureCube* loadTextureCube(const he::String& path);
    const gfx::Texture2D* makeTexture2D(const Color& color);

    const gfx::Texture2D* getFallbackTexture() { return m_DefaultTexture; }

    /* GETTERS */
    bool isLoading() const;

private:
    ObjectHandle asyncLoadTexture(const he::String& path, IResourceFactory* factory);
    ObjectHandle loadTexture(const he::String& path, IResourceFactory* factory);

    struct TextureLoadData
    {
        const static int MAX_CUBE_FACES = 6;

        TextureLoadData(): m_Path(""), m_Tex(ObjectHandle::unassigned), 
            m_Color(0.0f, 0.0f, 0.0f, 0.0f), m_DataLoaded(false)
        {
        }
        TextureLoadData(TextureLoadData&& other): 
            m_Path(std::move(other.m_Path)), m_Tex(other.m_Tex), m_Image(std::move(other.m_Image)),
            m_Color(other.m_Color), m_DataLoaded(other.m_DataLoaded)
        {
        }
        TextureLoadData& operator=(TextureLoadData&& other)
        {
            m_Path = std::move(other.m_Path);
            m_Tex = other.m_Tex;
            m_Image = std::move(other.m_Image);
            m_Color = other.m_Color;
            m_DataLoaded = other.m_DataLoaded;
            return *this;
        }
        ~TextureLoadData() {}

        he::String m_Path;
        ObjectHandle m_Tex;
        Image m_Image;
        Color m_Color;
        bool m_DataLoaded;

    private:
        TextureLoadData(const TextureLoadData&);
        TextureLoadData& operator=(const TextureLoadData&);
    };
    
    bool loadData(TextureLoadData& data);
    bool makeData(TextureLoadData& data);
    bool createTexture(TextureLoadData& data);
    bool createTexture2D(const TextureLoadData& data);
    bool createTextureCube(const TextureLoadData& data);

    /* DATAMEMBERS */
    he::ThreadSafeQueueMP1C<TextureLoadData> m_TextureLoadQueue;
    he::ThreadSafeQueueMP1C<TextureLoadData> m_TextureInvokeQueue;

    AssetContainer<ObjectHandle> m_AssetContainer;

    const gfx::Texture2D* m_DefaultTexture;

    float m_GCTimer;

    /* DEFAULT COPY & ASSIGNMENT */
    TextureLoader(const TextureLoader&);
    TextureLoader& operator=(const TextureLoader&);
};

} } //end namespace

#endif
