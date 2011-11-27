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
//Created: 27/11/2011

#ifndef _HE_FX_MANAGER_H_
#define _HE_FX_MANAGER_H_
#pragma once

#include "HappyTypes.h"

#include <set>
#include <queue>

namespace he {
namespace gfx {

class FxTimeLine;

class FxManager
{
public:
    FxManager();
    virtual ~FxManager();

    uint createTimeline();
    void removeTimeline(uint id);
    FxTimeLine* getTimeline(uint id) const;
    void startTimeline(uint id) const;

    void tick(float dTime);

private:

    void timelineStarted(uint id);
    void timelineStopped(uint id);

    const static int MAX_TIMELINES = 100;
    FxTimeLine* m_TimelineMemPool[MAX_TIMELINES];

    std::queue<uint> m_FreeSlots;
    std::set<uint> m_ActiveTimelines;

    //Disable default copy constructor and default assignment operator
    FxManager(const FxManager&);
    FxManager& operator=(const FxManager&);
};

} } //end namespace

#endif
