//HappyTest Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
#include "ModelComponent.h"
#include "LightComponent.h"

#include "HappyEngine.h"

#include "ContentManager.h"
#include "ControlsManager.h"
#include "Happy2DRenderer.h"
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"
#include "SoundEngine.h"

#include "MathConstants.h"
#include "MathFunctions.h"

#include "PhysicsCarManager.h"
#include "PhysicsMaterial.h"
#include "PhysicsConvexShape.h"

#include "PhysicsData.h"

#include "fx/FxManager.h"
#include "fx/FxTimeLine.h"
#include "fx/FxCameraEffect.h"
#include "fx/FxRandom.h"
#include "fx/FxTimeLineTrack.h"

#include "PhysicsRagdoll.h"

namespace happytest {

TestObject::TestObject(): 
    m_Rotation(0), m_Position(5, 0, 0), 
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
            wheelShapes[px::PhysicsCar::Tyre_FrontLeft] = shape;
        else if (pMesh->getName() == "pxcv_WheelFR")
            wheelShapes[px::PhysicsCar::Tyre_FrontRight] = shape;
        else if (pMesh->getName() == "pxcv_WheelBL")
            wheelShapes[px::PhysicsCar::Tyre_RearLeft] = shape;
        else if (pMesh->getName() == "pxcv_WheelBR")
            wheelShapes[px::PhysicsCar::Tyre_RearRight] = shape;
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
    
    m_pCar->setPose(mat44::createTranslation(vec3(5, 2, 0)));

    m_pRiggedModelComponent = NEW game::RiggedModelComponent();
    m_pRiggedModelComponent->setMaterial(CONTENT->loadMaterial("car.material"));
    m_pRiggedModelComponent->setModelMesh(CONTENT->asyncLoadModelMesh("car.binobj", "M_Car", m_pRiggedModelComponent->getMaterial().getCompatibleVertexLayout()));
    addComponent(m_pRiggedModelComponent);
    m_pRiggedModelComponent->getModelMesh()->callbackIfLoaded(boost::bind(&TestObject::onModelLoaded, this));

    game::ModelComponent* pCharModelComp(NEW game::ModelComponent());
    pCharModelComp->setMaterial(CONTENT->loadMaterial("char/char1.material"));
    pCharModelComp->setModelMesh(CONTENT->asyncLoadModelMesh("char/char1.binobj", "langen", pCharModelComp->getMaterial().getCompatibleVertexLayout()));
    pCharModelComp->setLocalTransform(mat44::createScale(vec3(1, 1, 1)) * mat44::createTranslation(vec3(0.0f, 0.25f, 1.0f)));
    pCharModelComp->setCastsShadow(true);
    addComponent(pCharModelComp);


    game::SpotLightComponent* pHeadLight1(NEW game::SpotLightComponent());
    addComponent(pHeadLight1);
    pHeadLight1->setOffset(vec3(-0.607f, -0.095, 2.158f));
    pHeadLight1->setMultiplier(5);
    pHeadLight1->setAttenuation(0, 20);
    pHeadLight1->setDirection(normalize(vec3(0, -0.2, -1)));

    game::SpotLightComponent* pHeadLight2(NEW game::SpotLightComponent());
    addComponent(pHeadLight2);
    pHeadLight2->setOffset(vec3(0.607f, -0.095, 2.158f));
    pHeadLight2->setMultiplier(5);
    pHeadLight2->setAttenuation(0, 20);
    pHeadLight2->setDirection(normalize(vec3(0, -0.2, -1)));

    game::PointLightComponent* pRearLight1(NEW game::PointLightComponent());
    addComponent(pRearLight1);
    pRearLight1->setOffset(vec3(0.567f, 0.047, -2.219f));
    pRearLight1->setMultiplier(2);
    pRearLight1->setAttenuation(0, 3);
    pRearLight1->setColor(vec3(1.0f, 0.5f, 0.5f));

    game::PointLightComponent* pRearLight2(NEW game::PointLightComponent());
    addComponent(pRearLight2);
    pRearLight2->setOffset(vec3(-0.567f, 0.047, -2.219f));
    pRearLight2->setMultiplier(2);
    pRearLight2->setAttenuation(0, 3);
    pRearLight2->setColor(vec3(1.0f, 0.2f, 0.2f));

    m_pCarEngineSfx = AUDIO->loadSound3D("../data/audio/carIdle.wav", false);
    m_pCarEngineSfx->setLooping(true);
    //m_pCarEngineSfx->play();
    m_pCarEngineSfx->setMaximumDistance(100.0f);
    m_pCarEngineSfx->setMinimumDistance(10.0f);

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


    //Ragdoll
    m_pCharacter = NEW Character();
}


TestObject::~TestObject()
{
    delete m_pCharacter;
}


void TestObject::tick(float /*dTime*/)
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
    CONTROLS->getFocus(this);

    if (CONTROLS->hasFocus(this))
    {
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
    }

    CONTROLS->returnFocus(this);

    m_pCarEngineSfx->setPosition(getWorldMatrix().getTranslation());
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