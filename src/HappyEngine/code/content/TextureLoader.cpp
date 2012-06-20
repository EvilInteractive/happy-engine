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

TextureLoader::TextureLoader(): m_isLoadThreadRunning(false)
{
}


TextureLoader::~TextureLoader()
{
}


inline void handleILError(const std::string& file)
{
    ILenum err = ilGetError();
    while (err != IL_NO_ERROR)
    {
        const char* errorstr(iluErrorString(err));
        HE_ERROR("%s: %s", errorstr, file.c_str());
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
    if (m_AssetContainer.isAssetPresent(stream.str()) && FACTORY->isAlive(m_AssetContainer.getAsset(stream.str())))
    {
        ObjectHandle handle(m_AssetContainer.getAsset(stream.str()));
        FACTORY->instantiate(handle);       
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());
        FACTORY->get(handle)->setName(stream.str());

        TextureLoadData data;
        data.path = stream.str();
        data.color = color;
        data.tex = handle;

        m_TextureLoadQueueMutex.lock();
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_AssetContainer.addAsset(stream.str(), handle);

        return FACTORY->get(handle);
    }
}
const gfx::Texture2D* TextureLoader::asyncLoadTexture(const std::string& path)
{
    if (m_AssetContainer.isAssetPresent(path) && FACTORY->isAlive(m_AssetContainer.getAsset(path)))
    {
        ObjectHandle handle(m_AssetContainer.getAsset(path));
        FACTORY->instantiate(handle); 
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());

        TextureLoadData data;
        data.path = path;
        data.tex = handle;

        m_TextureLoadQueueMutex.lock();
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_AssetContainer.addAsset(path, handle);

        return FACTORY->get(handle);
    }
}
const gfx::Texture2D* TextureLoader::loadTexture(const std::string& path)
{
    if (m_AssetContainer.isAssetPresent(path) && FACTORY->isAlive(m_AssetContainer.getAsset(path)))
    {
        ObjectHandle handle(m_AssetContainer.getAsset(path));
        FACTORY->instantiate(handle); 
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());
        m_AssetContainer.addAsset(path, handle);

        TextureLoadData data;
        data.path = path;
        data.tex = handle;

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
    if (m_AssetContainer.isAssetPresent(stream.str()) && FACTORY->isAlive(m_AssetContainer.getAsset(stream.str())))
    {
        ObjectHandle handle(m_AssetContainer.getAsset(stream.str()));
        FACTORY->instantiate(handle); 
        return FACTORY->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY->create());
        m_AssetContainer.addAsset(stream.str(), handle);

        TextureLoadData data;
        data.path = stream.str();
        data.color = color;
        data.tex = handle;

        makeData(data);

        createTexture(data);

        return FACTORY->get(handle);
    }
}

bool TextureLoader::createTexture( const TextureLoadData& data )
{
    gfx::Texture2D* tex2D(FACTORY->get(data.tex));

    if (tex2D->getName() == "")
        tex2D->setName(data.path);

    tex2D->init(gfx::Texture2D::WrapType_Repeat, gfx::Texture2D::FilterType_Anisotropic_16x, 
                data.textureFormat, true);

    std::for_each(data.mipData.cbegin(), data.mipData.cend(), [&](const TextureLoadMipData& mipData)
    {
        if (mipData.isCompressed == false)
        {
            tex2D->setData(mipData.width, mipData.height, mipData.data,
                mipData.format, mipData.type, mipData.mipLevel);
        }
        else
        {
            tex2D->setCompressedData(mipData.width, mipData.height, mipData.data, 
                mipData.bufferSize, mipData.mipLevel );
        }
        if (mipData.isDataDirty)
            he_free(mipData.data);
    });
    if (data.mipData.size() == 1)
    {
        tex2D->generateMipMaps();
    }

    if (data.isILimage == true)
    {
        ilDeleteImage(data.ilImageId);
    }

    tex2D->setLoadFinished();
    HE_INFO("Texture create completed: %s", data.path.c_str());

    return true;
}

bool TextureLoader::loadData( TextureLoadData& data )
{
    data.ilImageId = ilGenImage();
    data.isILimage = true;
    data.textureFormat = gfx::Texture2D::TextureFormat_Compressed_RGBA8_DXT5;
    ilBindImage(data.ilImageId);
    if (ilLoadImage(data.path.c_str()))
    {
        ILuint dxtcFormat(ilGetInteger(IL_DXTC_DATA_FORMAT));
        uint channels(ilGetInteger(IL_IMAGE_CHANNELS));
        bool isCompressed(true);
        if (dxtcFormat == IL_DXT_NO_COMP)
        {
            isCompressed = false;
        }
        else if (dxtcFormat == IL_DXT1)
        {
            if (channels == 4)
                data.textureFormat = gfx::Texture2D::TextureFormat_Compressed_RGBA8_DXT1;
            else if (channels == 3)
                data.textureFormat = gfx::Texture2D::TextureFormat_Compressed_RGB8_DXT1;
            else
            {
                HE_ERROR("Unsupported channel count (%d) for DXT1 compression - %s", channels, data.path);
                return false;
            }
        }
        else if (dxtcFormat == IL_DXT3)
        {
            data.textureFormat = gfx::Texture2D::TextureFormat_Compressed_RGBA8_DXT3;
            if (channels != 4)
            {
                HE_ERROR("Unsupported channel count (%d) for DXT3 compression - %s", channels, data.path);
                return false;
            }
        }
        else if (dxtcFormat == IL_DXT5)
        {
            data.textureFormat = gfx::Texture2D::TextureFormat_Compressed_RGBA8_DXT5;
            if (channels != 4)
            {
                HE_ERROR("Unsupported channel count (%d) for DXT5 compression - %s", channels, data.path);
                return false;
            }
        }
        else
        {
            HE_ERROR("Unsupported compression type: %d", dxtcFormat);
            return false;
        }

        ILuint numMipMaps(ilGetInteger(IL_NUM_MIPMAPS));
        for (byte mip(0); mip <= numMipMaps; ++mip) // <= : 0 = normal, 1+ are mipmaps
        {
            ilBindImage(data.ilImageId); // reset image else mip counter is relative to previous
            ilActiveMipmap(mip);

            data.mipData.push_back(TextureLoadMipData());
            TextureLoadMipData& mipData(data.mipData.back());

            mipData.mipLevel = mip;
            mipData.isCompressed = isCompressed;
            mipData.width = ilGetInteger(IL_IMAGE_WIDTH);
            mipData.height = ilGetInteger(IL_IMAGE_HEIGHT);

            if (isCompressed)
            {
                mipData.bufferSize = ilGetDXTCData(nullptr, 0, dxtcFormat);
                mipData.data = static_cast<byte*>(he_malloc(mipData.bufferSize));
                mipData.isDataDirty = true;
                if (ilGetDXTCData(mipData.data, mipData.bufferSize, dxtcFormat) == 0)
                {
                    HE_ERROR("Could not compress texture - %s", data.path);
                    he_free(mipData.data);
                    mipData.isDataDirty = false;
                    handleILError(data.path);
                    return false;
                }
            }
            else
            {
                if (ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE))
                {
                    mipData.data = ilGetData();
                    mipData.format = gfx::Texture2D::BufferLayout_BGRA;
                    mipData.type = gfx::Texture2D::BufferType_Byte;
                    mipData.isDataDirty = false;
                }
                else
                {
                    handleILError(data.path);
                    return false;
                }
            }
        }

        HE_INFO("Texture load completed: %s", data.path.c_str());
        return true;
    }
    else
    {
        handleILError(data.path);
        return false;
    }
}

bool TextureLoader::makeData( TextureLoadData& data )
{
    data.ilImageId = 0;
    data.isILimage = false;
    data.textureFormat = gfx::Texture2D::TextureFormat_Compressed_RGBA8_DXT5;

    for (byte mip(0); mip < 4; ++mip)
    {
        data.mipData.push_back(TextureLoadMipData());
        TextureLoadMipData& mipData(data.mipData.back());

        mipData.width = static_cast<uint>(pow(2.0f, mip));
        mipData.height = mipData.width;
        mipData.format = gfx::Texture2D::BufferLayout_BGRA;
        mipData.type = gfx::Texture2D::BufferType_Byte;
        mipData.isCompressed = false;
        mipData.mipLevel = 4 - mip - 1;
        uint pixels(mipData.width * mipData.height);
        mipData.data = NEW byte[mipData.width * mipData.height * 4];
        mipData.isDataDirty = true;
        for (uint i = 0; i < pixels * 4; i += 4)
        {
            mipData.data[i]   = data.color.bByte();
            mipData.data[i+1] = data.color.gByte();
            mipData.data[i+2] = data.color.rByte();
            mipData.data[i+3] = data.color.aByte();
        }
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

        if (data.path[0] != '_')
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