//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#include "Obstacle.h"

#include "ControlsManager.h"
#include "ContentManager.h"

#include "ModelComponent.h"
#include "ModelMesh.h"

#include "Material.h"
#include "Game.h"

namespace hpc {

Obstacle::Obstacle(): 
    m_Position(0, 0, 0), 
    m_Radius(8.0f)
{
    he::ge::ModelComponent* model(NEW he::ge::ModelComponent());
    model->setModelMeshAndMaterial("pong/obstacle.material", "pong/obstacles.binobj");
    model->setLocalScale(he::vec3(100, 100, 100));
    addComponent(model);

    setLocalTranslate(m_Position);
}


Obstacle::~Obstacle()
{
}

float Obstacle::getRadius() const
{
    return m_Radius;
}

const he::vec3& Obstacle::getPosition() const
{
    return m_Position;
}

} //end namespace