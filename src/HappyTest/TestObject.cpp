//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#include "HappyNew.h"
#include "TestObject.h"
#include "MathConstants.h"
#include "HappyEngine.h"
#include "HappyEngine.h"
#include "ContentManager.h"
#include "ControlsManager.h"
#include "ModelComponent.h"
#include "PhysicsMaterial.h"

namespace happytest {

TestObject::TestObject(): 
    m_Rotation(0), m_Position(0, 0, 0), 
    m_pActor(nullptr),
    m_pFont(CONTENT->loadFont("Ubuntu-Regular.ttf", 14)),
    m_pMaterial(NEW he::px::PhysicsMaterial(0.8f, 0.5f, 0.1f))
{
    using namespace he;
        
    m_pActor = NEW px::PhysicsDynamicActor(mat44::createTranslation(m_Position),
        CONTENT->loadPhysicsShape("car.bphys"), 5.0f, m_pMaterial);
    m_pActor->setKeyframed(true);

    game::ModelComponent* pModelComp(NEW game::ModelComponent());
    pModelComp->setMaterial(CONTENT->loadMaterial("car.material"));
    pModelComp->setModel(CONTENT->asyncLoadModelMesh("car.binobj", "M_Car", pModelComp->getMaterial().getCompatibleVertexLayout()));
    addComponent(pModelComp);
}


TestObject::~TestObject()
{
    delete m_pActor;
}


void TestObject::tick(float dTime)
{
    using namespace he;
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Left))
    {
        m_Rotation += pi * dTime;
    }
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Right))
    {
        m_Rotation -= pi * dTime;
    }
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Down))
    {
        m_Position -= vec3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
    }
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Up))
    {
        m_Position += vec3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
    }
    m_pActor->keyframedSetPose(m_Position, vec3(0, 1, 0), m_Rotation);

    setWorldMatrix(m_pActor->getPose());
}

} //end namespace