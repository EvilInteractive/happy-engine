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

#include "FxTimeLine.h"
#include "HappyNew.h"
#include "FxTimeLineTrack.h"

#include "boost/bind.hpp"

namespace he {
namespace gfx {

FxTimeLine::FxTimeLine(): m_IsRunning(false)
{
}


FxTimeLine::~FxTimeLine()
{
    he::for_each(m_Tracks.cbegin(), m_Tracks.cend(), [&](FxTimeLineTrack* pTrack)
    {
        delete pTrack;
    });
}

void FxTimeLine::start()
{
    StartEvent();
    m_IsRunning = true;
    m_CurrentTime = 0;
}
void FxTimeLine::stop()
{
    m_CurrentTime = m_EndTime + 1;
    EndEvent();
    m_IsRunning = false;
}
void FxTimeLine::setTime( float time )
{
    ASSERT(time >= 0, "time should be positive");
    m_CurrentTime = time;
}
void FxTimeLine::setEndTime( float endTime )
{
    ASSERT(endTime >= 0, "end time should be positive");
    m_EndTime = endTime;
}


void FxTimeLine::tick( float dTime )
{
    m_CurrentTime += dTime;
    if (m_CurrentTime > m_EndTime)
        stop();
    else
    {
        he::for_each(m_Tracks.cbegin(), m_Tracks.cend(), [&](FxTimeLineTrack* pTrack)
        {
            pTrack->tick(m_CurrentTime, dTime);
        });
    }
}

uint FxTimeLine::addTrack()
{
    FxTimeLineTrack* pTrack(NEW FxTimeLineTrack());
    StartEvent += boost::bind(&FxTimeLineTrack::start, pTrack);
    EndEvent += boost::bind(&FxTimeLineTrack::stop, pTrack);
    return m_Tracks.insert(pTrack);
}

FxTimeLineTrack* FxTimeLine::getTrack( uint id ) const
{
    return m_Tracks[id];
}

void FxTimeLine::removeTrack( uint id )
{
    delete m_Tracks.remove(id);
}

bool FxTimeLine::isRunning() const
{
    return m_IsRunning;
}


} } //end namespace