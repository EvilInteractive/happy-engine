//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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
//Author:  Sebastiaan Sprengers
//Created: 11/12/2011

#include "TransformTools.h"
#include "HappyPCH.h"
#include "ContentManager.h"
#include "ExtraForward3DRenderer.h"
#include "CameraManager.h"
#include "ControlsManager.h"
#include "GraphicsEngine.h"

namespace happysandbox {

/* CONSTRUCTOR - DESTRUCTOR */
TransformTools::TransformTools() :	m_Mode(MODE_OFF),
                                    m_Pos(0.0f,0.0f,0.0f),
                                    m_Scale(1.0f),
                                    m_bButtonDown(false),
                                    m_bLockX(false),
                                    m_bLockY(false),
                                    m_bLockZ(false)
{
    m_BufferLayout.addElement(he::gfx::BufferElement(0, he::gfx::BufferElement::Type_Vec3, he::gfx::BufferElement::Usage_Position, sizeof(he::vec3), 0));

    m_pMoveTool = NEW he::game::Entity();

    m_pMoveAxis = NEW he::game::ModelComponent();
    m_pMoveAxis->setModelMesh(CONTENT->asyncLoadModelMesh("editor/transform_move_axis.binobj", "M_axis", m_BufferLayout));

    m_pMovePickAxisX = NEW he::game::ModelComponent();
    m_pMovePickAxisX->setModelMesh(CONTENT->asyncLoadModelMesh("editor/transform_move_pick_axis.binobj", "M_pick_axis", m_BufferLayout));
    m_pMovePickAxisX->setVisible(false);
    m_pMovePickAxisY = NEW he::game::ModelComponent();
    m_pMovePickAxisY->setModelMesh(CONTENT->asyncLoadModelMesh("editor/transform_move_pick_axis.binobj", "M_pick_axis", m_BufferLayout));
    m_pMovePickAxisY->setVisible(false);
    m_pMovePickAxisZ = NEW he::game::ModelComponent();
    m_pMovePickAxisZ->setModelMesh(CONTENT->asyncLoadModelMesh("editor/transform_move_pick_axis.binobj", "M_pick_axis", m_BufferLayout));
    m_pMovePickAxisZ->setVisible(false);

    m_pMoveTool->addComponent(m_pMovePickAxisX);
    m_pMoveTool->addComponent(m_pMovePickAxisY);
    m_pMoveTool->addComponent(m_pMovePickAxisZ);

    m_pMoveCenter = NEW he::game::ModelComponent();
    m_pMoveCenter->setModelMesh(CONTENT->asyncLoadModelMesh("editor/transform_move_center.binobj", "M_center", m_BufferLayout));

    //m_pMoveTool->addComponent()
  
    m_pRotateAxis = CONTENT->asyncLoadModelMesh("editor/transform_rotate_axis.binobj", "M_axis", m_BufferLayout);
}

TransformTools::~TransformTools()
{
    delete m_pMoveTool;
}

/* GENERAL */
void TransformTools::tick(float /*dTime*/)
{
    he::vec3 lengthV(CAMERAMANAGER->getActiveCamera()->getPosition() - m_Pos);
    float l(length(lengthV));

    m_Scale = l/10.0f;

    if (CONTROLS->getMouse()->isButtonDown(he::io::MouseButton_Left))
    {
        switch (m_Mode)
        {
            case MODE_MOVE:
            {
                if (m_bButtonDown == false)
                {
                    he::mat44 scale(he::mat44::createScale(m_Scale));
                    he::mat44 translation(he::mat44::createTranslation(m_Pos));

                    he::mat44 a1(translation * scale);
                    he::mat44 a2(translation * he::mat44::createRotation(he::vec3::forward, -he::piOverTwo) * scale);
                    he::mat44 a3(translation * he::mat44::createRotation(he::vec3::right, he::piOverTwo) * scale);

                    std::vector<he::gfx::IDrawable*> drawList;

                    /*m_pMovePickAxisX->setLocalTransform(a2);
                    m_pMovePickAxisY->setLocalTransform(a1);
                    m_pMovePickAxisZ->setLocalTransform(a3);*/

                    drawList.push_back(m_pMovePickAxisX);
                    //drawList.push_back(m_pMovePickAxisY);
                    //drawList.push_back(m_pMovePickAxisZ);

                    uint i(GRAPHICS->pick(CONTROLS->getMouse()->getPosition(), drawList));

                    if (i == 0)
                        m_bLockY = true;
                    else if (i == 1)
                        m_bLockX = true;
                    else if (i == 2)
                        m_bLockZ = true;
                }
                else
                {

                }

                break;
            }

            /*case MODE_ROTATE:
            {
                he::mat44 scale(he::mat44::createScale(m_Scale));
                he::mat44 translation(he::mat44::createTranslation(m_Pos));

                HE3DX->drawColoredNoDepth(m_pRotateAxis, translation * scale, he::Color(1.0f, 5 / 255.0f, 0.0f, 1.0f));
                HE3DX->drawColoredNoDepth(m_pRotateAxis, translation * he::mat44::createRotation(he::vec3::up, -he::piOverTwo) * scale, he::Color(0.0f, 120 / 255.0f, 1.0f, 1.0f));
                HE3DX->drawColoredNoDepth(m_pRotateAxis, translation * he::mat44::createRotation(he::vec3::right, he::piOverTwo) * scale, he::Color(0.0f, 255 / 255.0f, 12 / 255.0f, 1.0f));

                he::mat44 localTranslation1(he::mat44::createTranslation(he::vec3(0.75f,0.0f,0.0f)));
                he::mat44 localTranslation2(he::mat44::createTranslation(he::vec3(-0.75f,0.0f,0.0f)));
                he::mat44 localTranslation3(he::mat44::createTranslation(he::vec3(0.0f,0.75f,0.0f)));
                he::mat44 localTranslation4(he::mat44::createTranslation(he::vec3(0.0f,-0.75f,0.0f)));
                he::mat44 localTranslation5(he::mat44::createTranslation(he::vec3(0.0f,0.0f,0.75f)));
                he::mat44 localTranslation6(he::mat44::createTranslation(he::vec3(0.0f,0.0f,-0.75f)));

                HE3DX->drawColoredNoDepth(m_pMoveCenter, translation * scale * localTranslation1, he::Color(he::vec4(1.0f, 234 / 255.0f, 0.0f, 1.0f)));
                HE3DX->drawColoredNoDepth(m_pMoveCenter, translation * scale * localTranslation2, he::Color(he::vec4(1.0f, 234 / 255.0f, 0.0f, 1.0f)));
                HE3DX->drawColoredNoDepth(m_pMoveCenter, translation * scale * localTranslation3, he::Color(he::vec4(1.0f, 234 / 255.0f, 0.0f, 1.0f)));
                HE3DX->drawColoredNoDepth(m_pMoveCenter, translation * scale * localTranslation4, he::Color(he::vec4(1.0f, 234 / 255.0f, 0.0f, 1.0f)));
                HE3DX->drawColoredNoDepth(m_pMoveCenter, translation * scale * localTranslation5, he::Color(he::vec4(1.0f, 234 / 255.0f, 0.0f, 1.0f)));
                HE3DX->drawColoredNoDepth(m_pMoveCenter, translation * scale * localTranslation6, he::Color(he::vec4(1.0f, 234 / 255.0f, 0.0f, 1.0f)));

                break;
            }*/
        }

        m_bButtonDown = true;
    }
    else if (m_bButtonDown == true)
        m_bButtonDown = false;
}

void TransformTools::draw()
{
    switch (m_Mode)
    {
        case MODE_MOVE:
        {
            he::mat44 scale(he::mat44::createScale(m_Scale));
            he::mat44 translation(he::mat44::createTranslation(m_Pos));

            if (m_bLockY == false)
                HE3DX->drawColoredNoDepth(m_pMoveAxis->getModelMesh(), translation * scale, he::Color(0.0f, 120 / 255.0f, 1.0f, 1.0f));
                //HE3DX->drawColoredNoDepth(m_pMovePickAxisY->getModelMesh(), he::mat44::Identity, he::Color(0.0f, 120 / 255.0f, 1.0f, 1.0f));
            else
                HE3DX->drawColoredNoDepth(m_pMoveAxis->getModelMesh(), translation * scale, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));
            if (m_bLockX == false)
                HE3DX->drawColoredNoDepth(m_pMoveAxis->getModelMesh(), translation * he::mat44::createRotation(he::vec3::forward, -he::piOverTwo) * scale, he::Color(1.0f, 5 / 255.0f, 0.0f, 1.0f));
            else
                HE3DX->drawColoredNoDepth(m_pMoveAxis->getModelMesh(), translation * he::mat44::createRotation(he::vec3::forward, -he::piOverTwo) * scale, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));
            if (m_bLockZ == false)
                HE3DX->drawColoredNoDepth(m_pMoveAxis->getModelMesh(), translation * he::mat44::createRotation(he::vec3::right, he::piOverTwo) * scale, he::Color(0.0f, 255 / 255.0f, 12 / 255.0f, 1.0f));
            else 
                HE3DX->drawColoredNoDepth(m_pMoveAxis->getModelMesh(), translation * he::mat44::createRotation(he::vec3::right, he::piOverTwo) * scale, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));

            HE3DX->drawColoredNoDepth(m_pMoveCenter->getModelMesh(), translation * scale, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));

            break;
        }

        case MODE_ROTATE:
        {
            he::mat44 scale(he::mat44::createScale(m_Scale));
            he::mat44 translation(he::mat44::createTranslation(m_Pos));

            HE3DX->drawColoredNoDepth(m_pRotateAxis, translation * scale, he::Color(1.0f, 5 / 255.0f, 0.0f, 1.0f));
            HE3DX->drawColoredNoDepth(m_pRotateAxis, translation * he::mat44::createRotation(he::vec3::up, -he::piOverTwo) * scale, he::Color(0.0f, 120 / 255.0f, 1.0f, 1.0f));
            HE3DX->drawColoredNoDepth(m_pRotateAxis, translation * he::mat44::createRotation(he::vec3::right, he::piOverTwo) * scale, he::Color(0.0f, 255 / 255.0f, 12 / 255.0f, 1.0f));

            he::mat44 localTranslation1(he::mat44::createTranslation(he::vec3(0.75f,0.0f,0.0f)));
            he::mat44 localTranslation2(he::mat44::createTranslation(he::vec3(-0.75f,0.0f,0.0f)));
            he::mat44 localTranslation3(he::mat44::createTranslation(he::vec3(0.0f,0.75f,0.0f)));
            he::mat44 localTranslation4(he::mat44::createTranslation(he::vec3(0.0f,-0.75f,0.0f)));
            he::mat44 localTranslation5(he::mat44::createTranslation(he::vec3(0.0f,0.0f,0.75f)));
            he::mat44 localTranslation6(he::mat44::createTranslation(he::vec3(0.0f,0.0f,-0.75f)));

            HE3DX->drawColoredNoDepth(m_pMoveCenter->getModelMesh(), translation * scale * localTranslation1, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));
            HE3DX->drawColoredNoDepth(m_pMoveCenter->getModelMesh(), translation * scale * localTranslation2, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));
            HE3DX->drawColoredNoDepth(m_pMoveCenter->getModelMesh(), translation * scale * localTranslation3, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));
            HE3DX->drawColoredNoDepth(m_pMoveCenter->getModelMesh(), translation * scale * localTranslation4, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));
            HE3DX->drawColoredNoDepth(m_pMoveCenter->getModelMesh(), translation * scale * localTranslation5, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));
            HE3DX->drawColoredNoDepth(m_pMoveCenter->getModelMesh(), translation * scale * localTranslation6, he::Color(1.0f, 234 / 255.0f, 0.0f, 1.0f));

            break;
        }
    }
}

void TransformTools::setMode(MODE mode)
{
    if (m_Mode != mode)
        m_Mode = mode;
}

} //end namespace