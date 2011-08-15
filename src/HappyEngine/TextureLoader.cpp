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

#include "TextureLoader.h"

#undef UNICODE
#undef _UNICODE
#include "IL/il.h"
#include "IL/ilu.h"

#include "GL/glew.h"

#include "HappyTypes.h"
#include <iostream>
#include "Texture2D.h"

namespace happyengine {
namespace content {

TextureLoader::TextureLoader()
{
}


TextureLoader::~TextureLoader()
{
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
bool TextureLoader::load(const std::string& path, graphics::Texture2D::pointer& texture2D)
{
    ILuint id = ilGenImage();
    ilBindImage(id);
    if (ilLoadImage(path.c_str()))
    {
        if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
        {
            /*iluRotate(180);
            iluMirror();*/

            GLuint texID;
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            uint width = ilGetInteger(IL_IMAGE_WIDTH);
            uint height = ilGetInteger(IL_IMAGE_HEIGHT);
            uint format = ilGetInteger(IL_IMAGE_FORMAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, ilGetData());

            graphics::Texture2D::pointer tex2D(new graphics::Texture2D(texID, width, height, format));
            texture2D = tex2D;
        }
        else
        {
            handleILError();
            return false;
        }
        ilDeleteImage(id);
        return true;

    }
    else
    {
        handleILError();
        return false;
    }
}

} } //end namespace