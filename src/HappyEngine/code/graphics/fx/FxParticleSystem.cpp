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

#include "GraphicsEngine.h"
#include "ContentManager.h"
#include "InstancingManager.h"
#include "InstancingController.h"
#include "DynamicBuffer.h"

#include "FxConstant.h"

#include "FxParticleColor.h"
#include "FxParticleForce.h"
#include "FxParticleRotation.h"
#include "FxParticleRotationRate.h"
#include "FxParticleScale.h"
#include "FxParticleSpeed.h"
#include "FxParticleLocation.h"
#include "FxParticleLife.h"
#include "CameraManager.h"
#include "ICamera.h"
#include "Camera.h"

#include "FxTimeLine.h"
#include "FxTimeLineTrack.h"
#include "I3DObject.h"

#include "boost/weak_ptr.hpp"

namespace he {
namespace gfx {

FxParticleSystem::FxParticleSystem(const FxTimeLineTrack* pParent): 
                                      m_Emit(false), m_Stopped(false), m_UvTiles(NEW FxConstant<vec2>(vec2(1, 1))),
                                      m_SpawnRate(NEW FxConstant<float>(20)),
                                      m_pFxParticleContainer(NEW FxParticleContainer(512)),
                                      m_TimeSinceLastSpawn(0), m_pParent(pParent), m_pInstancingController(nullptr)

{
    EaseOutEnd += [&]()
    { 
        if (m_pInstancingController != nullptr)
        {
            m_pInstancingController->removeManualFiller(this); 
            m_pInstancingController = nullptr; 
        }
    };
}


FxParticleSystem::~FxParticleSystem()
{
    he::for_each(m_ParticleInitComponents.cbegin(), m_ParticleInitComponents.cend(), [](IFxParticleInitComponent* pComp)
    {
        delete pComp;
    });
    he::for_each(m_ParticleModifyComponents.cbegin(), m_ParticleModifyComponents.cend(), [](IFxParticleModifyComponent* pComp)
    {
        delete pComp;
    });
    delete m_pFxParticleContainer;
}

void FxParticleSystem::start()
{
    m_Stopped = false;
    ShaderUserVar<Texture2D*>::pointer pTexture(boost::static_pointer_cast<ShaderUserVar<Texture2D*>>(m_Material.getVar("diffuseMap")));
    if (pTexture != nullptr)
    {
        boost::weak_ptr<ShaderUserVar<Texture2D*>> weakTex(pTexture);
        pTexture->getData()->callbackIfLoaded([&, weakTex]()
        {
            if (m_Stopped == false)
            {
                ShaderUserVar<Texture2D*>::pointer sharedTex(weakTex);
                std::stringstream stream;
                stream << "part_" << sharedTex->getData()->getID() << "_" << m_UvTiles->getValue(0).x << "," << m_UvTiles->getValue(0).y;
                m_ShaderUvTiles = boost::static_pointer_cast<ShaderUserVar<vec2>>(m_Material.getVar("uvTiles"));
                m_ShaderUvTiles->setData(m_UvTiles->getValue(0));
                if (m_pInstancingController == nullptr)
                {
                    if (GRAPHICS->getInstancingManager()->getController(stream.str()) == nullptr)
                    {
                        ModelMesh* quad(CONTENT->getParticleQuad());
                        GRAPHICS->getInstancingManager()->createController(stream.str(), true, quad->getHandle(), m_Material);
                        quad->release();
                        m_pInstancingController = GRAPHICS->getInstancingManager()->getController(stream.str());

                        m_pInstancingController->addManualFiller(this);
                        m_pInstancingController->setCastsShadow(false);
                    }
                    else
                    {
                        m_pInstancingController = GRAPHICS->getInstancingManager()->getController(stream.str());
                        m_pInstancingController->addManualFiller(this);
                    }
                }
                m_Emit = true;
            }
        });
    }
    else
    {
        HE_ERROR("Starting particlesystem without texture");
    }
}
void FxParticleSystem::fillInstancingBuffer( details::InstancingBuffer& buffer )
{
    DynamicBuffer dbuffer(m_pInstancingController->getMaterial().getCompatibleInstancingLayout());
    ICamera* pCam(CAMERAMANAGER->getActiveCamera());
    m_pFxParticleContainer->for_each([&dbuffer,&buffer,&pCam](FxParticle* pParticle)
    {
        dbuffer.setBuffer(buffer.addItem());
        dbuffer.setValue(0, pParticle->m_BlendColor);
        dbuffer.setValue(1, pParticle->m_UvTile);
        dbuffer.setValue(2, pParticle->getWorld(pCam));
    });
}


void FxParticleSystem::stop()
{
    m_Stopped = true;
    m_Emit = false;
    EaseOutStart();
}

void FxParticleSystem::tick( float normTime, float dTime )
{
    //////////////////////////////////////////////////////////////////////////
    // Emit
    if (m_Emit)
    {
        m_TimeSinceLastSpawn += dTime;
        float spawnRate(1.0f/m_SpawnRate->getValue(normTime));
        while (m_TimeSinceLastSpawn > spawnRate)
        {
            m_TimeSinceLastSpawn -= spawnRate;
            if (m_pFxParticleContainer->tryAddParticle())
            {
                m_pFxParticleContainer->back()->setToDefault();

                mat44 parentWorld;
                if (m_pParent->getParent()->getParent() != nullptr)
                    parentWorld = m_pParent->getParent()->getParent()->getWorldMatrix();

                he::for_each(m_ParticleInitComponents.cbegin(), m_ParticleInitComponents.cend(), [&](IFxParticleInitComponent* pComponent)
                {
                    pComponent->init(m_pFxParticleContainer->back(), parentWorld);
                });
                m_pFxParticleContainer->back()->m_Id = static_cast<uint16>(m_pInstancingController->addInstance());
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Update
    auto& components(m_ParticleModifyComponents);
    m_pFxParticleContainer->for_each([&](FxParticle* pParticle)
    {
        pParticle->m_LifeTime += dTime;
        if (pParticle->m_LifeTime >= pParticle->m_MaxLifeTime)
        {
            m_pInstancingController->removeInstance(pParticle->m_Id);
            m_pFxParticleContainer->removeParticle(pParticle);
        }
        else
        {
            float particleTime(pParticle->m_LifeTime / pParticle->m_MaxLifeTime);
            float& dTime_(dTime);

            mat44 parentWorld;
            if (m_pParent->getParent()->getParent() != nullptr)
                parentWorld = m_pParent->getParent()->getParent()->getWorldMatrix();
            he::for_each(components.cbegin(), components.cend(), [&pParticle, &particleTime, &dTime_, &parentWorld](IFxParticleModifyComponent* pComponent)
            {
                pComponent->transform(pParticle, particleTime, dTime_, parentWorld);
            });
            pParticle->m_Position += pParticle->m_Velocity * dTime;
        }
    });
    m_pFxParticleContainer->flushRemove();
    vec3 camPos(CAMERAMANAGER->getActiveCamera()->getPosition());
    m_pFxParticleContainer->sort([&camPos](const FxParticle& a, const FxParticle& b)
    {
        // Strange lines ahead:
        // std asserts because of float imprecision, this is however no problem in release
        #if DEBUG | _DEBUG
        return static_cast<uint>(lengthSqr(camPos - b.m_Position) * 100) < static_cast<uint>(lengthSqr(camPos - a.m_Position) * 100);
        #else
        return lengthSqr(camPos - b.m_Position) < lengthSqr(camPos - a.m_Position);
        #endif
    });
    //////////////////////////////////////////////////////////////////////////

    if (m_Stopped && m_pFxParticleContainer->getNumParticles() == 0)
    {
        EaseOutEnd();
    }
}

void FxParticleSystem::setMaterial( const Material& mat )
{
    m_Material = mat;
    m_Material.setDepthWriteEnabled(false);
}
void FxParticleSystem::setTiles( const IFxVariable<vec2>::pointer& tiles )
{
    m_UvTiles = tiles;
}
void FxParticleSystem::setSpawnRate( const IFxVariable<float>::pointer& rate )
{
    m_SpawnRate = rate;
}
void FxParticleSystem::setMaxParticles( uint max )
{
    m_pFxParticleContainer->resize(max);
}

uint FxParticleSystem::addInitComponent( ParticleInitComponentType type )
{
    switch (type)
    {
        case PICT_Color:
            return m_ParticleInitComponents.insert(NEW FxParticleColor());
        case PICT_Rotation:
            return m_ParticleInitComponents.insert(NEW FxParticleRotation());
        case PICT_Scale:
            return m_ParticleInitComponents.insert(NEW FxParticleScale());
        case PICT_Speed:
            return m_ParticleInitComponents.insert(NEW FxParticleSpeed());
        case PICT_Location:
            return m_ParticleInitComponents.insert(NEW FxParticleLocation());
        case PICT_Life:
            return m_ParticleInitComponents.insert(NEW FxParticleLife());
        default:
            HE_ASSERT(false, "Unkown PICT type"); return 0;
    }
}

uint FxParticleSystem::addModifyComponent( ParticleModifyComponentType type )
{
    switch (type)
    {
        case PMCT_Color:
            return m_ParticleModifyComponents.insert(NEW FxParticleColor());
        case PMCT_Force:
            return m_ParticleModifyComponents.insert(NEW FxParticleForce());
        case PMCT_Rotation:
            return m_ParticleModifyComponents.insert(NEW FxParticleRotation());
        case PMCT_RotationRate:
            return m_ParticleModifyComponents.insert(NEW FxParticleRotationRate());
        case PMCT_Scale:
            return m_ParticleModifyComponents.insert(NEW FxParticleScale());
        case PMCT_Speed:
            return m_ParticleModifyComponents.insert(NEW FxParticleSpeed());
        default:
            HE_ASSERT(false, "Unkown PMCT type"); return 0;
    }
}


} } //end namespace