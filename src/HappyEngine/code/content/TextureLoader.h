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

#ifndef _HE_TEXTURE_LOADER_H_
#define _HE_TEXTURE_LOADER_H_
#pragma once

#include <string>
#include <queue>
#include "Texture2D.h"

#include "boost/thread.hpp"

#include "HappyTypes.h"
#include "AssetContainer.h"
#include "Color.h"

namespace he {
namespace ct {

class TextureLoader
{
public:
	TextureLoader();
    virtual ~TextureLoader();
    
    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

    gfx::Texture2D::pointer asyncLoadTexture(const std::string& path);
    gfx::Texture2D::pointer asyncMakeTexture(const Color& color);


private:

    struct TextureLoadData
    {
        std::string path;
        byte* pData;
        uint width;
        uint height;
        uint format;
        uint id;
        Color color;
        gfx::Texture2D::pointer tex;
    };

    bool m_isLoadThreadRunning;
    void TextureLoadThread();

    
    std::queue<TextureLoadData> m_TextureLoadQueue;
    boost::mutex m_TextureLoadQueueMutex;

    std::queue<TextureLoadData> m_TextureInvokeQueue;
    boost::mutex m_TextureInvokeQueueMutex;

    boost::thread m_TextureLoadThread;

	AssetContainer<gfx::Texture2D::pointer>* m_pAssetContainer;

    //Disable default copy constructor and default assignment operator
    TextureLoader(const TextureLoader&);
    TextureLoader& operator=(const TextureLoader&);
};

} } //end namespace

#endif