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
#include "StdAfx.h" 

#ifdef HE_ENABLE_QT
#include "HappyQtWidget.h"
#include "HappyNew.h"
#include "HappyEngine.h"
#include "GraphicsEngine.h"
#include "Happy2DRenderer.h"
#include "SimpleForward3DRenderer.h"
#include "QtMouse.h"
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
    init();
    if (GRAPHICS != nullptr)
        GRAPHICS->init(true);
    if (HE2D != nullptr)
        HE2D->initialize();
    if (HE3D != nullptr)       
		HE3D->init();
}
void HappyQtWidget::paintGL()
{
    makeCurrent();
    draw();
}
void HappyQtWidget::resizeGL(int /*width*/, int /*height*/)
{

}
void HappyQtWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_pMouse->mouseMoveEvent(event);
}
void HappyQtWidget::mousePressEvent(QMouseEvent* event)
{
    m_pMouse->mousePressEvent(event);
}
void HappyQtWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_pMouse->mouseReleaseEvent(event);
}

} } //end namespace
#endif