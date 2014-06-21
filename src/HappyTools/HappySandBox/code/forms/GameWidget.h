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

#ifndef _HS_GAME_WIDGET_H_
#define _HS_GAME_WIDGET_H_
#pragma once

#include <QGLWidget>

#include <Window.h>

namespace he {
namespace io {
    class QtMouse;
    class QtKeyboard;
}}

namespace hs {
class GLContextQT;

class GameWidget :  public QGLWidget, public he::gfx::Window
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    virtual ~GameWidget();

    // From Window
    bool create(const bool show);
    void destroy();

    // Do
    virtual void present();
    void show();
    void hide();

    // Getters
    he::gfx::GLContext* getContext() const;
    const he::FixedString& getType() const { return HSFS::strQTWindow; }
    bool isOpen() const;

    // Setters
    void setWindowTitle(const he::String& caption);
    void setWindowPosition(int x, int y);
    void setWindowDimension(he::uint32 width, he::uint32 height);
    void setVSync(bool enable);
    void setFullscreen(bool fullscreen);
    void setMousePosition(const he::vec2& pos);
    // ~
    
private:
    // From QGLWidget
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void closeEvent(QCloseEvent* event);
    void moveEvent(QMoveEvent* event);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);

    GLContextQT* m_Context;

    //Disable default copy constructor and default assignment operator
    GameWidget(const GameWidget&);
    GameWidget& operator=(const GameWidget&);
};

} //end namespace

#endif
