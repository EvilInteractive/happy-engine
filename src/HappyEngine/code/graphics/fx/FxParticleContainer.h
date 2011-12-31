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

#ifndef _HE_FX_PARTICLE_CONTAINER_H_
#define _HE_FX_PARTICLE_CONTAINER_H_
#pragma once

#include <deque>
#include "boost/function.hpp"

namespace he {
namespace gfx {

struct FxParticle;
class FxParticleContainer
{
public:
    FxParticleContainer(uint maxParticles);
    virtual ~FxParticleContainer();

    bool tryAddParticle();
    void removeParticle(FxParticle* pParticle);
    void flushRemove();
    uint getNumParticles() const;

    void resize(uint maxParticles);

    FxParticle* back() const;

    void sort(boost::function<bool(const FxParticle& a, const FxParticle& b)> pred);

    void for_each(const boost::function<void(FxParticle*)>& func);

private:
    FxParticle* m_MemPool;
    FxParticle* m_End;

    std::vector<FxParticle*> m_RemoveVec;

    uint m_Count;
    uint m_MaxCount;

    //Disable default copy constructor and default assignment operator
    FxParticleContainer(const FxParticleContainer&);
    FxParticleContainer& operator=(const FxParticleContainer&);
};

} } //end namespace

#endif
