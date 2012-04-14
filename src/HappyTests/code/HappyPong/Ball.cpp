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
#include "HappyTestsPCH.h" 

#include "Ball.h"

#include "ModelComponent.h"
#include "ContentManager.h"
#include "Game.h"
#include "LightComponent.h"
#include "LightFlashComponent.h"
#include "MainGame.h"
#include "Palet.h"
#include "Obstacle.h"

namespace ht {

Ball::Ball(MainGame* mainGame): 
    m_Position(0, 0, 0), 
    m_Velocity(0, 0, 0), 
    m_MainGame(mainGame),
    m_Radius(1.0f),
    m_Restitution(1.01f)
{
    reset();

    he::game::ModelComponent* model(NEW he::game::ModelComponent());
    model->setMaterial(CONTENT->loadMaterial("pong/ball.material"));
    he::gfx::ModelMesh* mesh(CONTENT->asyncLoadModelMesh("pong/ball.binobj", "M_Bal", model->getMaterial().getCompatibleVertexLayout()));
    model->setModelMesh(mesh->getHandle());
    mesh->release();
    model->setLocalTransform(he::mat44::createScale(100));
    addComponent(model);

    m_LightFlashComponent = NEW LightFlashComponent();
    addComponent(m_LightFlashComponent);
    m_LightFlashComponent->setAttenuation(1.0f, 50);
    m_LightFlashComponent->setMultiplier(50);
    m_LightFlashComponent->setColor(he::Color(1.0f, 1.0f, 0.5f));
    m_LightFlashComponent->setOffset(he::vec3(0.0f, 1.5f, 0));
    m_LightFlashComponent->setFlashMultiplier(50);
    m_LightFlashComponent->setNormalMultiplier(0);
    m_LightFlashComponent->setFlashDuration(0.25f);

    he::game::PointLightComponent* redLight(NEW he::game::PointLightComponent());
    addComponent(redLight);
    redLight->setAttenuation(1.0f, 50);
    redLight->setMultiplier(10);
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
    m_Position += m_Velocity * dTime;
    he::vec3 normal(0, 0, 0);
    bool reflect(false);
    
    if (m_Position.z - m_Radius < -m_MainGame->getBoardDimension().y / 2)
    {
        m_Position.z = -m_MainGame->getBoardDimension().y / 2 + m_Radius;
        normal += he::vec3(0, 0, 1);
        reflect = true;
    }
    else if (m_Position.z + m_Radius > m_MainGame->getBoardDimension().y / 2)
    {
        m_Position.z = m_MainGame->getBoardDimension().y / 2 - m_Radius;
        normal += he::vec3(0, 0, -1);
        reflect = true;
    }

    if (m_Dead == false)
    {
        if (m_Position.x - m_Radius < -m_MainGame->getBoardDimension().x / 2)
        {
            m_MainGame->addPoint(0);
            m_MainGame->restart(true);
            m_Dead = true;
        }
        else if (m_Position.x + m_Radius > m_MainGame->getBoardDimension().x / 2)
        {
            m_MainGame->addPoint(1);
            m_MainGame->restart(true);
            m_Dead = true;
        }
    }


    std::for_each(m_MainGame->getPalets().cbegin(), m_MainGame->getPalets().cend(), [&](Palet* palet)
    {
        if (m_Position.z + m_Radius > palet->getPosition().z - palet->getDimension().y / 2 &&
            m_Position.z - m_Radius < palet->getPosition().z + palet->getDimension().y / 2)
        {
            if (m_Position.x + m_Radius > palet->getPosition().x - palet->getDimension().x / 2 && 
                m_Position.x - m_Radius < palet->getPosition().x + palet->getDimension().x / 2)
            {
                if (m_Velocity.x < 0)
                {
                    m_Position.x = palet->getPosition().x + palet->getDimension().x / 2 + m_Radius;
                    normal += he::vec3(1, 0, 0);
                }
                else
                {
                    m_Position.x = palet->getPosition().x - palet->getDimension().x / 2 - m_Radius;
                    normal += he::vec3(-1, 0, 0);
                }
                reflect = true;
                palet->hitByBall();
            }
        }
    });

    std::for_each(m_MainGame->getObstacles().cbegin(), m_MainGame->getObstacles().cend(), [&](const Obstacle* obstacle)
    {
        if (he::lengthSqr(m_Position - obstacle->getPosition()) < he::sqr(m_Radius + obstacle->getRadius()))
        {
            reflect = true;
            normal += he::normalize(m_Position - obstacle->getPosition());
            m_Position = obstacle->getPosition() + normal * (m_Radius + obstacle->getRadius());
        }
    });


    if (reflect)
    {
        m_Velocity = he::reflect(-m_Velocity, he::normalize(normal)) * m_Restitution;
        m_LightFlashComponent->flash();

    }
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

} //end namespace