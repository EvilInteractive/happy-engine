//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 08/11/2011
#include "HappyPCH.h" 

#ifdef HE_ENABLE_QT
#include "HappyQtWidget.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "Happy2DRenderer.h"
#include "SimpleForward3DRenderer.h"
#include "QtMouse.h"
#include "QtKeyboard.h"
#include "ControlsManager.h"

namespace he {
namespace gfx {

HappyQtWidget::HappyQtWidget(/*qt::*/QWidget *parent): /*qt::*/QGLWidget(/*qt::*/QGLFormat(), parent)
{
    format().setProfile(/*qt::*/QGLFormat::CoreProfile);
    format().setVersion(3, 2);
}


HappyQtWidget::~HappyQtWidget()
{
}

void HappyQtWidget::initializeGL()
{
    m_pMouse = dynamic_cast<io::QtMouse*>(const_cast<io::IMouse*>(CONTROLS->getMouse()));
    ASSERT(m_pMouse != nullptr, "HappyQtWidget is used but not the qtMouse !");

    m_pKeyboard = dynamic_cast<io::QtKeyboard*>(const_cast<io::IKeyboard*>(CONTROLS->getKeyboard()));
    ASSERT(m_pKeyboard != nullptr, "HappyQtWidget is used but not the qtKeyboard !");

    init();
    if (GRAPHICS != nullptr)
        GRAPHICS->init(true);
    if (GUI != nullptr)
        GUI->init();
    if (HE3D != nullptr)       
        HE3D->init();
}
void HappyQtWidget::paintGL()
{
    /*makeCurrent();
    draw();*/
}
void HappyQtWidget::resizeGL(int width, int height)
{
    GRAPHICS->setScreenDimension(width, height);
    GRAPHICS->setViewport(RectI(0, 0, width, height));
}
void HappyQtWidget::present()
{
    swapBuffers();
}
void HappyQtWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_pMouse->mouseMoveEvent(event);
    QWidget::mouseMoveEvent(event);
}
void HappyQtWidget::mousePressEvent(QMouseEvent* event)
{
    m_pMouse->mousePressEvent(event);
    QWidget::mousePressEvent(event);
    setFocus();
}
void HappyQtWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_pMouse->mouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
    setFocus();
}
void HappyQtWidget::closeEvent (QCloseEvent* /*event*/)
{
    HAPPYENGINE->quit();
}


void HappyQtWidget::keyPressEvent(QKeyEvent* event)
{
    m_pKeyboard->keyPressEvent(event);
    QWidget::keyPressEvent(event);
}
void HappyQtWidget::keyReleaseEvent(QKeyEvent* event)
{
    m_pKeyboard->keyReleaseEvent(event);
    QWidget::keyReleaseEvent(event);
}

} } //end namespace
#endif