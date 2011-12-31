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
//Created: 27/12/2011
#include "HappyPCH.h"

#include "FxParticleContainer.h"
#include "HappyNew.h"

#include "FxParticle.h"

namespace he {
namespace gfx {

FxParticleContainer::FxParticleContainer(uint maxParticles): m_MaxCount(maxParticles), m_Count(0)
{
    #ifdef GCC
    void* p;
    posix_memalign(&p, 16, maxParticles * sizeof(FxParticle));
    m_MemPool = (FxParticle*)p;
    #else
    m_MemPool = static_cast<FxParticle*>(_aligned_malloc(maxParticles * sizeof(FxParticle), 16));
    #endif

    ASSERT(m_MemPool != nullptr, "particle container: out of memory");
    memset(m_MemPool, 0, maxParticles * sizeof(FxParticle));
    m_End = m_MemPool;
}

FxParticleContainer::~FxParticleContainer()
{
    #ifdef GCC
    free(m_MemPool);
    #else
    _aligned_free(m_MemPool);
    #endif
}
void FxParticleContainer::resize(uint maxParticles)
{
    #ifdef GCC
    void* p;
    posix_memalign(&p, 16, maxParticles * sizeof(FxParticle));
    m_MemPool = (FxParticle*)p;
    #else
    m_MemPool = static_cast<FxParticle*>(_aligned_malloc(maxParticles * sizeof(FxParticle), 16));
    #endif

    ASSERT(m_MemPool != nullptr, "particle container: out of memory");
    if (maxParticles < m_Count)
        m_Count = maxParticles;
    m_End = m_MemPool + m_Count;
    m_MaxCount = maxParticles;
}


bool FxParticleContainer::tryAddParticle()
{
    if (m_Count < m_MaxCount)
    {
        memset(m_End, 0, sizeof(FxParticle));
        m_End += 1;
        ++m_Count;
        return true;
    }
    return false;
}
void FxParticleContainer::removeParticle( FxParticle* pParticle )
{
    m_RemoveVec.push_back(pParticle);
}
void FxParticleContainer::flushRemove()
{
    if (m_RemoveVec.size() > 0)
    {
        // First remove mem at the end -> continue to front
        std::sort(m_RemoveVec.begin(), m_RemoveVec.end(), [](FxParticle* a, FxParticle* b)
        {
            return a > b;
        });

        std::for_each(m_RemoveVec.cbegin(), m_RemoveVec.cend(), [&](FxParticle* p)
        {
            memmove(p, --m_End, sizeof(FxParticle));
        });
        m_Count -= m_RemoveVec.size();
        m_RemoveVec.clear();
    }
}

void FxParticleContainer::for_each( const boost::function<void(FxParticle*)>& func )
{
    FxParticle* it(m_MemPool);
    for (; it != m_End; ++it)
    {
        func(it);
    }
}

uint FxParticleContainer::getNumParticles() const
{
    return m_Count;
}

FxParticle* FxParticleContainer::back() const
{
    return m_End - 1;
}

void FxParticleContainer::sort( boost::function<bool(const FxParticle& a, const FxParticle& b)> pred )
{
    std::sort(m_MemPool, m_End, pred);
}

} } //end namespace
