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
#include "RiggedModelComponent.h"
#include "PhysicsMaterial.h"
#include "PhysicsConvexShape.h"

namespace happytest {

TestObject::TestObject(): 
    m_Rotation(0), m_Position(0, 0, 0), 
    m_pFont(CONTENT->loadFont("Ubuntu-Regular.ttf", 14)),
    m_WheelOrientation(0.0f), m_WheelMax(he::piOverFour), m_WheelTurnSpeed(2.0f/he::piOverFour), m_WheelSpeedRotation(0.0f)
{
    using namespace he;
        

    setWorldMatrix(mat44::createTranslation(m_Position));

    m_PhysicsComponent = NEW game::DynamicPhysicsComponent();
    addComponent(m_PhysicsComponent);
    px::PhysicsMaterial material(0.8f, 0.5f, 0.1f);
    const std::vector<px::PhysicsConvexMesh::pointer>& pPMeshes(CONTENT->loadPhysicsConvex("car.pxcv"));
    std::for_each(pPMeshes.cbegin(), pPMeshes.cend(), [&](const px::PhysicsConvexMesh::pointer& pMesh)
    {
        he::px::PhysicsConvexShape shape(pMesh);
        m_PhysicsComponent->addShape(&shape, material, 300.0f);
    });
    m_PhysicsComponent->getDynamicActor()->setKeyframed(true);

    m_pRiggedModelComponent = NEW game::RiggedModelComponent();
    m_pRiggedModelComponent->setMaterial(CONTENT->loadMaterial("car.material"));
    m_pRiggedModelComponent->setModel(CONTENT->asyncLoadModelMesh("car.binobj", "M_Car", m_pRiggedModelComponent->getMaterial().getCompatibleVertexLayout()));
    addComponent(m_pRiggedModelComponent);
    m_pRiggedModelComponent->getModel()->callbackIfLoaded(boost::bind(&TestObject::onModelLoaded, this));
}


TestObject::~TestObject()
{
}


void TestObject::tick(float dTime)
{
    if (m_pRiggedModelComponent->isVisible())
    {
        using namespace he;
        if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Left))
        {
            m_WheelOrientation += m_WheelTurnSpeed * dTime;
            if (m_WheelOrientation > m_WheelMax)
                m_WheelOrientation = m_WheelMax;
        }
        else if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Right))
        {
            m_WheelOrientation -= m_WheelTurnSpeed * dTime;
            if (m_WheelOrientation < -m_WheelMax)
                m_WheelOrientation = -m_WheelMax;
        }
        else
        {
            if (abs(m_WheelOrientation) <= m_WheelTurnSpeed * dTime)
            {
                m_WheelOrientation = 0.0f;
            }
            else
            {
                if (m_WheelOrientation > 0)
                    m_WheelOrientation -= m_WheelTurnSpeed * dTime;
                else
                    m_WheelOrientation += m_WheelTurnSpeed * dTime;
            }
        }
        if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Down))
        {
            m_Rotation -= m_WheelOrientation * dTime * 2;
            m_Position -= vec3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
            m_WheelSpeedRotation -= -dTime * 5;
        }
        else if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Up))
        {
            m_Rotation += m_WheelOrientation * dTime * 2;
            m_Position += vec3(cosf(m_Rotation), 0, -sinf(m_Rotation)) * dTime * 5;
            m_WheelSpeedRotation -= dTime * 5;
        }
        m_PhysicsComponent->getDynamicActor()->keyframedSetPose(m_Position, vec3(0, 1, 0), m_Rotation);

        (*m_LeftWheelBone.m_RealTransform) = m_LeftWheelBone.m_FromOrigTransform * mat44::createRotation(vec3::up, m_WheelOrientation) * mat44::createRotation(vec3::forward, m_WheelSpeedRotation) * m_LeftWheelBone.m_ToOrigTransform;
        (*m_RightWheelBone.m_RealTransform) = m_RightWheelBone.m_FromOrigTransform * mat44::createRotation(vec3::up, m_WheelOrientation) * mat44::createRotation(vec3::forward, m_WheelSpeedRotation) * m_RightWheelBone.m_ToOrigTransform;
    }
}

void TestObject::onModelLoaded()
{
    m_LeftWheelBone = m_pRiggedModelComponent->getBone("b_WheelLeft");
    m_RightWheelBone = m_pRiggedModelComponent->getBone("b_WheelRight");
}

} //end namespace