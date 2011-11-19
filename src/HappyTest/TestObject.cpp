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
#include "ContentManager.h"
#include "ControlsManager.h"
#include "RiggedModelComponent.h"
#include "PhysicsMaterial.h"
#include "PhysicsConvexShape.h"
#include "Game.h"

namespace happytest {

TestObject::TestObject(): 
    m_Rotation(0), m_Position(0, 0, 0), 
    m_pFont(CONTENT->loadFont("Ubuntu-Regular.ttf", 14)),
    m_WheelOrientation(0.0f), m_WheelMax(he::piOverFour), m_WheelTurnSpeed(2.0f/he::piOverFour), m_WheelSpeedRotation(0.0f)
{
    using namespace he;
        

    setWorldMatrix(mat44::createTranslation(m_Position));

    const std::vector<px::PhysicsConvexMesh::pointer>& pPMeshes(CONTENT->loadPhysicsConvex("car.pxcv"));
    std::vector<px::PhysicsConvexShape> wheelShapes;
    wheelShapes.resize(4); //4 wheels
    std::vector<px::PhysicsConvexShape> chassiShapes;
    std::for_each(pPMeshes.cbegin(), pPMeshes.cend(), [&](const px::PhysicsConvexMesh::pointer& pMesh)
    {
        using namespace he;
        px::PhysicsConvexShape shape(pMesh);
        if (pMesh->getName() == "pxcv_WheelFL")
            memcpy(&wheelShapes[0], &shape, sizeof(px::PhysicsConvexShape));
        else if (pMesh->getName() == "pxcv_WheelFR")
            memcpy(&wheelShapes[1], &shape, sizeof(px::PhysicsConvexShape));
        else if (pMesh->getName() == "pxcv_WheelBL")
            memcpy(&wheelShapes[2], &shape, sizeof(px::PhysicsConvexShape));
        else if (pMesh->getName() == "pxcv_WheelBR")
            memcpy(&wheelShapes[3], &shape, sizeof(px::PhysicsConvexShape));
        else
            chassiShapes.push_back(shape);
    });


    m_pCar = NEW px::PhysicsCar();
    m_pCar->init(wheelShapes, chassiShapes, PHYSICS->getDriveableMaterial(px::PxMat_Concrete));
    
    m_pRiggedModelComponent = NEW game::RiggedModelComponent();
    m_pRiggedModelComponent->setMaterial(CONTENT->loadMaterial("car.material"));
    m_pRiggedModelComponent->setModel(CONTENT->asyncLoadModelMesh("car.binobj", "M_Car", m_pRiggedModelComponent->getMaterial().getCompatibleVertexLayout()));
    addComponent(m_pRiggedModelComponent);
    m_pRiggedModelComponent->getModel()->callbackIfLoaded(boost::bind(&TestObject::onModelLoaded, this));

    GAME->addToTickList(this);
}


TestObject::~TestObject()
{
    delete m_pCar;
}


void TestObject::tick(float dTime)
{
    using namespace he;
    m_pCar->tick(dTime);
    setWorldMatrix(m_pCar->getChassiPose());
    if (m_pRiggedModelComponent->isVisible())
    {
        (*m_WheelFLBone.m_RealTransform) = /*m_WheelFLBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_FrontLeft) * m_WheelFLBone.m_ToOrigTransform;
        (*m_WheelFRBone.m_RealTransform) = /*m_WheelFRBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_FrontRight) * m_WheelFRBone.m_ToOrigTransform;
        (*m_WheelBLBone.m_RealTransform) = /*m_WheelBLBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_BackLeft) * m_WheelBLBone.m_ToOrigTransform;
        (*m_WheelBRBone.m_RealTransform) = /*m_WheelBRBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_BackRight) * m_WheelBRBone.m_ToOrigTransform;
    }
}

void TestObject::onModelLoaded()
{
    m_WheelFLBone = m_pRiggedModelComponent->getBone("b_WheelFL");
    m_WheelFRBone = m_pRiggedModelComponent->getBone("b_WheelFR");
    m_WheelBLBone = m_pRiggedModelComponent->getBone("b_WheelBL");
    m_WheelBRBone = m_pRiggedModelComponent->getBone("b_WheelBR");
}

} //end namespace