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

#include "Sandbox.h"
#include "system/GLContextQT.h"

#include <GraphicsEngine.h>
#include <QMoveEvent>

namespace hs {

GameWidget::GameWidget(QWidget *parent): QGLWidget(QGLFormat::defaultFormat(), parent, Sandbox::getInstance()->getSharedWidget())
{
    setAutoBufferSwap(false);
}

GameWidget::~GameWidget()
{
}

void GameWidget::initializeGL()
{
    QGLWidget::initializeGL();
}
void GameWidget::paintGL()
{
    QGLWidget::paintGL();
}
void GameWidget::resizeGL(int width, int height)
{
    QGLWidget::resizeGL(width, height);
    Resized(width, height);
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
    Closed();
}

void GameWidget::moveEvent(QMoveEvent* event)
{
    Moved(event->pos().x(), event->pos().y());
}

void GameWidget::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);
}
void GameWidget::keyReleaseEvent(QKeyEvent* event)
{
    QWidget::keyReleaseEvent(event);
}

void GameWidget::focusInEvent(QFocusEvent* /*event*/)
{
    GRAPHICS->setActiveWindow(this);
    GainedFocus();
}

void GameWidget::focusOutEvent(QFocusEvent* /*event*/)
{
    LostFocus();
}

bool GameWidget::create(const bool show)
{
    bool result(false);

    m_Context = NEW GLContextQT();
    if (Window::create(show))
    {
        m_WindowRect.x = x();
        m_WindowRect.y = y();
        m_WindowRect.width = width();
        m_WindowRect.height = height();
        result = true;
    }
    return result;
}
void GameWidget::destroy()
{
    Window::destroy();
    delete m_Context;
    m_Context = nullptr;
}
bool GameWidget::isOpen() const
{
    return QGLWidget::isVisible();
}

void GameWidget::show()
{
    QGLWidget::show();
    Window::show();
}

void GameWidget::hide()
{
    QGLWidget::hide();
    Window::hide();
}

void GameWidget::setWindowTitle( const he::String& caption )
{
    QGLWidget::setWindowTitle(caption.c_str());
    Window::setWindowTitle(caption);
}

void GameWidget::setWindowPosition( int x, int y )
{
    move(x, y);
}

void GameWidget::setWindowDimension( he::uint32 width, he::uint32 height )
{
    resize(width, height);
}

void GameWidget::setVSync( bool enable )
{
    he::gfx::Window::setVSync(enable);
}

void GameWidget::setFullscreen(bool fullscreen)
{
    if (fullscreen)
        showFullScreen();
    else
        showNormal();
    he::gfx::Window::setFullscreen(fullscreen);
}

void GameWidget::setMousePosition( const he::vec2& /*pos*/ )
{

}

he::gfx::GLContext* GameWidget::getContext() const
{
    return m_Context;
}


} //end namespace

