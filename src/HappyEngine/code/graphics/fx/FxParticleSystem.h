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

#ifndef _HE_FX_PARTICLE_SYSTEM_H_
#define _HE_FX_PARTICLE_SYSTEM_H_
#pragma once

#include "IFxComponent.h"
#include "SlotPContainer.h"

#include "Texture2D.h"
#include "IFxVariable.h"

#include "ShaderVar.h"

namespace he {
namespace gfx {

class FxParticleContainer;
class IFxParticleInitComponent;
class IFxParticleModifyComponent;

class InstancingController;

namespace details {
class InstancingBuffer;
}

enum ParticleInitComponentType
{
    PICT_Color,
    PICT_Rotation,
    PICT_Scale,
    PICT_Speed
};
enum ParticleModifyComponentType
{
    PMCT_Color,
    PMCT_Force,
    PMCT_Rotation,
    PMCT_Scale,
    PMCT_Speed
};

class FxParticleSystem : public IFxComponent
{
public:
    FxParticleSystem();
    virtual ~FxParticleSystem();

    //////////////////////////////////////////////////////////////////////////
    ///                             IFxComponent                           ///
    //////////////////////////////////////////////////////////////////////////
    virtual FxType getType() const { return FxType_ParticleSystem; };
    
    virtual void start();
    virtual void stop();

    void setTexture(const Texture2D::pointer& tex2D);
    void setTiles(const IFxVariable<vec2>::pointer& tiles);
    void setSpawnRate(const IFxVariable<float>::pointer& rate);

    uint addInitComponent(ParticleInitComponentType type);  
    template <typename T>
    T* getInitComponent(uint id)
    {
        return dynamic_cast<T*>(m_ParticleInitComponents.get(id));
    }

    uint addModifyComponent(ParticleModifyComponentType type);
    template <typename T>
    T* getModifyComponent(uint id)
    {
        return dynamic_cast<T*>(m_ParticleModifyComponents.get(id));
    }

    virtual void tick(float currentTime, float dTime);

private:

    void instancingUpdater(details::InstancingBuffer& buffer);

    bool m_Emit;

    Texture2D::pointer m_pTexture;

    IFxVariable<vec2>::pointer m_UvTiles;
    ShaderUserVar<vec2>::pointer m_ShaderUvTiles;

    IFxVariable<float>::pointer m_SpawnRate;
    float m_TimeSinceLastSpawn;

    SlotPContainer<IFxParticleInitComponent*> m_ParticleInitComponents;
    SlotPContainer<IFxParticleModifyComponent*> m_ParticleModifyComponents;

    FxParticleContainer* m_pFxParticleContainer;

    InstancingController* m_pInstancingController;

    //Disable default copy constructor and default assignment operator
    FxParticleSystem(const FxParticleSystem&);
    FxParticleSystem& operator=(const FxParticleSystem&);
};

} } //end namespace

#endif
