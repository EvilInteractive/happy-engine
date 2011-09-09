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

#ifndef _HE_TEXTURE_LOADER_H_
#define _HE_TEXTURE_LOADER_H_
#pragma once

#include <string>
#include "Texture2D.h"

#include <ppl.h>
#include <concurrent_queue.h>

#include "boost/thread.hpp"

#include "HappyTypes.h"
#include "AssetContainer.h"

namespace happyengine {
namespace content {

class TextureLoader
{
public:
	TextureLoader();
    virtual ~TextureLoader();
    
    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

    graphics::Texture2D::pointer asyncLoadTexture(const std::string& path);


private:

    struct TextureLoadData
    {
        std::string path;
        byte* pData;
        uint width;
        uint height;
        uint format;
        uint id;
        graphics::Texture2D::pointer tex;
    };

    bool m_isLoadThreadRunning;
    void TextureLoadThread();

    Concurrency::concurrent_queue<TextureLoadData> m_TextureLoadQueue;
    Concurrency::concurrent_queue<TextureLoadData> m_TextureInvokeQueue;

    boost::thread m_TextureLoadThread;

	AssetContainer<graphics::Texture2D::pointer>* m_pAssetContainer;

    //Disable default copy constructor and default assignment operator
    TextureLoader(const TextureLoader&);
    TextureLoader& operator=(const TextureLoader&);
};

} } //end namespace

#endif
