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

#include "Palet.h"

#include "ControlsManager.h"
#include "ContentManager.h"

#include "ModelComponent.h"
#include "ModelMesh.h"

#include "Material.h"
#include "Game.h"
#include "LightFlashComponent.h"

namespace ht {

Palet::Palet(he::byte player, const he::vec2& boardDim): 
    m_BoardDim(boardDim), 
    m_Speed(50.0f), 
    m_PaletDim(1.0f, 5.0f),
    m_Player(player)
{
    if (m_Player == 0)
        m_Position = he::vec3(40, 0, 0);
    else
        m_Position = he::vec3(-40, 0, 0);

    he::game::ModelComponent* model(NEW he::game::ModelComponent());
    model->setMaterial(CONTENT->loadMaterial("pong/palet.material"));
    model->setModelMesh(CONTENT->asyncLoadModelMesh("pong/palet.binobj", "M_Palet", model->getMaterial().getCompatibleVertexLayout()));
    model->setLocalTransform(he::mat44::createScale(100));
    addComponent(model);
    
    m_LightFlashComponent = NEW LightFlashComponent();
    addComponent(m_LightFlashComponent);
    m_LightFlashComponent->setAttenuation(1.0f, 50);
    if (m_Player == 0)
        m_LightFlashComponent->setColor(he::Color(0.2f, 1.0f, 0.2f));
    else
        m_LightFlashComponent->setColor(he::Color(0.2f, 0.2f, 1.0f));
    m_LightFlashComponent->setOffset(he::vec3(0.0f, 1.5f, 0));
    m_LightFlashComponent->setFlashMultiplier(50);
    m_LightFlashComponent->setNormalMultiplier(10);
    m_LightFlashComponent->setFlashDuration(0.5f);

    m_LightFlashAddPointComponent = NEW LightFlashComponent();
    m_LightFlashAddPointComponent->init(nullptr);
    m_LightFlashAddPointComponent->setAttenuation(1.0f, 100);
    if (m_Player == 0)
    {
        m_LightFlashAddPointComponent->setOffset(he::vec3(-boardDim.x/2, 0.5f, 0));
        m_LightFlashAddPointComponent->setColor(he::Color(0.2f, 1.0f, 0.2f));
    }
    else
    {
        m_LightFlashAddPointComponent->setOffset(he::vec3(boardDim.x/2, 0.5f, 0));
        m_LightFlashAddPointComponent->setColor(he::Color(0.2f, 0.2f, 1.0f));
    }
    m_LightFlashAddPointComponent->setFlashMultiplier(200);
    m_LightFlashAddPointComponent->setNormalMultiplier(0);
    m_LightFlashAddPointComponent->setFlashDuration(1.0f);

    GAME->addToTickList(this);
}


Palet::~Palet()
{
    delete m_LightFlashAddPointComponent;
    GAME->removeFromTickList(this);
}

void Palet::tick( float dTime )
{
    he::io::Key up(m_Player == 0? he::io::Key_W : he::io::Key_Up);
    he::io::Key down(m_Player == 0? he::io::Key_S : he::io::Key_Down);

    if (CONTROLS->getKeyboard()->isKeyDown(up))
    {
        m_Position.z += m_Speed * dTime;
        if (m_Position.z + m_PaletDim.y / 2 > m_BoardDim.y / 2)
            m_Position.z = m_BoardDim.y / 2 - m_PaletDim.y / 2;
    }
    if (CONTROLS->getKeyboard()->isKeyDown(down))
    {
        m_Position.z -= m_Speed * dTime;
        if (m_Position.z - m_PaletDim.y / 2 <  -m_BoardDim.y / 2)
            m_Position.z = -m_BoardDim.y / 2 + m_PaletDim.y / 2;
    }
    setWorldMatrix(he::mat44::createTranslation(m_Position));
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
    m_LightFlashComponent->flash();
}

void Palet::addPoint()
{
    m_LightFlashAddPointComponent->flash();
}

} //end namespace