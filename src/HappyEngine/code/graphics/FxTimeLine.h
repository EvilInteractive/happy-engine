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

#ifndef _HE_FX_TIMELINE_H_
#define _HE_FX_TIMELINE_H_
#pragma once

#include "event.h"
#include <vector>

#include "HappyTypes.h"

namespace he {
namespace gfx {

class FxTimeLineTrack;

class FxTimeLine
{
public:
    FxTimeLine();
    virtual ~FxTimeLine();

    void start();

    uint addTrack();
    void removeTrack(uint id);

    void tick(float dTime);
    
    event<void> EndEvent;
    event<void> StartEvent;

private:

    std::vector<FxTimeLineTrack*> m_Tracks;

    float m_CurrentTime;
    float m_EndTime;

    //Disable default copy constructor and default assignment operator
    FxTimeLine(const FxTimeLine&);
    FxTimeLine& operator=(const FxTimeLine&);
};

} } //end namespace

#endif
