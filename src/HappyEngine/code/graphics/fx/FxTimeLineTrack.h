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

#ifndef _HE_FX_TIME_LINE_TRACK_H_
#define _HE_FX_TIME_LINE_TRACK_H_
#pragma once

#include <queue>
#include <vector>
#include "HappyTypes.h"
#include "SlotPContainer.h"
#include "IFxTimeLineTrackComponent.h"

namespace he {
namespace gfx {

class IFxTimeLineTrackComponent;
class FxTimeLine;
enum FxType;

class FxTimeLineTrack
{
public:
    FxTimeLineTrack(const FxTimeLine* pParent);
    virtual ~FxTimeLineTrack();

    void start();
    void stop();

    const FxTimeLine* getParent() const;

    uint addComponent(FxType type);
    void removeComponent(uint id);
    template<typename T> T* getComponent(uint id) const
    {
        T* pComp(dynamic_cast<T*>(m_Components[id]));
        ASSERT(pComp != nullptr, "pComp is not a T*");
        return pComp;
    }

    void tick(float currentTime, float dTime);

    event<void> EaseOutStart;
    event<void> EaseOutEnd;

private:

    const FxTimeLine* m_pParent;

    std::queue<uint> m_PlayQueue;
    std::vector<IFxTimeLineTrackComponent*> m_EaseOutComponents;
    std::vector<IFxTimeLineTrackComponent*> m_EaseOutDoneComponents;
    SlotPContainer<IFxTimeLineTrackComponent*> m_Components;

    //Disable default copy constructor and default assignment operator
    FxTimeLineTrack(const FxTimeLineTrack&);
    FxTimeLineTrack& operator=(const FxTimeLineTrack&);
};

} } //end namespace

#endif
