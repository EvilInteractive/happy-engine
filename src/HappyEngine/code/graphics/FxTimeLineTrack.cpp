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

#include "FxTimeLineTrack.h"
#include "HappyNew.h"

#include "IFxComponent.h"

#include <algorithm>

namespace he {
namespace gfx {

FxTimeLineTrack::FxTimeLineTrack()
{
}


FxTimeLineTrack::~FxTimeLineTrack()
{
}

void FxTimeLineTrack::tick( float currentTime, float dTime )
{
    if (m_Components[m_PlayQueue.front()]->getEndTime() > currentTime)
        m_PlayQueue.pop();
    if (m_Components[m_PlayQueue.front()]->getStartTime() >= currentTime)
    {
        m_Components[m_PlayQueue.front()]->tick(currentTime, dTime);
    }
}

bool timeSort(const IFxComponent* pComponent1, const IFxComponent* pComponent2)
{
    return pComponent1->getStartTime() < pComponent2->getStartTime();
}
void FxTimeLineTrack::start()
{
    std::vector<IFxComponent*> tempComponents;
    he::for_each(m_Components.cbegin(), m_Components.cend(), [&](IFxComponent* pComp)
    {
        tempComponents.push_back(pComp);
    });
    std::sort(tempComponents.begin(), tempComponents.end(), timeSort);

    m_PlayQueue = std::queue<uint>();
    for (uint i(0); i < tempComponents.size(); ++i)
    {
        m_PlayQueue.push(m_Components.getId(tempComponents[i]));
    }
}

} } //end namespace