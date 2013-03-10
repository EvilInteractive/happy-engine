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
#include "TextureCube.h"
#include "ResourceFactory.h"

#define FACTORY_2D ResourceFactory<gfx::Texture2D>::getInstance()
#define FACTORY_CUBE ResourceFactory<gfx::TextureCube>::getInstance()

#define GC_TIME 10.0f

namespace he {
namespace ct {

TextureLoader::TextureLoader(): m_isLoadThreadRunning(false), m_GCTimer(GC_TIME)
{
}


TextureLoader::~TextureLoader()
{
    FACTORY_2D->garbageCollect();
    FACTORY_CUBE->garbageCollect();
}


inline void handleILError(const he::String& file)
{
    ILenum err = ilGetError();
    while (err != IL_NO_ERROR)
    {
        const char* errorstr(iluErrorString(err));
        HE_ERROR("%s: %s", errorstr, file.c_str());
        err = ilGetError();
    }
}

void TextureLoader::tick(float dTime) //checks for new load operations, if true start thread
{
    if (m_isLoadThreadRunning == false)
    {
        PROFILER_BEGIN("TextureFactory garbage collect");
        if (m_GCTimer > 0.0f)
        {
            m_GCTimer -= dTime;
            if (m_GCTimer <= 0.0f)
            {
                ResourceFactory<gfx::Texture2D>* const factory(ResourceFactory<gfx::Texture2D>::getInstance());
                uint32 destoyed(factory->garbageCollect());
                if (destoyed > 0)
                {
                    HE_WARNING("TextureFactory: GC'd %d textures!", destoyed);
                }
                m_GCTimer = GC_TIME;
            }
        }
        PROFILER_END();
        if (m_TextureLoadQueue.empty() == false)
        {
            m_isLoadThreadRunning = true; //must be here else it could happen that the load thread starts twice
            m_TextureLoadThread.join();
            m_TextureLoadThread.startThread(boost::bind(&TextureLoader::TextureLoadThread, this), "TextureLoadThread");
        }
    }
}
void TextureLoader::glThreadInvoke()  //needed for all of the gl operations
{
    while (m_TextureInvokeQueue.empty() == false)
    {
        m_TextureInvokeQueueMutex.lock(FILE_AND_LINE);
        TextureLoadData data(m_TextureInvokeQueue.front());
        m_TextureInvokeQueue.pop();
        m_TextureInvokeQueueMutex.unlock();

        createTexture(data);
    }
}

const gfx::Texture2D* TextureLoader::asyncMakeTexture2D(const Color& color)
{
    std::stringstream stream;
    stream << "__" << (int)color.rByte() << " " << (int)color.gByte() << " " << (int)color.bByte() << " " << (int)color.aByte();
    if (m_AssetContainer.isAssetPresent(stream.str()) && FACTORY_2D->isAlive(m_AssetContainer.getAsset(stream.str())))
    {
        ObjectHandle handle(m_AssetContainer.getAsset(stream.str()));
        FACTORY_2D->instantiate(handle);       
        return FACTORY_2D->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY_2D->create());
        FACTORY_2D->get(handle)->setName(stream.str());

        TextureLoadData data;
        data.m_Path = stream.str();
        data.m_Color = color;
        data.m_Tex = handle;

        m_TextureLoadQueueMutex.lock(FILE_AND_LINE);
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_AssetContainer.addAsset(stream.str(), handle);

        return FACTORY_2D->get(handle);
    }
}
const gfx::Texture2D* TextureLoader::makeTexture2D(const Color& color)
{
    std::stringstream stream;
    stream << "__" << (int)color.rByte() << " " << (int)color.gByte() << " " << (int)color.bByte() << " " << (int)color.aByte();
    if (m_AssetContainer.isAssetPresent(stream.str()) && FACTORY_2D->isAlive(m_AssetContainer.getAsset(stream.str())))
    {
        ObjectHandle handle(m_AssetContainer.getAsset(stream.str()));
        FACTORY_2D->instantiate(handle); 
        return FACTORY_2D->get(handle);
    }
    else
    {
        ObjectHandle handle(FACTORY_2D->create());
        m_AssetContainer.addAsset(stream.str(), handle);

        TextureLoadData data;
        data.m_Path = stream.str();
        data.m_Color = color;
        data.m_Tex = handle;

        makeData(data);

        createTexture(data);

        return FACTORY_2D->get(handle);
    }
}
const gfx::Texture2D* TextureLoader::asyncLoadTexture2D(const he::String& path)
{
    ResourceFactory<gfx::Texture2D>* factory(FACTORY_2D);
    return factory->get(asyncLoadTexture(path, factory));
}
const gfx::Texture2D* TextureLoader::loadTexture2D(const he::String& path)
{
    ResourceFactory<gfx::Texture2D>* factory(FACTORY_2D);
    return factory->get(loadTexture(path, factory));
}
const gfx::TextureCube* TextureLoader::asyncLoadTextureCube(const he::String& path)
{
    ResourceFactory<gfx::TextureCube>* factory(FACTORY_CUBE);
    return factory->get(asyncLoadTexture(path, factory));
}
const gfx::TextureCube* TextureLoader::loadTextureCube(const he::String& path)
{
    ResourceFactory<gfx::TextureCube>* factory(FACTORY_CUBE);
    return factory->get(loadTexture(path, factory));
}

he::ObjectHandle TextureLoader::loadTexture( const he::String& path, IResourceFactory* factory )
{
    ObjectHandle handle;
    if (m_AssetContainer.isAssetPresent(path, handle) && factory->isAlive(handle))
    {
        factory->instantiate(handle); 
    }
    else
    {
        handle = factory->create();
        m_AssetContainer.addAsset(path, handle);

        TextureLoadData data;
        data.m_Path = path;
        data.m_Tex = handle;

        if (loadData(data))
        {
            createTexture(data);
        }
    }
    return handle;
}
he::ObjectHandle TextureLoader::asyncLoadTexture( const he::String& path, IResourceFactory* factory )
{
    ObjectHandle handle;
    if (m_AssetContainer.isAssetPresent(path, handle) && factory->isAlive(handle))
    {
        factory->instantiate(handle); 
    }
    else
    {
        handle = factory->create();

        TextureLoadData data;
        data.m_Path = path;
        data.m_Tex = handle;

        m_TextureLoadQueueMutex.lock(FILE_AND_LINE);
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_AssetContainer.addAsset(path, handle);
    }
    return handle;
}

bool TextureLoader::createTexture( const TextureLoadData& data )
{
    if (data.m_Tex.type == gfx::Texture2D::s_ObjectType)
        return createTexture2D(data);
    else if (data.m_Tex.type == gfx::TextureCube::s_ObjectType)
        return createTextureCube(data);
    LOG(LogType_ProgrammerAssert, "Unsupported object type id!");
    return false;
}
bool TextureLoader::createTexture2D( const TextureLoadData& data )
{
    bool succes(true);

    gfx::Texture2D* tex2D(FACTORY_2D->get(data.m_Tex));

    if (tex2D->getName() == "")
        tex2D->setName(data.m_Path);

    tex2D->init(gfx::TextureWrapType_Repeat, gfx::TextureFilterType_Anisotropic_16x, 
        data.m_TextureFormat, true);

    if (data.m_Faces > 0)
    {
        HE_ART_ASSERT(data.m_Faces == 1, "Loading texture as 2D texture but is actually a Cube texture, %s", data.m_Path.c_str());
        data.m_MipData[0].forEach([&](const TextureLoadMipData& mipData)
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
        if (data.m_MipData[0].size() == 1)
        {
            tex2D->generateMipMaps();
        }
    }
    else
    {
        HE_ERROR("Error loaded image with 0 face, %s", data.m_Path.c_str());
        succes = false;
    }

    if (data.m_IsILimage == true)
    {
        ilDeleteImage(data.m_IlImageId);
    }

    tex2D->setLoadFinished();
    HE_INFO("Texture2D create completed: %s", data.m_Path.c_str());
    return succes;
}
bool TextureLoader::createTextureCube( const TextureLoadData& data )
{
    bool succes(true);
    gfx::TextureCube* texCube(FACTORY_CUBE->get(data.m_Tex));

    if (texCube->getName() == "")
        texCube->setName(data.m_Path);

    texCube->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Anisotropic_16x, data.m_TextureFormat, true);

    if (data.m_Faces > 0)
    {
        HE_ART_ASSERT(data.m_Faces == 6, "Loading Cube texture with not enough faces, face supplied: %d, %s", data.m_Faces, data.m_Path.c_str());
        for (uint8 face(0); face < data.m_Faces; ++face)
        {
            data.m_MipData[face].forEach([&](const TextureLoadMipData& mipData)
            {
                if (mipData.isCompressed == false)
                {
                    texCube->setData(mipData.width, mipData.height, (gfx::TextureCube::Face)face, mipData.data,
                        mipData.format, mipData.type, mipData.mipLevel);
                }
                else
                {
                    texCube->setCompressedData(mipData.width, mipData.height, (gfx::TextureCube::Face)face, mipData.data, 
                        mipData.bufferSize, mipData.mipLevel );
                }
                if (mipData.isDataDirty)
                    he_free(mipData.data);
            });
            HE_ART_ASSERT(data.m_MipData[face].size() > 1, "TextureCube face does not have any mipmaps! %s", data.m_Path.c_str());
        }
    }
    else
    {
        HE_ERROR("Error loaded image with 0 face, %s", data.m_Path.c_str());
        succes = false;
    }

    if (data.m_IsILimage == true)
    {
        ilDeleteImage(data.m_IlImageId);
    }

    texCube->setLoadFinished();
    HE_INFO("TextureCube create completed: %s", data.m_Path.c_str());
    return succes;
}


bool TextureLoader::loadData( TextureLoadData& data )
{
    data.m_IlImageId = ilGenImage();
    data.m_IsILimage = true;
    data.m_TextureFormat = gfx::TextureFormat_Compressed_RGBA8_DXT5;
    data.m_Faces = 0;
    ilBindImage(data.m_IlImageId);
    if (ilLoadImage(data.m_Path.c_str()))
    {
        ILuint dxtcFormat(ilGetInteger(IL_DXTC_DATA_FORMAT));
        uint32 channels(ilGetInteger(IL_IMAGE_CHANNELS));
        bool isCompressed(true);
        if (dxtcFormat == IL_DXT_NO_COMP)
        {
            isCompressed = false;
        }
        else if (dxtcFormat == IL_DXT1)
        {
            if (channels == 4)
                data.m_TextureFormat = gfx::TextureFormat_Compressed_RGBA8_DXT1;
            else if (channels == 3)
                data.m_TextureFormat = gfx::TextureFormat_Compressed_RGB8_DXT1;
            else
            {
                HE_ERROR("Unsupported channel count (%d) for DXT1 compression - %s", channels, data.m_Path.c_str());
                return false;
            }
        }
        else if (dxtcFormat == IL_DXT3)
        {
            data.m_TextureFormat = gfx::TextureFormat_Compressed_RGBA8_DXT3;
            if (channels != 4)
            {
                HE_ERROR("Unsupported channel count (%d) for DXT3 compression - %s", channels, data.m_Path.c_str());
                return false;
            }
        }
        else if (dxtcFormat == IL_DXT5)
        {
            data.m_TextureFormat = gfx::TextureFormat_Compressed_RGBA8_DXT5;
            if (channels != 4)
            {
                HE_ERROR("Unsupported channel count (%d) for DXT5 compression - %s", channels, data.m_Path.c_str());
                return false;
            }
        }
        else
        {
            HE_ERROR("Unsupported compression type: %d", dxtcFormat);
            return false;
        }

        ILuint numMipMaps(ilGetInteger(IL_NUM_MIPMAPS));
        ILuint numFaces(ilGetInteger(IL_NUM_FACES) + 1);
        data.m_Faces = static_cast<uint8>(numFaces);
        for (uint8 face(0); face < numFaces; ++face)
        {
            HE_IF_ASSERT(face < TextureLoadData::MAX_CUBE_FACES, "More than %d faces in texture!", TextureLoadData::MAX_CUBE_FACES)
            {
                for (uint8 mip(0); mip <= numMipMaps; ++mip) // <= : 0 = normal, 1+ are mipmaps
                {
                    ilBindImage(data.m_IlImageId); // reset image else mip counter is relative to previous
                    ilActiveFace(face);
                    ilActiveMipmap(mip);

                    data.m_MipData[face].add(TextureLoadMipData());
                    TextureLoadMipData& mipData(data.m_MipData[face].back());

                    mipData.mipLevel = mip;
                    mipData.isCompressed = isCompressed;
                    mipData.width = ilGetInteger(IL_IMAGE_WIDTH);
                    mipData.height = ilGetInteger(IL_IMAGE_HEIGHT);

                    if (isCompressed)
                    {
                        mipData.bufferSize = ilGetDXTCData(nullptr, 0, dxtcFormat);
                        mipData.data = static_cast<uint8*>(he_malloc(mipData.bufferSize));
                        mipData.isDataDirty = true;
                        if (ilGetDXTCData(mipData.data, mipData.bufferSize, dxtcFormat) == 0)
                        {
                            HE_ERROR("Could not compress texture - %s", data.m_Path.c_str());
                            he_free(mipData.data);
                            mipData.isDataDirty = false;
                            handleILError(data.m_Path);
                            return false;
                        }
                    }
                    else
                    {
                        if (ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE))
                        {
                            mipData.data = ilGetData();
                            mipData.format = gfx::TextureBufferLayout_BGRA;
                            mipData.type = gfx::TextureBufferType_Byte;
                            mipData.isDataDirty = false;
                        }
                        else
                        {
                            handleILError(data.m_Path);
                            return false;
                        }
                    }
                }
            }
        }

        HE_INFO("Texture load completed: %s", data.m_Path.c_str());
        return true;
    }
    else
    {
        handleILError(data.m_Path);
        return false;
    }
}
bool TextureLoader::makeData( TextureLoadData& data )
{
    data.m_IlImageId = 0;
    data.m_IsILimage = false;
    data.m_TextureFormat = gfx::TextureFormat_Compressed_RGBA8_DXT5;
    data.m_Faces = 1;

    for (uint8 mip(0); mip < 4; ++mip)
    {
        data.m_MipData[0].add(TextureLoadMipData());
        TextureLoadMipData& mipData(data.m_MipData[0].back());

        mipData.width = static_cast<uint32>(pow(2.0f, mip));
        mipData.height = mipData.width;
        mipData.format = gfx::TextureBufferLayout_BGRA;
        mipData.type = gfx::TextureBufferType_Byte;
        mipData.isCompressed = false;
        mipData.mipLevel = 4 - mip - 1;
        uint32 pixels(mipData.width * mipData.height);
        mipData.data = NEW uint8[mipData.width * mipData.height * 4];
        mipData.isDataDirty = true;
        for (uint32 i = 0; i < pixels * 4; i += 4)
        {
            mipData.data[i]   = data.m_Color.bByte();
            mipData.data[i+1] = data.m_Color.gByte();
            mipData.data[i+2] = data.m_Color.rByte();
            mipData.data[i+3] = data.m_Color.aByte();
        }
    }
    HE_INFO("Texture color load completed: (%d,%d,%d,%d)", (int)data.m_Color.rByte(), (int)data.m_Color.gByte(), (int)data.m_Color.bByte(), (int)data.m_Color.aByte());
    return true;
}

void TextureLoader::TextureLoadThread()
{
    HE_INFO("Texture Load thread started");
    while (m_TextureLoadQueue.empty() == false)
    {
        m_TextureLoadQueueMutex.lock(FILE_AND_LINE);
        TextureLoadData data(m_TextureLoadQueue.front());
        m_TextureLoadQueue.pop();
        m_TextureLoadQueueMutex.unlock();

        if (data.m_Path[0] != '_')
        {
            if (loadData(data))
            {
                m_TextureInvokeQueueMutex.lock(FILE_AND_LINE);
                m_TextureInvokeQueue.push(data);
                m_TextureInvokeQueueMutex.unlock();
            }
        }
        else
        {
            if (makeData(data))
            {
                m_TextureInvokeQueueMutex.lock(FILE_AND_LINE);
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
