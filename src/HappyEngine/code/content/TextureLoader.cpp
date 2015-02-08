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

TextureLoader::TextureLoader(): m_GCTimer(GC_TIME), m_TextureLoadQueue(10, 10, "TextureLoadQueue"), m_TextureInvokeQueue(10, 10, "TextureInvokeQueue"), m_DefaultTexture(nullptr)
{
    m_DefaultTexture = makeTexture2D(he::Color(0.5f, 0.5f, 0.0f, 1.0f));
}

TextureLoader::~TextureLoader()
{
    m_DefaultTexture->release();
    FACTORY_2D->garbageCollect();
    FACTORY_CUBE->garbageCollect();
}

void TextureLoader::tick(float /*dTime*/)
{
}

bool TextureLoader::loadTick()
{
    if (m_TextureLoadQueue.empty() == false)
    {
        TextureLoadData data;
        if (m_TextureLoadQueue.pop(data))
        {
            bool success(false);
            if (data.m_Path[0] != '_')
            {
                success = loadData(data);
            }
            else
            {
                success = makeData(data);
            }
            data.m_DataLoaded = success;
            m_TextureInvokeQueue.push(std::move(data));
        }
        return true;
    }
    return false;
}

void TextureLoader::glThreadInvoke()  //needed for all of the gl operations
{
    while (m_TextureInvokeQueue.empty() == false)
    {
        TextureLoadData data;
        if (m_TextureInvokeQueue.pop(data))
        {
            bool succes(false);
            if (data.m_DataLoaded)
            {
                succes = createTexture(data);
            }

            if (data.m_Tex.getType() == gfx::Texture2D::s_ObjectType)
                FACTORY_2D->get(data.m_Tex)->setLoaded(succes? eLoadResult_Success : eLoadResult_Failed);
            else if (data.m_Tex.getType() == gfx::TextureCube::s_ObjectType)
                FACTORY_CUBE->get(data.m_Tex)->setLoaded(succes? eLoadResult_Success : eLoadResult_Failed);
            else
                LOG(LogType_ProgrammerAssert, "Unsupported object type id! (%d)", data.m_Tex.getType());
        }
    }
}

const gfx::Texture2D* TextureLoader::asyncMakeTexture2D(const Color& color)
{
    ResourceFactory<gfx::Texture2D>* const tex2DFactory(FACTORY_2D);
    he::StringStream stream;
    stream << "__" << (int)color.rByte() << " " << (int)color.gByte() << " " << (int)color.bByte() << " " << (int)color.aByte();
    if (m_AssetContainer.isAssetPresent(stream.str()) && tex2DFactory->isAlive(m_AssetContainer.getAsset(stream.str())))
    {
        const ObjectHandle handle(m_AssetContainer.getAsset(stream.str()));
        tex2DFactory->instantiate(handle);       
        return tex2DFactory->get(handle);
    }
    else
    {
        const ObjectHandle handle(tex2DFactory->create());
        tex2DFactory->get(handle)->setName(stream.str());

        TextureLoadData data;
        data.m_Path = stream.str();
        data.m_Color = color;
        data.m_Tex = handle;

        m_TextureLoadQueue.push(std::move(data));

        m_AssetContainer.addAsset(stream.str(), handle);

        return tex2DFactory->get(handle);
    }
}
const gfx::Texture2D* TextureLoader::makeTexture2D(const Color& color)
{
    ResourceFactory<gfx::Texture2D>* const tex2DFactory(FACTORY_2D);
    he::StringStream stream;
    stream << "__" << (int)color.rByte() << " " << (int)color.gByte() << " " << (int)color.bByte() << " " << (int)color.aByte();
    if (m_AssetContainer.isAssetPresent(stream.str()) && tex2DFactory->isAlive(m_AssetContainer.getAsset(stream.str())))
    {
        const ObjectHandle handle(m_AssetContainer.getAsset(stream.str()));
        tex2DFactory->instantiate(handle); 
        return tex2DFactory->get(handle);
    }
    else
    {
        const ObjectHandle handle(tex2DFactory->create());
        m_AssetContainer.addAsset(stream.str(), handle);

        TextureLoadData data;
        data.m_Path = stream.str();
        data.m_Color = color;
        data.m_Tex = handle;

        makeData(data);

        const bool succes(createTexture(data));
        gfx::Texture2D* const tex(tex2DFactory->get(handle));
        tex->setLoaded(succes? eLoadResult_Success : eLoadResult_Failed);
        return tex;
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

        bool succes(true);
        if (loadData(data))
        {
            succes = createTexture(data);
        }
        else
        {
            succes = false;
        }
        IResource* resource(factory->getResource(handle));
        resource->setLoaded(succes? eLoadResult_Success : eLoadResult_Failed);
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

        m_TextureLoadQueue.push(std::move(data));

        m_AssetContainer.addAsset(path, handle);
    }
    return handle;
}

bool TextureLoader::createTexture( TextureLoadData& data )
{
    bool result(false);
    if (data.m_Tex.getType() == gfx::Texture2D::s_ObjectType)
        result = createTexture2D(data);
    else if (data.m_Tex.getType()  == gfx::TextureCube::s_ObjectType)
        result = createTextureCube(data);
    else
        LOG(LogType_ProgrammerAssert, "Unsupported object type id!");
    data.m_Image.destroy();
    return result;
}
bool TextureLoader::createTexture2D( const TextureLoadData& data )
{
    bool succes(true);

    gfx::Texture2D* tex2D(FACTORY_2D->get(data.m_Tex));

    if (tex2D->getName().empty())
        tex2D->setName(data.m_Path);

    tex2D->init(gfx::TextureWrapType_Repeat, gfx::TextureFilterType_Anisotropic_16x, data.m_Image.getFormat(), true);

    const he::ObjectList<Image::Face>& faces(data.m_Image.getFaces());
    const size_t faceCount(faces.size());
    if (faces.size() > 0)
    {
        HE_ART_ASSERT(faceCount == 1, "Loading texture as 2D texture but is actually a Cube texture, %s", data.m_Path.c_str());
        const he::ObjectList<Image::MipData>& mips(faces[0].m_Mips);
        mips.forEach([&](const Image::MipData& mipData)
        {
            if (mipData.isCompressed == false)
            {
                tex2D->setData(mipData.width, mipData.height, mipData.data,
                    mipData.format, mipData.type, mipData.mipLevel);
            }
            else
            {
                tex2D->setCompressedData(mipData.width, mipData.height, mipData.data, 
                    mipData.bufferSize, mipData.mipLevel);
            }
        });
        if (mips.size() == 1)
        {
            tex2D->generateMipMaps();
        }
    }
    else
    {
        HE_ERROR("Error loaded image with 0 face, %s", data.m_Path.c_str());
        succes = false;
    }
    HE_INFO("Texture2D create completed: %s", data.m_Path.c_str());
    return succes;
}
bool TextureLoader::createTextureCube( const TextureLoadData& data )
{
    bool succes(true);
    gfx::TextureCube* texCube(FACTORY_CUBE->get(data.m_Tex));

    if (texCube->getName().empty())
        texCube->setName(data.m_Path);

    texCube->init(gfx::TextureWrapType_Clamp, gfx::TextureFilterType_Anisotropic_16x, data.m_Image.getFormat(), true);

    const he::ObjectList<Image::Face>& faces(data.m_Image.getFaces());
    const size_t faceCount(faces.size());
    if (faceCount > 0)
    {
        HE_ART_ASSERT(faceCount == 6, "Loading Cube texture with not enough faces, face supplied: %d, %s", faceCount, data.m_Path.c_str());
        for (uint8 face(0); face < faceCount; ++face)
        {
            const he::ObjectList<Image::MipData>& mips(faces[face].m_Mips);
            mips.forEach([&](const Image::MipData& mipData)
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
            });
            HE_ART_ASSERT(mips.size() > 1, "TextureCube face does not have any mipmaps! %s", data.m_Path.c_str());
        }
    }
    else
    {
        HE_ERROR("Error loaded image with 0 face, %s", data.m_Path.c_str());
        succes = false;
    }

    HE_INFO("TextureCube create completed: %s", data.m_Path.c_str());
    return succes;
}


bool TextureLoader::loadData( TextureLoadData& data )
{
    bool result(data.m_Image.load(data.m_Path));
    HE_INFO("Image load completed (%s): %s", result? "SUCCESS" : "FAILED", data.m_Path.c_str());
    return result;
}
bool TextureLoader::makeData( TextureLoadData& data )
{
    data.m_Image.create(data.m_Color);
    HE_INFO("Texture color load completed: (%d,%d,%d,%d)", (int)data.m_Color.rByte(), (int)data.m_Color.gByte(), (int)data.m_Color.bByte(), (int)data.m_Color.aByte());
    return true;
}

/* GETTERS */
bool TextureLoader::isLoading() const
{
    return m_TextureLoadQueue.empty() == false || m_TextureInvokeQueue.empty() == false;
}

} } //end namespace
