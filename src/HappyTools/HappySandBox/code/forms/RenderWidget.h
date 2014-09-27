//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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

#ifndef _HS_RENDER_WIDGET_H_
#define _HS_RENDER_WIDGET_H_
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

class RenderWidget :  public QGLWidget, public he::gfx::Window
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *parent = 0);
    virtual ~RenderWidget();

    // From Window
    bool create(const bool show) override;
    void destroy() override;

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
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void closeEvent(QCloseEvent* event) override;
    void moveEvent(QMoveEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    
    //Disable default copy constructor and default assignment operator
    RenderWidget(const RenderWidget&);
    RenderWidget& operator=(const RenderWidget&);
};

} //end namespace

#endif
