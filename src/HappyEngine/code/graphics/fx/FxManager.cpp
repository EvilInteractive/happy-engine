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
#include "HappyPCH.h" 

#include "FxManager.h"
#include "HappyNew.h"

#include "FxTimeLine.h"

#include "boost/bind.hpp"

namespace he {
namespace gfx {

FxManager::FxManager()
{
}


FxManager::~FxManager()
{
    he::for_each(m_TimeLines.cbegin(), m_TimeLines.cend(), [&](FxTimeLine* pTimeLine)
    {
        delete pTimeLine;
    });
}

void FxManager::tick( float dTime )
{
    std::set<uint> activeTimelines = m_ActiveTimelines; //set could change, because of end of time line -> make copy
    std::for_each(activeTimelines.cbegin(), activeTimelines.cend(), [&](uint id)
    {
        m_TimeLines[id]->tick(dTime);
    });
}

uint FxManager::createTimeline()
{
    uint id(m_TimeLines.insert(NEW FxTimeLine()));

    m_TimeLines[id]->StartEvent += boost::bind(&FxManager::timelineStarted, this, id);
    m_TimeLines[id]->EndEvent += boost::bind(&FxManager::timelineStopped, this, id);

    return id;
}

void FxManager::removeTimeline( uint id )
{
    delete m_TimeLines.remove(id);
}

FxTimeLine* FxManager::getTimeline( uint id ) const
{
    return m_TimeLines[id];
}

void FxManager::timelineStarted( uint id )
{
    if (m_ActiveTimelines.find(id) == m_ActiveTimelines.cend()) //Timeline not active?
        m_ActiveTimelines.insert(id);
}

void FxManager::timelineStopped( uint id )
{
    ASSERT(m_ActiveTimelines.find(id) != m_ActiveTimelines.cend(), "Timeline already stopped!");
    m_ActiveTimelines.erase(id);
}

} } //end namespace