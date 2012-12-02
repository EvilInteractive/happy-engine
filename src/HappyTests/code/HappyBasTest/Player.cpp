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
//Created: 28/11/2012
#include "HappyBasTestPCH.h" 

#include "Player.h"
#include "PlayerMover.h"
#include "PlayerThirdPersonCamera.h"
#include "MainGame.h"

#include "ModelComponent.h"
#include "Scene.h"
#include "CameraManager.h"
#include "LightComponent.h"
#include "Light.h"

namespace ht {

#pragma warning(disable:4355) // this in constructor
Player::Player()
    : m_Mover(NEW PlayerMover(this))
    , m_Camera(NEW PlayerThirdPersonCamera(this))
{
    he::gfx::Scene* scene(static_cast<MainGame*>(GAME)->getScene());
    init(scene);
    addComponent(m_Mover);

    he::ge::ModelComponent* const model(NEW he::ge::ModelComponent);
    model->setModelMeshAndMaterial("testPlatformer/player.material", "testPlatformer/player2m.binobj");
    addComponent(model);

    he::ge::SpotLightComponent* const spotLight(NEW he::ge::SpotLightComponent);
    addComponent(spotLight);
    spotLight->setAttenuation(1.0f, 20.0f);
    spotLight->setDirection(he::vec3::forward);
    spotLight->setFov(he::piOverTwo);
    spotLight->setColor(he::Color(0.5f, 0.8f, 1.0f));
    spotLight->setMultiplier(2.0f);
    spotLight->setShadow(he::gfx::ShadowResolution_512);
    spotLight->setLocalTranslate(he::vec3(0, 1, 0));

    scene->getCameraManager()->addCamera("player3thperson", m_Camera);
}
#pragma warning(default:4355) // this in constructor


Player::~Player()
{
    he::gfx::Scene* scene(static_cast<MainGame*>(GAME)->getScene());
    scene->getCameraManager()->deleteCamera("player3thperson");
}

he::vec3 Player::getCenterPosition() const
{
    he::vec3 pos;
    getWorldMatrix().getTranslationComponent(pos);
    pos.y += 1.0f;
    return pos;
}

} //end namespace