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

#include "FxTimeLine.h"
#include "HappyNew.h"
#include "FxTimeLineTrack.h"

#include "boost/bind.hpp"

namespace he {
namespace gfx {

FxTimeLine::FxTimeLine(): m_IsRunning(false), m_pParent(nullptr)
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
    m_EaseOutTracks.clear();
    m_EaseOutDoneTracks.clear();
    m_IsRunning = true;
    m_CurrentTime = 0;
}
void FxTimeLine::stop()
{
    m_CurrentTime = m_EndTime + 1;
    StopEvent();
    if (m_EaseOutTracks.size() == 0)
        EndEvent();
    m_IsRunning = false;
}
void FxTimeLine::setTime( float time )
{
    HE_ASSERT(time >= 0, "time should be positive");
    m_CurrentTime = time;
}
void FxTimeLine::setEndTime( float endTime )
{
    HE_ASSERT(endTime >= 0, "end time should be positive");
    m_EndTime = endTime;
}


void FxTimeLine::tick( float dTime )
{
    m_CurrentTime += dTime;
    if (m_IsRunning)
    {
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

    if (m_EaseOutTracks.size() > 0)
    {
        std::for_each(m_EaseOutTracks.cbegin(), m_EaseOutTracks.cend(), [&](FxTimeLineTrack* pTrack)
        {
            pTrack->tick(m_CurrentTime, dTime);
        });
        if (m_EaseOutDoneTracks.size() > 0)
        {
            std::for_each(m_EaseOutDoneTracks.cbegin(), m_EaseOutDoneTracks.cend(), [&](FxTimeLineTrack* pTrack)
            {
                m_EaseOutTracks.erase(std::remove(m_EaseOutTracks.begin(), m_EaseOutTracks.end(), pTrack), m_EaseOutTracks.end());
            });
            m_EaseOutDoneTracks.clear();
        }
        if (m_EaseOutTracks.size() == 0)
        {
            EndEvent();
        }
    }
}

uint FxTimeLine::addTrack()
{
    FxTimeLineTrack* pTrack(NEW FxTimeLineTrack(this));
    StartEvent += boost::bind(&FxTimeLineTrack::start, pTrack);
    StopEvent += boost::bind(&FxTimeLineTrack::stop, pTrack);

    std::vector<FxTimeLineTrack*>& easeOutTracks(m_EaseOutTracks);
    pTrack->EaseOutStart += [pTrack, &easeOutTracks](){ easeOutTracks.push_back(pTrack); };

    std::vector<FxTimeLineTrack*>& easeOutDoneTracks(m_EaseOutDoneTracks);
    pTrack->EaseOutEnd += [pTrack, &easeOutDoneTracks](){ easeOutDoneTracks.push_back(pTrack); };


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

void FxTimeLine::setParent( const I3DObject* pParent )
{
    m_pParent = pParent;
}
const I3DObject* FxTimeLine::getParent() const
{
    return m_pParent;
}


} } //end namespace