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

#include "Game.h"
#include "RiggedModelComponent.h"

#include "HappyEngine.h"

#include "ContentManager.h"
#include "ControlsManager.h"
#include "Happy2DRenderer.h"
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"
#include "SoundEngine.h"
#include "FxManager.h"

#include "MathConstants.h"
#include "MathFunctions.h"

#include "PhysicsCarManager.h"
#include "PhysicsMaterial.h"
#include "PhysicsConvexShape.h"

#include "PhysicsData.h"

#include "FxTimeLine.h"
#include "FxCameraEffect.h"
#include "FxRandom.h"
#include "FxTimeLineTrack.h"

namespace happytest {

TestObject::TestObject(): 
    m_Rotation(0), m_Position(0, 0, 0), 
    m_pFont(CONTENT->loadFont("Ubuntu-Regular.ttf", 32, true)),
    m_WheelOrientation(0.0f), m_WheelMax(he::piOverFour), m_WheelTurnSpeed(2.0f/he::piOverFour), m_WheelSpeedRotation(0.0f)
{
    using namespace he;
        

    setWorldMatrix(mat44::createTranslation(m_Position));

    const std::vector<px::PhysicsConvexMesh::pointer>& pPMeshes(CONTENT->loadPhysicsConvex("car.pxcv"));
    std::vector<px::PhysicsConvexShape> wheelShapes;


    px::ChassiDesc chassi;
    chassi.m_Aabb = vec3(2.025f, 1.323f, 4.445f);
    chassi.m_Mass = 1250;

    wheelShapes.resize(4); //4 wheels
    std::for_each(pPMeshes.cbegin(), pPMeshes.cend(), [&](const px::PhysicsConvexMesh::pointer& pMesh)
    {
        using namespace he;
        px::PhysicsConvexShape shape(pMesh);
        if (pMesh->getName() == "pxcv_WheelFL")
            memcpy(&wheelShapes[px::PhysicsCar::Tyre_FrontLeft], &shape, sizeof(px::PhysicsConvexShape));
        else if (pMesh->getName() == "pxcv_WheelFR")
            memcpy(&wheelShapes[px::PhysicsCar::Tyre_FrontRight], &shape, sizeof(px::PhysicsConvexShape));
        else if (pMesh->getName() == "pxcv_WheelBL")
            memcpy(&wheelShapes[px::PhysicsCar::Tyre_RearLeft], &shape, sizeof(px::PhysicsConvexShape));
        else if (pMesh->getName() == "pxcv_WheelBR")
            memcpy(&wheelShapes[px::PhysicsCar::Tyre_RearRight], &shape, sizeof(px::PhysicsConvexShape));
        else
            chassi.m_ChassiShapes.push_back(shape);
    });


    m_pCar = PHYSICS->getCarManager()->createCar();


    std::vector<px::TyreDesc> tyres(4);
    tyres[px::PhysicsCar::Tyre_FrontLeft] = px::TyreDesc(
        vec3( 0.691f, -0.314f*2.5f,  1.348f),
        0.335f, 0.281f, 20, wheelShapes[0]);
    tyres[px::PhysicsCar::Tyre_FrontRight] = px::TyreDesc(
        vec3(-0.691f, -0.314f*2.5f,  1.348f),
        0.335f, 0.281f, 20, wheelShapes[1]);
    tyres[px::PhysicsCar::Tyre_RearLeft] = px::TyreDesc(
        vec3( 0.691f, -0.314f*2.5f, -1.348f),
        0.335f, 0.281f, 20, wheelShapes[2]);
    tyres[px::PhysicsCar::Tyre_RearRight] = px::TyreDesc(
        vec3(-0.691f, -0.314f*2.5f, -1.348f),
        0.335f, 0.281f, 20, wheelShapes[3]);
    m_pCar->init(chassi, PHYSICS->getCarManager()->getFrictionTable()->getMaterial(DM_Metal),
                 tyres, PHYSICS->getCarManager()->getFrictionTable()->getMaterial(DM_Tyre));
    
    m_pCar->setPose(mat44::createTranslation(vec3(0, 2, 0)));

    m_pRiggedModelComponent = NEW game::RiggedModelComponent();
    m_pRiggedModelComponent->setMaterial(CONTENT->loadMaterial("car.material"));
    m_pRiggedModelComponent->setModel(CONTENT->asyncLoadModelMesh("car.binobj", "M_Car", m_pRiggedModelComponent->getMaterial().getCompatibleVertexLayout()));
    addComponent(m_pRiggedModelComponent);
    m_pRiggedModelComponent->getModel()->callbackIfLoaded(boost::bind(&TestObject::onModelLoaded, this));



    m_pCarEngineSfx = AUDIO->loadSound2D("../data/audio/carIdle.wav", false);
    m_pCarEngineSfx->setLooping(true);
    m_pCarEngineSfx->play();

    //m_pCar->setAutoTransmission(false);

    GAME->addToTickList(this);


    m_CameraShakeTL = FX->createTimeline();
    gfx::FxTimeLine* pTL(FX->getTimeline(m_CameraShakeTL));
    pTL->setEndTime(0.25f);
    gfx::FxTimeLineTrack* pTrack(pTL->getTrack(pTL->addTrack()));
    gfx::FxCameraEffect* pEffect(pTrack->getComponent<gfx::FxCameraEffect>(pTrack->addComponent(gfx::FxType_CameraEffect)));
    pEffect->setStartTime(0.0f);
    pEffect->setEndTime(0.25f);
    
    gfx::FxRandomVec3::pointer shakeIntens(NEW gfx::FxRandomVec3());
    shakeIntens->setMin(vec3(0, 0, 0));
    shakeIntens->setMax(vec3(0.25f, 0.25f, 10.0f));
    pEffect->setShakeIntensity(shakeIntens);
    pEffect->toggleShake(true);
}


TestObject::~TestObject()
{
}


void TestObject::tick(float dTime)
{
    using namespace he;

    setWorldMatrix(m_pCar->getChassiPose());
    if (m_pRiggedModelComponent->isVisible())
    {
        (*m_WheelFLBone.m_RealTransform) = /*m_WheelFLBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_FrontLeft) * m_WheelFLBone.m_ToOrigTransform;
        (*m_WheelFRBone.m_RealTransform) = /*m_WheelFRBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_FrontRight) * m_WheelFRBone.m_ToOrigTransform;
        (*m_WheelBLBone.m_RealTransform) = /*m_WheelBLBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_RearLeft) * m_WheelBLBone.m_ToOrigTransform;
        (*m_WheelBRBone.m_RealTransform) = /*m_WheelBRBone.m_FromOrigTransform */ m_pCar->getTyrePose(px::PhysicsCar::Tyre_RearRight) * m_WheelBRBone.m_ToOrigTransform;
    }

    //////////////////////////////////////////////////////////////////////////
    ///     INPUT                                                          ///
    //////////////////////////////////////////////////////////////////////////

    m_pCar->inputAccel(CONTROLS->getKeyboard()->isKeyDown(io::Key_Up));
    m_pCar->inputBrake(CONTROLS->getKeyboard()->isKeyDown(io::Key_Down));
    m_pCar->inputTurnLeft(CONTROLS->getKeyboard()->isKeyDown(io::Key_Left));
    m_pCar->inputTurnRight(CONTROLS->getKeyboard()->isKeyDown(io::Key_Right));
    m_pCar->inputHandBrake(CONTROLS->getKeyboard()->isKeyDown(io::Key_Lctrl));
    m_pCar->inputGearUp(CONTROLS->getKeyboard()->isKeyPressed(io::Key_PageUp));
    m_pCar->inputGearDown(CONTROLS->getKeyboard()->isKeyPressed(io::Key_PageDown));
    if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Backspace))
    {
        m_pCar->setPose(mat44::createTranslation(m_pCar->getPosition() + vec3(0, 2, 0)));
        m_pCar->reset();
    }
    if (CONTROLS->getKeyboard()->isKeyPressed(io::Key_Delete))
    {
        m_pCar->setPose(mat44::createTranslation(vec3(0, 2, 0)));
        m_pCar->reset();
    }

    if (m_pCar->isInAir() && CONTROLS->getKeyboard()->isKeyPressed(io::Key_Down))
        m_pCar->setAngularVelocity(vec3(0, 0, 0));
    if (CONTROLS->getKeyboard()->isKeyDown(io::Key_Lshift))
    {
        if (FX->getTimeline(m_CameraShakeTL)->isRunning() == false)
            FX->getTimeline(m_CameraShakeTL)->start();
        vec4 force(0, 0, 15000, 0);
        m_pCar->addForce((m_pCar->getPose() * force).xyz());
    }

    m_pCarEngineSfx->setPitch(he::clamp<float>(m_pCar->getEngineRPM() / 150 + 1.0f, 1, 6));

    //////////////////////////////////////////////////////////////////////////
    ///     GUI                                                            ///
    //////////////////////////////////////////////////////////////////////////    
    GUI->setColor(33 / 255.0f, 30 / 255.0f, 32 / 255.0f);
    GUI->fillShape2D(gui::Rectangle2D(vec2(-1, GRAPHICS->getScreenHeight() - 64.0f), vec2(384, 64)), true);
    GUI->fillShape2D(gui::Rectangle2D(vec2(GRAPHICS->getScreenWidth() - 128.0f - 16, GRAPHICS->getScreenHeight() - 64.0f), vec2(128+16, 64)), true);
    GUI->fillShape2D(gui::Rectangle2D(vec2(GRAPHICS->getScreenWidth()/2 - 64.0f - 16, GRAPHICS->getScreenHeight() - 64.0f), vec2(128, 64)), true);

    GUI->setColor(1, 1, 1);

    char speedText[20];
    if (m_pCar->getSpeed()*3.6f < 1000)
        std::sprintf(speedText, "Speed: %06.2f km/h", m_pCar->getSpeed()*3.6f);
    else
        std::sprintf(speedText, "Speed: 999.99 km/h");
    GUI->drawText(gui::Text(speedText, m_pFont), vec2(12, GRAPHICS->getScreenHeight() - 52.0f));

    char gearText[7];
    if (m_pCar->getGear() > 1)
        std::sprintf(gearText, "Gear: %1d", m_pCar->getGear()-1);
    else if (m_pCar->getGear() == 1)
        std::sprintf(gearText, "Gear: N");
    else
        std::sprintf(gearText, "Gear: R");

    char engineText[20];
    std::sprintf(engineText, "Engine: %07.2f", m_pCar->getEngineRPM());
    GUI->drawText(gui::Text(engineText, m_pFont), vec2(GRAPHICS->getScreenWidth() / 2 - 64.0f + 4, GRAPHICS->getScreenHeight() - 72.0f));

    GUI->drawText(gui::Text(gearText, m_pFont), vec2(GRAPHICS->getScreenWidth() - 128.0f - 16 + 12, GRAPHICS->getScreenHeight() - 52.0f));

    if (m_pCar->isDrifting())
        GUI->drawText(gui::Text("Drift!", m_pFont), vec2(GRAPHICS->getScreenWidth() / 2 - 64.0f + 4, GRAPHICS->getScreenHeight() - 52.0f));
}

void TestObject::onModelLoaded()
{
    m_WheelFLBone = m_pRiggedModelComponent->getBone("b_WheelFL");
    m_WheelFRBone = m_pRiggedModelComponent->getBone("b_WheelFR");
    m_WheelBLBone = m_pRiggedModelComponent->getBone("b_WheelBL");
    m_WheelBRBone = m_pRiggedModelComponent->getBone("b_WheelBR");
}

} //end namespace