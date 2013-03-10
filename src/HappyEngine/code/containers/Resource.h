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
//Created: 17/03/2012

#ifndef _HE_RESOURCE_H_
#define _HE_RESOURCE_H_
#pragma once

#include "ResourceFactory.h"

namespace he {

template<typename T>
class Resource
{
DECLARE_OBJECT(Resource<T>)
public:
    Resource()
    {
    }
    virtual ~Resource()
    {
    }

    virtual void release() const
    {
        ResourceFactory<T>::getInstance()->release(m_Handle);
    }
    virtual void instantiate() const
    {
        ResourceFactory<T>::getInstance()->instantiate(m_Handle);
    }

    void setName(const he::String& name)
    {
        m_Name = name;
    }
    const he::String& getName() const
    {
        return m_Name;
    }

    virtual bool canBeGarbageCollected() const
    {
        return true;
    }

private:

    he::String m_Name;

    // disabled assignment operator
    Resource& operator=(const Resource&);
    Resource(const Resource&);
};


template<typename T>
IMPLEMENT_OBJECT(Resource<T>)

} //end namespace

#endif
