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
#include <vector>
#include <algorithm>

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
    virtual void load() = 0;


    virtual void tick(float dTime)
    {
        if (m_RemoveTickList.size() > 0)
        {
            std::vector<ITickable*>::iterator it(m_TickList.begin());
            for(; it != m_TickList.end() && m_RemoveTickList.size() > 0; )
            {
                std::vector<ITickable*>::iterator removeIt(std::find(m_RemoveTickList.begin(), m_RemoveTickList.end(), *it));
                if (removeIt != m_RemoveTickList.end())
                {
                    *it = m_TickList.back();
                    m_TickList.pop_back();
                    *removeIt = m_RemoveTickList.back();
                    m_RemoveTickList.pop_back();
                }
                else
                {
                    ++it;
                }
            }
            m_RemoveTickList.clear();
        }
        if (m_NewTickList.size() > 0)
        {
            std::for_each(m_NewTickList.cbegin(), m_NewTickList.cend(), [&](ITickable* pObj)
            {
                m_TickList.push_back(pObj);
            });
            m_NewTickList.clear();
        }
        std::for_each(m_TickList.cbegin(), m_TickList.cend(), [&dTime](ITickable* pObj)
        {
            pObj->tick(dTime);
        });
    }
    virtual void addToTickList(ITickable* pObj)
    {
        m_NewTickList.push_back(pObj);
    }
    virtual void removeFromTickList(ITickable* pObj)
    {
        m_RemoveTickList.push_back(pObj);
    }
    
private:
    std::vector<ITickable*> m_TickList;
    std::vector<ITickable*> m_NewTickList;
    std::vector<ITickable*> m_RemoveTickList;
};

} } //end namespace

#endif
