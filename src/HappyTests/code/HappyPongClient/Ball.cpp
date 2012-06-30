//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 30/03/2012
#include "HappyPongClientPCH.h" 

#include "Ball.h"

#include "ModelComponent.h"
#include "ContentManager.h"
#include "Game.h"
#include "LightComponent.h"
#include "LightFlashComponent.h"
#include "MainGame.h"
#include "Palet.h"
#include "Obstacle.h"
#include "ModelMesh.h"
#include "NetworkSerializer.h"

namespace hpc {

Ball::Ball(): 
    m_Position(0, 0, 0), 
    m_Velocity(0, 0, 0), 
    m_MainGame(static_cast<MainGame*>(GAME)),
    m_Radius(1.0f),
    m_Restitution(1.01f)
{
    reset();

    he::ge::ModelComponent* model(NEW he::ge::ModelComponent());
    he::ObjectHandle ballMaterial(CONTENT->loadMaterial("pong/ball.material"));
    model->setMaterial(ballMaterial);
    he::gfx::ModelMesh* mesh(CONTENT->asyncLoadModelMesh("pong/ball.binobj", "M_Bal", model->getMaterial()->getCompatibleVertexLayout()));
    model->setModelMesh(mesh->getHandle());
    mesh->release();
    he::ResourceFactory<he::gfx::Material>::getInstance()->release(ballMaterial);
    model->setLocalTransform(he::mat44::createScale(100));
    addComponent(model);

    m_LightFlashComponent = NEW LightFlashComponent();
    addComponent(m_LightFlashComponent);
    m_LightFlashComponent->setAttenuation(1.0f, 50);
    m_LightFlashComponent->setMultiplier(0.5f);
    m_LightFlashComponent->setColor(he::Color(1.0f, 1.0f, 0.5f));
    m_LightFlashComponent->setOffset(he::vec3(0.0f, 1.5f, 0));
    m_LightFlashComponent->setFlashMultiplier(10);
    m_LightFlashComponent->setNormalMultiplier(0);
    m_LightFlashComponent->setFlashDuration(0.25f);

    he::ge::PointLightComponent* redLight(NEW he::ge::PointLightComponent());
    addComponent(redLight);
    redLight->setAttenuation(1.0f, 50);
    redLight->setMultiplier(2.0f);
    redLight->setColor(he::Color(1.0f, 0.1f, 0.1f));
    redLight->setOffset(he::vec3(0.0f, 1.5f, 0));

    GAME->addToTickList(this);
}

void Ball::reset()
{
    m_Position = he::vec3(0, 0, 0);
    m_Velocity = he::vec3((m_Random.nextInt(0, 2) % 2 == 0? 1 : -1) * 30.0f, 0, (m_Random.nextInt(0, 2) % 2 == 0? 1 : -1) * 30.0f);
    m_Dead = false;
}



Ball::~Ball()
{
    GAME->removeFromTickList(this);
}

void Ball::tick( float dTime )
{
    /*m_MoveTo += m_Velocity * dTime;
    he::vec3 normal(0, 0, 0);
    bool reflect(false);
    
    if (m_MoveTo.z - m_Radius < -m_MainGame->getBoardDimension().y / 2)
    {
        m_MoveTo.z = -m_MainGame->getBoardDimension().y / 2 + m_Radius;
        normal += he::vec3(0, 0, 1);
        reflect = true;
    }
    else if (m_MoveTo.z + m_Radius > m_MainGame->getBoardDimension().y / 2)
    {
        m_MoveTo.z = m_MainGame->getBoardDimension().y / 2 - m_Radius;
        normal += he::vec3(0, 0, -1);
        reflect = true;
    }

    if (m_Dead == false)
    {
        if (m_MoveTo.x - m_Radius < -m_MainGame->getBoardDimension().x / 2)
        {
            m_MainGame->addPoint(0);
            m_MainGame->restart(true);
            m_Dead = true;
        }
        else if (m_MoveTo.x + m_Radius > m_MainGame->getBoardDimension().x / 2)
        {
            m_MainGame->addPoint(1);
            m_MainGame->restart(true);
            m_Dead = true;
        }
    }


    std::for_each(m_MainGame->getPalets().cbegin(), m_MainGame->getPalets().cend(), [&](Palet* palet)
    {
        if (m_MoveTo.z + m_Radius > palet->getPosition().z - palet->getDimension().y / 2 &&
            m_MoveTo.z - m_Radius < palet->getPosition().z + palet->getDimension().y / 2)
        {
            if (m_MoveTo.x + m_Radius > palet->getPosition().x - palet->getDimension().x / 2 && 
                m_MoveTo.x - m_Radius < palet->getPosition().x + palet->getDimension().x / 2)
            {
                if (m_Velocity.x < 0)
                {
                    m_MoveTo.x = palet->getPosition().x + palet->getDimension().x / 2 + m_Radius;
                    normal += he::vec3(1, 0, 0);
                }
                else
                {
                    m_MoveTo.x = palet->getPosition().x - palet->getDimension().x / 2 - m_Radius;
                    normal += he::vec3(-1, 0, 0);
                }
                reflect = true;
                palet->hitByBall();
            }
        }
    });

    std::for_each(m_MainGame->getObstacles().cbegin(), m_MainGame->getObstacles().cend(), [&](const Obstacle* obstacle)
    {
        if (he::lengthSqr(m_MoveTo - obstacle->getPosition()) < he::sqr(m_Radius + obstacle->getRadius()))
        {
            reflect = true;
            normal += he::normalize(m_MoveTo - obstacle->getPosition());
            m_MoveTo = obstacle->getPosition() + normal * (m_Radius + obstacle->getRadius());
        }
    });


    if (reflect)
    {
        m_Velocity = he::reflect(-m_Velocity, he::normalize(normal)) * m_Restitution;
        m_LightFlashComponent->flash();
        }*/
    //float speed(length(m_Velocity));
    m_MoveTo += m_Velocity * dTime;
    m_Position = m_MoveTo;
    setWorldMatrix(he::mat44::createTranslation(m_Position));
}

const he::vec3& Ball::getPosition() const
{
    return m_Position;
}

const he::vec3& Ball::getVelocity() const
{
    return m_Velocity;
}

float Ball::getRadius() const
{
    return m_Radius;
}

void Ball::serializeCreate( he::NetworkStream* /*stream*/ ) const
{
    HE_ASSERT(false, "will never happen");
}

bool Ball::deserializeCreate( he::NetworkStream* stream )
{
    stream->Read(m_MoveTo);
    stream->Read(m_Velocity);
    m_Position = m_MoveTo;
    return Entity::deserializeCreate(stream);
}

void Ball::serializeRemove( he::NetworkStream* /*stream*/ ) const
{

}

bool Ball::deserializeRemove( he::NetworkStream* stream )
{
    return Entity::deserializeRemove(stream);
}

void Ball::serialize( he::net::NetworkSerializer& /*serializer*/ )
{
    HE_ASSERT(false, "will never happen");
}

void Ball::deserialize( he::net::NetworkDeserializer& serializer )
{
    serializer.deserializeVariable(m_MoveTo);
    serializer.deserializeVariable(m_Velocity);
}

} //end namespace