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

namespace happytest {

TestObject::TestObject(const he::game::EntityDesc& desc): 
    m_Rotation(0), m_Position(0, 0, 0), 
    m_pActor(nullptr),
    m_pFont(CONTENT->loadFont("Ubuntu-Regular.ttf", 14)),
    m_pMaterial(desc.physicsDesc.pMaterial),
    Entity(desc)
{
    using namespace he;
        
    ASSERT(desc.physicsDesc.usePhysics, "this class must have px properties");
    m_pActor = NEW he::px::PhysicsDynamicActor(mat44::createTranslation(m_Position),
        CONTENT->loadPhysicsShape(desc.physicsDesc.shape), desc.physicsDesc.density, m_pMaterial);
    m_pActor->setKeyframed(true);
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