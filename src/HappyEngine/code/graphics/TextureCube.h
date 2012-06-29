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

#ifndef _HE_TEXTURE_CUBE_H_
#define _HE_TEXTURE_CUBE_H_
#pragma once

namespace he {
namespace gfx {
    

class TextureCube : public Resource<TextureCube>
{
public:
//    TextureCube();
//    virtual ~TextureCube();
//
//    void init(uint tex);
//
//    bool isInitialized() const;
//
     uint getID() const;
//
//    void callbackIfLoaded(const boost::function<void()>& callback);
//
//    static uint getTextureCount();
//
//private:
//    boost::mutex m_CallbackMutex;
//    event0<void> Loaded;
//    
     uint m_Id;
//
//    static uint s_Count;
//
//    bool m_isInitialized;
//
//    //Disable default copy constructor and default assignment operator
//    TextureCube(const TextureCube&);
//    TextureCube& operator=(const TextureCube&);
};

} } //end namespace

#endif
