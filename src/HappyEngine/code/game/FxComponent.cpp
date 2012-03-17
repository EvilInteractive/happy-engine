//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Created: 29/10/2011
#include "HappyPCH.h" 

#include "FxComponent.h"
#include "HappyNew.h"
#include "Entity.h"
#include "HappyEngine.h"

#include "fx/FxManager.h"
#include "fx/FxTimeLine.h"

namespace he {
namespace game {

FxComponent::FxComponent(): m_TimelineId(UINT_MAX)
{
}


FxComponent::~FxComponent()
{
}

void FxComponent::init( Entity* pParent )
{
    m_pParent = pParent;
}

void FxComponent::serialize(SerializerStream& stream)
{
    stream << m_mtxLocalTransform;
}

void FxComponent::deserialize(const SerializerStream& stream)
{
    stream >> m_mtxLocalTransform;
}


mat44 FxComponent::getWorldMatrix() const
{
    return m_pParent->getWorldMatrix() * m_mtxLocalTransform;
}

void FxComponent::setLocalTransform( const mat44& mtxWorld )
{
    m_mtxLocalTransform = mtxWorld;
}

const mat44& FxComponent::getLocalTransform() const
{
    return m_mtxLocalTransform;
}

void FxComponent::setTimeline( uint id )
{
    m_TimelineId = id;
    FX->getTimeline(m_TimelineId)->setParent(this);
}
uint FxComponent::getTimeline() const
{
    return m_TimelineId;
}


void FxComponent::start()
{
    HE_ASSERT(m_TimelineId != UINT_MAX, "Set Time line first!");
    FX->getTimeline(m_TimelineId)->start();
}
void FxComponent::stop()
{
    HE_ASSERT(m_TimelineId != UINT_MAX, "Set Time line first!");
    FX->getTimeline(m_TimelineId)->stop();
}
void FxComponent::isPlaying() const
{
    HE_ASSERT(m_TimelineId != UINT_MAX, "Set Time line first!");
    FX->getTimeline(m_TimelineId)->isRunning();
}

} } //end namespace