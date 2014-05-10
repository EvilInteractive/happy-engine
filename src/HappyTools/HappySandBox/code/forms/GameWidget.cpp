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
#include "HappySandBoxPCH.h"
#include "GameWidget.h"

namespace hs {

GameWidget::GameWidget(QWidget *parent): QGLWidget(QGLFormat(), parent)
{
    format().setProfile(QGLFormat::CoreProfile);
    format().setVersion(3, 3);
    format().setSwapInterval(0);
}


GameWidget::~GameWidget()
{
}

void GameWidget::init()
{

}

void GameWidget::destroy()
{

}

void GameWidget::initializeGL()
{
}
void GameWidget::paintGL()
{
}
void GameWidget::resizeGL(int width, int height)
{
}
void GameWidget::present()
{
    swapBuffers();
}

void GameWidget::mouseMoveEvent(QMouseEvent* event)
{
    QWidget::mouseMoveEvent(event);
}
void GameWidget::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    setFocus();
}
void GameWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
    setFocus();
}
void GameWidget::closeEvent (QCloseEvent* /*event*/)
{
}

void GameWidget::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);
}
void GameWidget::keyReleaseEvent(QKeyEvent* event)
{
    QWidget::keyReleaseEvent(event);
}

} //end namespace

