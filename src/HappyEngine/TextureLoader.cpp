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

#include "TextureLoader.h"

#undef UNICODE
#undef _UNICODE
#include "IL/il.h"
#include "IL/ilu.h"

#include "GL/glew.h"

#include "HappyTypes.h"
#include <iostream>
#include "Texture2D.h"

#include "HappyNew.h"

namespace happyengine {
namespace content {

TextureLoader::TextureLoader(): m_isLoadThreadRunning(false),
								m_pAssetContainer(NEW AssetContainer<graphics::Texture2D::pointer>())
{
}


TextureLoader::~TextureLoader()
{
	delete m_pAssetContainer;
}


inline void handleILError()
{
    ILenum error = ilGetError();
    while (error != IL_NO_ERROR)
    {
        std::cout << iluErrorString(error);
        error = ilGetError();
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
        TextureLoadData data;
        if (m_TextureInvokeQueue.try_pop(data))
        {
            GLuint texID;
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.pData);
            glGenerateMipmap(GL_TEXTURE_2D);

            ilDeleteImage(data.id);

            data.tex->init(texID, data.width, data.height, data.format);

            std::cout << "**TL INFO** texture create completed: " << data.path << "\n";
        }
    }
}

graphics::Texture2D::pointer TextureLoader::asyncLoadTexture(const std::string& path)
{
	if (m_pAssetContainer->IsAssetPresent(path))
	{
		return m_pAssetContainer->GetAsset(path);
	}
	else
	{
		graphics::Texture2D::pointer tex2D(NEW graphics::Texture2D());

		TextureLoadData data;
		data.path = path;
		data.id = 0;
		data.pData = 0;
		data.width = 0;
		data.height = 0;
		data.format = 0;
		data.tex = tex2D;

		m_TextureLoadQueue.push(data);

		m_pAssetContainer->AddAsset(path, tex2D);

		return tex2D;
	}
}

void TextureLoader::TextureLoadThread()
{
    std::cout << "**TL INFO** load thread started.\n";
    while (m_TextureLoadQueue.empty() == false)
    {
        TextureLoadData data;
        if (m_TextureLoadQueue.try_pop(data))
        {
            ILuint id = ilGenImage();
            ilBindImage(id);
            if (ilLoadImage(data.path.c_str()))
            {
                if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
                {
                    iluFlipImage();
                    data.id = id;
                    data.width = ilGetInteger(IL_IMAGE_WIDTH);
                    data.height = ilGetInteger(IL_IMAGE_HEIGHT);
                    data.format = ilGetInteger(IL_IMAGE_FORMAT);
                    data.pData = ilGetData();
                    m_TextureInvokeQueue.push(data);
                    std::cout << "**TL INFO** obj load completed: " << data.path << "\n";
                }
                else
                {
                    handleILError();
                }
            }
            else
            {
                handleILError();
            }
        }
    }
    m_isLoadThreadRunning = false;
    std::cout << "**TL INFO** load thread stopped.\n";
}

} } //end namespace