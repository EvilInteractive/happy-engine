//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include "OpenGL.h"

#include "HappyTypes.h"
#include <iostream>
#include "Texture2D.h"

#include "HappyEngine.h"
#include "Console.h"

#include "HappyNew.h"

namespace he {
namespace ct {

TextureLoader::TextureLoader(): m_isLoadThreadRunning(false),
                                m_pAssetContainer(NEW AssetContainer<gfx::Texture2D::pointer>())
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
        std::cout << errorstr << ": " << file << "\n";
        CONSOLE->addMessage(std::string(errorstr) + ": " + file, CMSG_TYPE_ERROR);
        err = ilGetError();
    }
}

void TextureLoader::tick(float /*dTime*/) //checks for new load operations, if true start thread
{
    if (m_isLoadThreadRunning == false)
        if (m_TextureLoadQueue.empty() == false)
        {
            m_isLoadThreadRunning = true; //must be here else it could happen that the load thread starts twice
            m_TextureLoadThread = boost::thread(boost::bind(&TextureLoader::TextureLoadThread, this));
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

        GLuint texID;
        glGenTextures(1, &texID);
        GL::heBindTexture2D(0, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.pData);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (data.path != "")
            ilDeleteImage(data.id);
        else
            delete data.pData;

        data.tex->init(texID, data.width, data.height, data.format);

        std::cout << "**TL INFO** texture create completed: " << data.path << "\n";
    }
}

gfx::Texture2D::pointer TextureLoader::asyncMakeTexture(const Color& color)
{
    std::stringstream stream;
    stream << "__" << (int)color.rByte() << " " << (int)color.gByte() << " " << (int)color.bByte() << " " << (int)color.aByte();
    if (m_pAssetContainer->isAssetPresent(stream.str()))
    {
        return m_pAssetContainer->getAsset(stream.str());
    }
    else
    {
        gfx::Texture2D::pointer tex2D(NEW gfx::Texture2D());

        TextureLoadData data;
        data.path = "";
        data.id = 0;
        data.pData = 0;
        data.width = 0;
        data.height = 0;
        data.format = 0;
        data.color = color;
        data.tex = tex2D;

        m_TextureLoadQueueMutex.lock();
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_pAssetContainer->addAsset(stream.str(), tex2D);

        return tex2D;
    }
}
gfx::Texture2D::pointer TextureLoader::asyncLoadTexture(const std::string& path)
{
    if (m_pAssetContainer->isAssetPresent(path))
    {
        return m_pAssetContainer->getAsset(path);
    }
    else
    {
        gfx::Texture2D::pointer tex2D(NEW gfx::Texture2D());

        TextureLoadData data;
        data.path = path;
        data.id = 0;
        data.pData = 0;
        data.width = 0;
        data.height = 0;
        data.format = 0;
        data.tex = tex2D;

        m_TextureLoadQueueMutex.lock();
        m_TextureLoadQueue.push(data);
        m_TextureLoadQueueMutex.unlock();

        m_pAssetContainer->addAsset(path, tex2D);

        return tex2D;
    }
}

void TextureLoader::TextureLoadThread()
{
    std::cout << "**TL INFO** load thread started.\n";
    while (m_TextureLoadQueue.empty() == false)
    {
        m_TextureLoadQueueMutex.lock();
        TextureLoadData data(m_TextureLoadQueue.front());
        m_TextureLoadQueue.pop();
        m_TextureLoadQueueMutex.unlock();

        if (data.path != "")
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

                    m_TextureInvokeQueueMutex.lock();
                    m_TextureInvokeQueue.push(data);
                    m_TextureInvokeQueueMutex.unlock();

                    std::cout << "**TL INFO** obj load completed: " << data.path << "\n";
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
        }
        else
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
            m_TextureInvokeQueueMutex.lock();
            m_TextureInvokeQueue.push(data);
            m_TextureInvokeQueueMutex.unlock();
            std::cout << "**TL INFO** obj load completed: " << data.path << "\n";
        }
    }
    m_isLoadThreadRunning = false;
    std::cout << "**TL INFO** load thread stopped.\n";
}

} } //end namespace