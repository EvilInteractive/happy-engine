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

#ifndef _HE_IGAME_H_
#define _HE_IGAME_H_
#pragma once

#include "ITickable.h"

namespace he {

namespace gfx {
    class CameraPerspective;
}

namespace ge {

class Game
{
public:
    Game() {}
    virtual ~Game() {}

    virtual void init() = 0;
    virtual void destroy() = 0;

    virtual void load() = 0;

    virtual void tick(float dTime)
    {
        if (m_NewTickList.size() > 0)
        {
            m_TickList.append(m_NewTickList);
            m_NewTickList.clear();
        }
        if (m_RemoveTickList.size() > 0)
        {
            m_RemoveTickList.forEach([this](ITickable* const obj)
            {
                m_TickList.remove(obj);
            });
            m_RemoveTickList.clear();
        }
        m_TickList.forEach([&dTime](ITickable* pObj)
        {
            pObj->tick(dTime);
        });
    }
    virtual void addToTickList(ITickable* pObj)
    {
        m_NewTickList.add(pObj);
    }
    virtual void removeFromTickList(ITickable* pObj)
    {
        m_RemoveTickList.add(pObj);
    }
    
private:
    he::PrimitiveList<ITickable*> m_TickList;
    he::PrimitiveList<ITickable*> m_NewTickList;
    he::PrimitiveList<ITickable*> m_RemoveTickList;
};

} } //end namespace

#endif
