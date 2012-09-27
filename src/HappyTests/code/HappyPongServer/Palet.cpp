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
#include "HappyPongServerPCH.h" 

#include "Palet.h"

#include "ControlsManager.h"
#include "ContentManager.h"

#include "ModelComponent.h"
#include "ModelMesh.h"

#include "Material.h"
#include "Game.h"

#include "MainGame.h"
#include "Ball.h"
#include "NetworkSerializer.h"

namespace hps {

IMPLEMENT_OBJECT(Palet)

Palet::Palet(): 
    m_Speed(50.0f), 
    m_PaletDim(1.0f, 5.0f),
    m_Player(0),
    m_Game(nullptr),
    m_Ai(false),
    m_GoUp(false),
    m_GoDown(false)
{        
    GAME->addToTickList(this);
}
void Palet::init( MainGame* ge, he::byte player, bool ai)
{
    m_Game = ge;
    m_Player = player;
    m_Ai = ai;
    if (m_Player == 0)
        m_Position = he::vec3(40, 0, 0);
    else
        m_Position = he::vec3(-40, 0, 0);
    m_MoveTo = m_Position;
}



Palet::~Palet()
{
    GAME->removeFromTickList(this);
    m_Game->unregisterPaddle(getOwner());
}

void Palet::tick( float dTime )
{
    if (m_Ai)
    {
        Ball* ball(m_Game->getBall());
        if (he::dot(ball->getVelocity(), m_MoveTo) > 0.0)
        {
            float z(ball->getPosition().z);
            if (fabs(m_MoveTo.z - z) > m_PaletDim.y/2)
            {
                if (m_MoveTo.z < z - m_Speed * dTime)
                {
                    m_MoveTo.z += m_Speed * dTime;
                }
                else if (m_MoveTo.z > z + m_Speed * dTime)
                {
                    m_MoveTo.z -= m_Speed * dTime;
                }
            }
        }
        else
        {// return to center
            if (m_MoveTo.z < -m_Speed * dTime)
            {
                m_MoveTo.z += m_Speed * dTime;
            }
            else if (m_MoveTo.z > m_Speed * dTime)
            {
                m_MoveTo.z -= m_Speed * dTime;
            }
        }
    }
    else
    {
        if (m_GoUp)
        {
            m_MoveTo.z += m_Speed * dTime;
            if (m_MoveTo.z + m_PaletDim.y / 2 > m_Game->getBoardDimension().y / 2)
                m_MoveTo.z = m_Game->getBoardDimension().y / 2 - m_PaletDim.y / 2;
        }
        if (m_GoDown)
        {
            m_MoveTo.z -= m_Speed * dTime;
            if (m_MoveTo.z - m_PaletDim.y / 2 <  -m_Game->getBoardDimension().y / 2)
                m_MoveTo.z = -m_Game->getBoardDimension().y / 2 + m_PaletDim.y / 2;
        }
    }

    float len(he::length(m_MoveTo - m_Position));
    if (len > m_Speed * dTime)
        m_Position += (m_MoveTo - m_Position) / len * m_Speed * dTime;
    else
        m_Position = m_MoveTo;
}

const he::vec2& Palet::getDimension() const
{
    return m_PaletDim;
}

const he::vec3& Palet::getPosition() const
{
    return m_Position;
}

void Palet::hitByBall()
{
}

void Palet::addPoint()
{
}

void Palet::serializeCreate( he::net::NetworkStream* stream ) const
{
    stream->Write(m_Position);
    stream->Write(m_Player);
    he::net::NetworkObject<Palet>::serializeCreate(stream);
}

bool Palet::deserializeCreate( he::net::NetworkStream* stream )
{
    bool succes(true);
    succes &= he::net::NetworkObject<Palet>::deserializeCreate(stream);
    return succes;
}

void Palet::serializeRemove( he::net::NetworkStream* stream ) const
{
    he::net::NetworkObject<Palet>::serializeRemove(stream);
}

bool Palet::deserializeRemove( he::net::NetworkStream* stream )
{
    return he::net::NetworkObject<Palet>::deserializeRemove(stream);
}

void Palet::serialize(const  he::net::NetworkSerializer& serializer )
{
    serializer.serializeVariable(m_MoveTo);
    serializer.serializeVariable(m_GoUp);
    serializer.serializeVariable(m_GoDown);
    setSerializeDataDirty(false);
}

void Palet::deserialize(const  he::net::NetworkDeserializer& deserializer )
{
    deserializer.deserializeVariable(m_MoveTo);
    deserializer.deserializeVariable(m_GoUp);
    deserializer.deserializeVariable(m_GoDown);
    //tick(deserializer.getDTime());
    setSerializeDataDirty(true);
}

} //end namespace