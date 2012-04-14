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
#include "HappyPCH.h" 

#include "TextureLoader.h"

#undef UNICODE
#undef _UNICODE
#include "IL/il.h"
#include "IL/ilu.h"

#include "Texture2D.h"
#include "ResourceFactory.h"

#define FACTORY ResourceFactory<gfx::Texture2D>::getInstance()

namespace he {
namespace ct {

TextureLoader::TextureLoader(): m_isLoadThreadRunning(false),
                                m_pAssetContainer(NEW AssetContainer<ObjectHandle>())
{
}


TextureLoader::~TextureLoader()
{
    delete m_pAssetContainer;
}


inline void handleILError(const std::string& file)
{
    ILenum err = ilGetError();
    while (err != IL_NO_ERROR)
    {
        const char* errorstr(iluErrorString(err));
        HE_ERROR("%s: %s", errorstr, file);
        err = ilGetError();
    }
}

void TextureLoader::tick(float /*dTime*/) //checks for new load operations, if true start thread
{
    if (m_isLoadThreadRunning == false)
    {
        PROFILER_BEGIN("TextureFactory garbage collect");
        //ResourceFactory<gfx::Texture2D>::getInstance()->garbageCollect();
        PROFILER_END();
        if (m_TextureLoadQueue.empty() == false)
        {
            m_isLoadThreadRunning = true; //must be here else it could happen that the load thread starts twice
            m_TextureLoadThread = boost::thread(boost::bind(&TextureLoader::TextureLoadThread, this));
        }
    }
}
void TextureLoader::glThreadInvoke()  //needed for all of the gl operations
{
    while (m_TextureInvokeQueue.empty() == false)
    {
        m_TextureInvokeQueueMutex.lock();
        TextureLoadData data(m_TextureInvokeQueue.front());
        m_TextureInvokeQueue.pop();
        m_TextureInvokeQueueMutex.unlock();

        createTexture(data);
    }
}

const gfx::Texture2D* TextureLoader::asyncMakeTexture(const Color& color)
{
    std::stringstream stream;
    stream << "__" << (int)color.rByte() << " " << (int)color.gByte() << " " << (int)color.bByte() << " " << (int)color.aByte();
    if (m_pAssetContainer->isAssetPresent(stream.str()) && FACTORY->isAlive(m_pAssetContainer->getAsset(stream.str())))
    {
        ObjectHandle handle(m_pAssetContainer->getAsset(stream.str()));
        FACTORY->instantiate(handle);       
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());

        TextureLoadData data;
        data.path = "";
        data.id = 0;
        data.pData = 0;
        data.width = 0;
        data.height = 0;
        data.format = 0;
        data.color = color;
        data.tex = handle;

        m_TextureLoadQueueMutex.lock();
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_pAssetContainer->addAsset(stream.str(), handle);

        return FACTORY->get(handle);
    }
}
const gfx::Texture2D* TextureLoader::asyncLoadTexture(const std::string& path, bool storePixelsInTexture)
{
    if (m_pAssetContainer->isAssetPresent(path) && FACTORY->isAlive(m_pAssetContainer->getAsset(path)))
    {
        ObjectHandle handle(m_pAssetContainer->getAsset(path));
        FACTORY->instantiate(handle); 
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());

        TextureLoadData data;
        data.path = path;
        data.id = 0;
        data.pData = 0;
        data.width = 0;
        data.height = 0;
        data.format = 0;
        data.tex = handle;
        data.storePixels = storePixelsInTexture;

        m_TextureLoadQueueMutex.lock();
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_pAssetContainer->addAsset(path, handle);

        return FACTORY->get(handle);
    }
}
const gfx::Texture2D* TextureLoader::loadTexture(const std::string& path, bool storePixelsInTexture)
{
    if (m_pAssetContainer->isAssetPresent(path) && FACTORY->isAlive(m_pAssetContainer->getAsset(path)))
    {
        ObjectHandle handle(m_pAssetContainer->getAsset(path));
        FACTORY->instantiate(handle); 
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());
        m_pAssetContainer->addAsset(path, handle);

        TextureLoadData data;
        data.path = path;
        data.id = 0;
        data.pData = 0;
        data.width = 0;
        data.height = 0;
        data.format = 0;
        data.tex = handle;
        data.storePixels = storePixelsInTexture;

        if (loadData(data))
        {
            createTexture(data);
        }

        return FACTORY->get(handle);
    }
}

const gfx::Texture2D* TextureLoader::makeTexture(const Color& color)
{
    std::stringstream stream;
    stream << "__" << (int)color.rByte() << " " << (int)color.gByte() << " " << (int)color.bByte() << " " << (int)color.aByte();
    if (m_pAssetContainer->isAssetPresent(stream.str()) && FACTORY->isAlive(m_pAssetContainer->getAsset(stream.str())))
    {
        ObjectHandle handle(m_pAssetContainer->getAsset(stream.str()));
        FACTORY->instantiate(handle); 
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());
        m_pAssetContainer->addAsset(stream.str(), handle);

        TextureLoadData data;
        data.path = stream.str();
        data.id = 0;
        data.format = 0;
        data.color = color;
        data.tex = handle;

        makeData(data);

        createTexture(data);

        return FACTORY->get(handle);
    }
}

bool TextureLoader::createTexture( const TextureLoadData& data )
{
    GLuint texID;
    glGenTextures(1, &texID);
    GL::heBindTexture2D(0, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.pData);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (data.storePixels)
        FACTORY->get(data.tex)->init(texID, data.width, data.height, data.format, data.pData, 4 * data.width * data.height);
    else
        FACTORY->get(data.tex)->init(texID, data.width, data.height, data.format);

    FACTORY->get(data.tex)->setName(data.path);

    if (data.path != "")
        ilDeleteImage(data.id);
    else
        delete data.pData;

    HE_INFO("Texture create completed: %s", data.path.c_str());

    return true;
}

bool TextureLoader::loadData( TextureLoadData& data )
{
    ILuint id = ilGenImage();
    ilBindImage(id);
    if (ilLoadImage(data.path.c_str()))
    {
        if (ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE))
        {
            iluFlipImage();
            data.id = id;
            data.width = ilGetInteger(IL_IMAGE_WIDTH);
            data.height = ilGetInteger(IL_IMAGE_HEIGHT);
            data.format = ilGetInteger(IL_IMAGE_FORMAT);
            data.pData = ilGetData();

            HE_INFO("Texture load completed: %s", data.path.c_str());

            return true;
        }
        else
        {
            handleILError(data.path);
        }
    }
    else
    {
        handleILError(data.path);
    }
    return false;
}

bool TextureLoader::makeData( TextureLoadData& data )
{
    data.id = 0;
    data.width = 8;
    data.height = 8;
    data.format = GL_BGRA;
    data.pData = NEW byte[8*8*4];
    for (uint i = 0; i < 64*4; i += 4)
    {
        data.pData[i] = data.color.bByte();
        data.pData[i+1] = data.color.gByte();
        data.pData[i+2] = data.color.rByte();
        data.pData[i+3] = data.color.aByte();
    }
    HE_INFO("Texture color load completed: (%d,%d,%d,%d)", (int)data.color.rByte(), (int)data.color.gByte(), (int)data.color.bByte(), (int)data.color.aByte());
    return true;
}

void TextureLoader::TextureLoadThread()
{
    HE_INFO("Texture Load thread started");
    while (m_TextureLoadQueue.empty() == false)
    {
        m_TextureLoadQueueMutex.lock();
        TextureLoadData data(m_TextureLoadQueue.front());
        m_TextureLoadQueue.pop();
        m_TextureLoadQueueMutex.unlock();

        if (data.path != "")
        {
            if (loadData(data))
            {
                m_TextureInvokeQueueMutex.lock();
                m_TextureInvokeQueue.push(data);
                m_TextureInvokeQueueMutex.unlock();
            }
        }
        else
        {
            if (makeData(data))
            {
                m_TextureInvokeQueueMutex.lock();
                m_TextureInvokeQueue.push(data);
                m_TextureInvokeQueueMutex.unlock();
            }
        }
    }
    m_isLoadThreadRunning = false;
    HE_INFO("Texture load thread stopped");
}

/* GETTERS */
bool TextureLoader::isLoading() const
{
    return m_isLoadThreadRunning;
}

} } //end namespace