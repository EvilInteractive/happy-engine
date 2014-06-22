//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

    virtual void tick(float dTime)
    {
        HIERARCHICAL_PROFILE(__HE_FUNCTION__);
        for (size_t i(0); i < m_TickList.size(); ++i)
        {
            m_TickList[i]->tick(dTime);
        };
    }
    virtual void addToTickList(ITickable* obj)
    {
        m_TickList.add(obj);
    }
    virtual void removeFromTickList(ITickable* obj)
    {
        m_TickList.remove(obj);
    }
    
private:
    he::PrimitiveList<ITickable*> m_TickList;
};

} } //end namespace

#endif
