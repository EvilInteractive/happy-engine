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

#ifndef _HE_IGAME_H_
#define _HE_IGAME_H_
#pragma once

#include "ITickable.h"
#include <vector>
#include <algorithm>

namespace he {

namespace gfx {
    class Camera;
}

namespace game {

class Game
{
public:
    Game(): m_pActiveCamera(nullptr) {}
    virtual ~Game() {}

    virtual void init() = 0;
    virtual void load() = 0;
    virtual void draw() = 0;


    virtual void tick(float dTime)
    {
        std::for_each(m_TickList.cbegin(), m_TickList.cend(), [&dTime](ITickable* pObj)
        {
            pObj->tick(dTime);
        });
    }
    virtual void addToTickList(ITickable* pObj)
    {
        m_TickList.push_back(pObj);
    }

    virtual void setActiveCamera(gfx::Camera* pCamera) { m_pActiveCamera = pCamera; }
    virtual gfx::Camera* getActiveCamera() { return m_pActiveCamera; }

private:
    gfx::Camera* m_pActiveCamera;
    std::vector<ITickable*> m_TickList;
};

} } //end namespace

#endif
