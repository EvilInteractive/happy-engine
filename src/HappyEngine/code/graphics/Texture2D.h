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
//
// 23/03/2012: shared_ptr be gone!

#ifndef _HE_TEXTURE2D_H_
#define _HE_TEXTURE2D_H_
#pragma once

#include "HeAssert.h"
#undef assert
#define assert HE_ASSERT

#include "HappyTypes.h"

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

#include "event.h"

#include "Resource.h"

namespace he {
namespace gfx {
    
class Texture2D : public Resource<Texture2D>
{
public:
    Texture2D();
    virtual ~Texture2D();

    void init(uint tex, uint width, uint height, uint format, void* pixels = nullptr, uint bufferSize = 0);
    
    bool isInitialized() const;

    uint getID() const;
    uint getWidth() const;
    uint getHeight() const;
    uint getFormat() const;

    void* getPixelsIfAvailable() const;

    void callbackIfLoaded(const boost::function<void()>& callback) const;

    static uint getTextureCount();

private:
    boost::mutex m_CallbackMutex;
    event<void> Loaded;

    uint m_Width, m_Height;
    uint m_Format;

    uint m_Id;

    static uint s_Count;

    void* m_pPixels;

    bool m_isInitialized;

    //Disable default copy constructor and default assignment operator
    Texture2D(const Texture2D&);
    Texture2D& operator=(const Texture2D&);
};

} } //end namespace

#endif
