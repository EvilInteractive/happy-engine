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

#ifndef _CONTENT_MANAGER_H_
#define _CONTENT_MANAGER_H_
#pragma once

#include <string>
#include "VertexLayout.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Texture2D.h"
#include "TextureLoader.h"
#include "Vector3.h"

namespace happyengine {
namespace content {

class ContentManager
{
public:
	ContentManager();
    virtual ~ContentManager();

	//graphics::Texture2D::pointer loadTexture2D(const std::string& path);
	// graphics::Texture2D::pointer asyncLoadTexture2D(const std::string& path);
	// asyncLoadTexture2D(const std::string& path, graphics::Texture2D::pointer*);

    void tick(float dTime); //checks for new load operations, if true start thread
    void glThreadInvoke();  //needed for all of the gl operations

	graphics::Model::pointer asyncLoadModel(const std::string& path, const graphics::VertexLayout& vertexLayout);
	graphics::Texture2D::pointer asyncLoadTexture(const std::string& path);

    std::vector<math::Vector3> loadPath(const std::string& path);

private:

    ModelLoader* m_pModelLoader;
    TextureLoader* m_pTextureLoader;

    //Disable default copy constructor and default assignment operator
    ContentManager(const ContentManager&);
    ContentManager& operator=(const ContentManager&);
};

} } //end namespace

#endif
