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
#include "HappyPCH.h" 

#include "TextureCube.h"

namespace he {
namespace gfx {
//
//uint TextureCube::s_Count = 0;
//
//TextureCube::TextureCube(): m_Id(UINT_MAX), m_isInitialized(false)
//{
//    ++s_Count;
//}
//
//void TextureCube::init(uint tex)
//{
//    if (m_Id != UINT_MAX)
//        glDeleteTextures(1, &m_Id);
//    m_Id = tex;
//    m_CallbackMutex.lock();
//    m_isInitialized = true;
//    Loaded();
//    m_CallbackMutex.unlock();
//}
//
//bool TextureCube::isInitialized() const
//{
//    return m_isInitialized;
//}
//
//TextureCube::~TextureCube()
//{
//    glDeleteTextures(1, &m_Id);
//    --s_Count;
//}
//
//uint TextureCube::getTextureCount()
//{
//    return s_Count;
//}
//
//
uint TextureCube::getID() const
{
    return m_Id;
}
//
//void TextureCube::callbackIfLoaded( const boost::function<void()>& callback )
//{
//    m_CallbackMutex.lock();
//    if (m_isInitialized)
//    {
//        m_CallbackMutex.unlock();
//        callback();
//    }
//    else
//    {
//        Loaded += callback;
//        m_CallbackMutex.unlock();
//    }
//}
//
} } //end namespace