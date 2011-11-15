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

#ifndef _HE_HAPPY_QT_WIDGET_H_
#define _HE_HAPPY_QT_WIDGET_H_
#pragma once

#ifdef HE_ENABLE_QT

#include "OpenGL.h"
#pragma warning(disable:4127)
#include <QGLWidget>
#pragma warning(default:4127)
#include "Game.h"

namespace he {
namespace io {
    class QtMouse;
    class QtKeyboard;
}}

namespace he {
namespace gfx {

class HappyQtWidget :  public /*qt::*/QGLWidget, public game::Game
{
    Q_OBJECT
public:
    HappyQtWidget(/*qt::*/QWidget *parent = 0);
    virtual ~HappyQtWidget();

    void present();
    
private:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void closeEvent (QCloseEvent* event);

    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    
    io::QtMouse* m_pMouse;
    io::QtKeyboard* m_pKeyboard;

    //Disable default copy constructor and default assignment operator
    HappyQtWidget(const HappyQtWidget&);
    HappyQtWidget& operator=(const HappyQtWidget&);
};

} } //end namespace

#endif
#endif
