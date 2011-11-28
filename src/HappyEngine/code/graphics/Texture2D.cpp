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
#include "StdAfx.h" 

#include "Texture2D.h"
#include "GL/glew.h"
#include "SDL_ttf.h"

namespace he {
namespace gfx {

uint Texture2D::s_Count = 0;

Texture2D::Texture2D(): m_Id(UINT_MAX), m_Width(0), m_Height(0), m_Format(0), m_isInitialized(false)
{
    ++s_Count;
}

void Texture2D::init(uint tex, uint width, uint height, uint format)
{
    if (m_Id != UINT_MAX)
        glDeleteTextures(1, &m_Id);
    m_Id = tex;
    m_Width = width;
    m_Height = height;
    m_Format = format;
    m_isInitialized = true;
}

bool Texture2D::isInitialized() const
{
    return m_isInitialized;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_Id);
    --s_Count;
}

uint Texture2D::getTextureCount()
{
    return s_Count;
}


uint Texture2D::getID() const
{
    return m_Id;
}

uint Texture2D::getWidth() const
{
    return m_Width;
}
uint Texture2D::getHeight() const
{
    return m_Height;
}

} } //end namespace