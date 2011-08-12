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

#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_
#pragma once

#include "boost/shared_ptr.hpp"
#include "HappyTypes.h"

namespace happyengine {
namespace graphics {

class Texture2D
{
public:
	Texture2D();
	Texture2D(uint tex, uint width, uint height, uint bpp, uint format);
    virtual ~Texture2D();

    typedef boost::shared_ptr<Texture2D> pointer;

    uint getID() const;

private:
    uint m_Width, m_Height;
    uint m_Bpp, m_Format;

    uint m_Id;

    //Disable default copy constructor and default assignment operator
    Texture2D(const Texture2D&);
    Texture2D& operator=(const Texture2D&);
};

} } //end namespace

#endif
