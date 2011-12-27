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
//Created: 26/11/2011
#include "HappyPCH.h" 

#include "FxParticleSystem.h"
#include "HappyNew.h"

#include "FxParticleContainer.h"
#include "IFxParticleModifyComponent.h"
#include "IFxParticleInitComponent.h"
#include "FxParticle.h"

namespace he {
namespace gfx {

FxParticleSystem::FxParticleSystem(): m_UvTiles(1, 1)
{
}


FxParticleSystem::~FxParticleSystem()
{
}

void FxParticleSystem::start()
{

}

void FxParticleSystem::stop()
{

}

void FxParticleSystem::tick( float currentTime, float dTime )
{
    //////////////////////////////////////////////////////////////////////////
    // Emmit
    m_TimeSinceLastSpawn += dTime;
    float spawnRate(1.0f/m_SpawnRate->getValue(currentTime));
    while (m_TimeSinceLastSpawn > spawnRate)
    {
        m_TimeSinceLastSpawn -= spawnRate;
        if (m_pFxParticleContainer->tryAddParticle())
        {
            std::for_each(m_ParticleInitComponents.cbegin(), m_ParticleInitComponents.cend(), [&](IFxParticleInitComponent* pComponent)
            {
                pComponent->init(m_pFxParticleContainer->back());
            });
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Update
    const std::vector<IFxParticleModifyComponent*>& components(m_ParticleModifyComponents);
    m_pFxParticleContainer->for_each([&](FxParticle* pParticle)
    {
        pParticle->m_Life -= dTime;
        if (pParticle->m_Life <= 0.0f)
        {
            m_pFxParticleContainer->removeParticle(pParticle);
        }
        else
        {
            float& currentTime_(currentTime);
            float& dTime_(dTime);
            std::for_each(components.cbegin(), components.cend(), [&pParticle, &currentTime_, &dTime_](IFxParticleModifyComponent* pComponent)
            {
                pComponent->transform(pParticle, currentTime_, dTime_);
            });
        }
    });
    m_pFxParticleContainer->flushRemove();
    //////////////////////////////////////////////////////////////////////////
}

} } //end namespace