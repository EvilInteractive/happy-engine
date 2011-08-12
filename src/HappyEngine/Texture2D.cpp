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

#include "Texture2D.h"
#include "GL/glew.h"

namespace happyengine {
namespace graphics {

Texture2D::Texture2D(uint tex, uint width, uint height, uint bpp, uint format): m_Id(tex),
    m_Width(width), m_Height(height), m_Bpp(bpp), m_Format(format)
{
}


Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_Id);
}


uint Texture2D::getID() const
{
    return m_Id;
}

} } //end namespace