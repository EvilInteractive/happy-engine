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
#include "StdAfx.h" 

#include "FxManager.h"
#include "HappyNew.h"

#include "FxTimeLine.h"

#include "boost/bind.hpp"

namespace he {
namespace gfx {

FxManager::FxManager()
{
    memset(m_TimelineMemPool, 0, sizeof(FxTimeLine*) * MAX_TIMELINES);
    for (int i(0); i < MAX_TIMELINES; ++i)
    {
        m_FreeSlots.push(i);
    }
}


FxManager::~FxManager()
{
    for (int i(0); i < MAX_TIMELINES; ++i)
    {
        delete m_TimelineMemPool[i];
    }
}

void FxManager::tick( float dTime )
{
    std::set<uint> activeTimelines = m_ActiveTimelines; //set could change make copy
    std::for_each(activeTimelines.cbegin(), activeTimelines.cend(), [&](uint id)
    {
        m_TimelineMemPool[id]->tick(dTime);
    });
}

uint FxManager::createTimeline()
{
    ASSERT(m_FreeSlots.size() > 0, "Fx needs bigger mempool or something went wrong");
    uint id(m_FreeSlots.front());
    m_FreeSlots.pop();

    m_TimelineMemPool[id] = NEW FxTimeLine();
    m_TimelineMemPool[id]->StartEvent += boost::bind(&FxManager::timelineStarted, this, id);
    m_TimelineMemPool[id]->EndEvent += boost::bind(&FxManager::timelineStopped, this, id);

    return id;
}

void FxManager::removeTimeline( uint id )
{
    ASSERT(id >= 0 && id < MAX_TIMELINES, "id is not in a valid range");
    ASSERT(m_TimelineMemPool[id] != nullptr, "No effect exists @id");

    delete m_TimelineMemPool[id];
    m_TimelineMemPool[id] = nullptr;

    m_FreeSlots.push(id);
}

FxTimeLine* FxManager::getTimeline( uint id ) const
{
    ASSERT(id >= 0 && id < MAX_TIMELINES, "id is not in a valid range");
    ASSERT(m_TimelineMemPool[id] != nullptr, "No effect exists @id");
    return m_TimelineMemPool[id];

    boost::function<void(int)> test;
    test(0);
}

void FxManager::timelineStarted( uint id )
{
    ASSERT(m_ActiveTimelines.find(id) == m_ActiveTimelines.cend(), "Timeline already active!");
    m_ActiveTimelines.insert(id);
}

void FxManager::timelineStopped( uint id )
{
    ASSERT(m_ActiveTimelines.find(id) != m_ActiveTimelines.cend(), "Timeline already stopped!");
    m_ActiveTimelines.erase(id);
}

} } //end namespace