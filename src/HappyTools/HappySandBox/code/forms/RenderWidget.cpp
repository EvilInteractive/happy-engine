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
#include "HappySandBoxPCH.h"
#include "RenderWidget.h"

#include "Sandbox.h"
#include "system/GLContextQT.h"

#include <GraphicsEngine.h>
#include <ControlsManager.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <QMoveEvent>

namespace
{
    he::io::Key ConvertKey(const int key)
    {
        he::io::Key result(he::io::Key_Unknown);
        switch (key)
        {
        case Qt::Key_Escape: result = he::io::Key_Escape; break;
        case Qt::Key_Tab: result = he::io::Key_Tab; break;
        case Qt::Key_Backspace: result = he::io::Key_Backspace; break;
        case Qt::Key_Return: result = he::io::Key_Return; break;
        case Qt::Key_Enter: result = he::io::Key_KpEnter; break;
        case Qt::Key_Insert: result = he::io::Key_Insert; break;
        case Qt::Key_Delete: result = he::io::Key_Delete; break;
        case Qt::Key_Pause: result = he::io::Key_Pause; break;
        case Qt::Key_SysReq: result = he::io::Key_Sysreq; break;
        case Qt::Key_Clear: result = he::io::Key_Clear; break;
        case Qt::Key_Home: result = he::io::Key_Home; break;
        case Qt::Key_End: result = he::io::Key_End; break;
        case Qt::Key_Left: result = he::io::Key_Left; break;
        case Qt::Key_Up: result = he::io::Key_Up; break;
        case Qt::Key_Right: result = he::io::Key_Right; break;
        case Qt::Key_Down: result = he::io::Key_Down; break;
        case Qt::Key_PageUp: result = he::io::Key_Pageup; break;
        case Qt::Key_PageDown: result = he::io::Key_Pagedown; break;
        case Qt::Key_Shift: result = he::io::Key_Shift; break;
        case Qt::Key_Control: result = he::io::Key_Ctrl; break;
        case Qt::Key_Meta: result = he::io::Key_Lgui; break;
        case Qt::Key_Alt: result = he::io::Key_Alt; break;
        case Qt::Key_AltGr: result = he::io::Key_Ralt; break;
        case Qt::Key_CapsLock: result = he::io::Key_Capslock; break;
        case Qt::Key_NumLock: result = he::io::Key_Numlockclear; break;
        case Qt::Key_ScrollLock: result = he::io::Key_Scrolllock; break;
        case Qt::Key_F1: result = he::io::Key_F1; break;
        case Qt::Key_F2: result = he::io::Key_F2; break;
        case Qt::Key_F3: result = he::io::Key_F3; break;
        case Qt::Key_F4: result = he::io::Key_F4; break;
        case Qt::Key_F5: result = he::io::Key_F5; break;
        case Qt::Key_F6: result = he::io::Key_F6; break;
        case Qt::Key_F7: result = he::io::Key_F7; break;
        case Qt::Key_F8: result = he::io::Key_F8; break;
        case Qt::Key_F9: result = he::io::Key_F9; break;
        case Qt::Key_F10: result = he::io::Key_F10; break;
        case Qt::Key_F11: result = he::io::Key_F11; break;
        case Qt::Key_F12: result = he::io::Key_F12; break;
        case Qt::Key_F13: result = he::io::Key_F13; break;
        case Qt::Key_F14: result = he::io::Key_F14; break;
        case Qt::Key_F15: result = he::io::Key_F15; break;
        case Qt::Key_F16: result = he::io::Key_F16; break;
        case Qt::Key_F17: result = he::io::Key_F17; break;
        case Qt::Key_F18: result = he::io::Key_F18; break;
        case Qt::Key_F19: result = he::io::Key_F19; break;
        case Qt::Key_F20: result = he::io::Key_F20; break;
        case Qt::Key_F21: result = he::io::Key_F21; break;
        case Qt::Key_F22: result = he::io::Key_F22; break;
        case Qt::Key_F23: result = he::io::Key_F23; break;
        case Qt::Key_F24: result = he::io::Key_F24; break;
        case Qt::Key_Menu: result = he::io::Key_Menu; break;
        case Qt::Key_Help: result = he::io::Key_Help; break;
        case Qt::Key_Space: result = he::io::Key_Space; break;
        case Qt::Key_Exclam: result = he::io::Key_KpExclam; break;
        case Qt::Key_QuoteDbl: result = he::io::Key_KpDblampersand; break;
        case Qt::Key_QuoteLeft: result = he::io::Key_KpAmpersand; break;
        case Qt::Key_NumberSign: result = he::io::Key_Backslash; break;
        case Qt::Key_Dollar: result = he::io::Key_Backslash; break;
        case Qt::Key_Percent: result = he::io::Key_KpPercent; break;
        case Qt::Key_Ampersand: result = he::io::Key_KpAmpersand; break;
        case Qt::Key_Apostrophe: result = he::io::Key_Apostrophe; break;
        case Qt::Key_ParenLeft: result = he::io::Key_KpLeftparen; break;
        case Qt::Key_ParenRight: result = he::io::Key_KpRightparen; break;
        case Qt::Key_Asterisk: result = he::io::Key_Backslash; break;
        case Qt::Key_Plus: result = he::io::Key_KpPlus; break;
        case Qt::Key_Comma: result = he::io::Key_Comma; break;
        case Qt::Key_Minus: result = he::io::Key_Minus; break;
        case Qt::Key_Period: result = he::io::Key_Period; break;
        case Qt::Key_Slash: result = he::io::Key_Slash; break;
        case Qt::Key_0: result = he::io::Key_0; break;
        case Qt::Key_1: result = he::io::Key_1; break;
        case Qt::Key_2: result = he::io::Key_2; break;
        case Qt::Key_3: result = he::io::Key_3; break;
        case Qt::Key_4: result = he::io::Key_4; break;
        case Qt::Key_5: result = he::io::Key_5; break;
        case Qt::Key_6: result = he::io::Key_6; break;
        case Qt::Key_7: result = he::io::Key_7; break;
        case Qt::Key_8: result = he::io::Key_8; break;
        case Qt::Key_9: result = he::io::Key_9; break;
        case Qt::Key_Colon: result = he::io::Key_KpColon; break;
        case Qt::Key_Semicolon: result = he::io::Key_Semicolon; break;
        case Qt::Key_Less: result = he::io::Key_KpLess; break;
        case Qt::Key_Equal: result = he::io::Key_Equals; break;
        case Qt::Key_Greater: result = he::io::Key_KpGreater; break;
        case Qt::Key_At: result = he::io::Key_Lalt; break;
        case Qt::Key_A: result = he::io::Key_A; break;
        case Qt::Key_B: result = he::io::Key_B; break;
        case Qt::Key_C: result = he::io::Key_C; break;
        case Qt::Key_D: result = he::io::Key_D; break;
        case Qt::Key_E: result = he::io::Key_E; break;
        case Qt::Key_F: result = he::io::Key_F; break;
        case Qt::Key_G: result = he::io::Key_G; break;
        case Qt::Key_H: result = he::io::Key_H; break;
        case Qt::Key_I: result = he::io::Key_I; break;
        case Qt::Key_J: result = he::io::Key_J; break;
        case Qt::Key_K: result = he::io::Key_K; break;
        case Qt::Key_L: result = he::io::Key_L; break;
        case Qt::Key_M: result = he::io::Key_M; break;
        case Qt::Key_N: result = he::io::Key_N; break;
        case Qt::Key_O: result = he::io::Key_O; break;
        case Qt::Key_P: result = he::io::Key_P; break;
        case Qt::Key_Q: result = he::io::Key_Q; break;
        case Qt::Key_R: result = he::io::Key_R; break;
        case Qt::Key_S: result = he::io::Key_S; break;
        case Qt::Key_T: result = he::io::Key_T; break;
        case Qt::Key_U: result = he::io::Key_U; break;
        case Qt::Key_V: result = he::io::Key_V; break;
        case Qt::Key_W: result = he::io::Key_W; break;
        case Qt::Key_X: result = he::io::Key_X; break;
        case Qt::Key_Y: result = he::io::Key_Y; break;
        case Qt::Key_Z: result = he::io::Key_Z; break;
        case Qt::Key_BracketLeft: result = he::io::Key_Leftbracket; break;
        case Qt::Key_Backslash: result = he::io::Key_Backslash; break;
        case Qt::Key_BracketRight: result = he::io::Key_Rightbracket; break;
        case Qt::Key_BraceLeft: result = he::io::Key_KpLeftbrace; break;
        case Qt::Key_Bar: result = he::io::Key_KpVerticalbar; break;
        case Qt::Key_BraceRight: result = he::io::Key_KpRightbrace; break;
        case Qt::Key_AsciiTilde: result = he::io::Key_Grave; break;
        case Qt::Key_multiply: result = he::io::Key_KpMultiply; break;
        case Qt::Key_Mode_switch: result = he::io::Key_Mode; break;
        case Qt::Key_Back: result = he::io::Key_AcBack; break;
        case Qt::Key_Forward: result = he::io::Key_AcForward; break;
        case Qt::Key_Stop: result = he::io::Key_Stop; break;
        case Qt::Key_Refresh: result = he::io::Key_AcRefresh; break;
        case Qt::Key_VolumeDown: result = he::io::Key_Volumedown; break;
        case Qt::Key_VolumeMute: result = he::io::Key_Mute; break;
        case Qt::Key_VolumeUp: result = he::io::Key_Volumeup; break;
        case Qt::Key_MediaPlay: result = he::io::Key_Audioplay; break;
        case Qt::Key_MediaStop: result = he::io::Key_Audiostop; break;
        case Qt::Key_MediaPrevious: result = he::io::Key_Audioprev; break;
        case Qt::Key_MediaNext: result = he::io::Key_Audionext; break;
        case Qt::Key_Standby: result = he::io::Key_Sleep; break;
        case Qt::Key_MonBrightnessUp: result = he::io::Key_Brightnessup; break;
        case Qt::Key_MonBrightnessDown: result = he::io::Key_Brightnessdown; break;
        case Qt::Key_KeyboardBrightnessUp: result = he::io::Key_Brightnessup; break;
        case Qt::Key_KeyboardBrightnessDown: result = he::io::Key_Brightnessdown; break;
        case Qt::Key_PowerOff: result = he::io::Key_Power; break;
        case Qt::Key_Eject: result = he::io::Key_Eject; break;
        case Qt::Key_ApplicationLeft: result = he::io::Key_Application; break;
        case Qt::Key_ApplicationRight: result = he::io::Key_Application; break;
        case Qt::Key_Calculator: result = he::io::Key_Calculator; break;
        case Qt::Key_Copy: result = he::io::Key_Copy; break;
        case Qt::Key_Cut: result = he::io::Key_Cut; break;
        case Qt::Key_Paste: result = he::io::Key_Paste; break;
        case Qt::Key_Select: result = he::io::Key_Select; break;
        case Qt::Key_Execute: result = he::io::Key_Execute; break;
        case Qt::Key_Play: result = he::io::Key_Audioplay; break;
        case Qt::Key_Sleep: result = he::io::Key_Sleep; break;
        case Qt::Key_Cancel: result = he::io::Key_Cancel; break;
        }
        return result;
    }
    he::io::MouseButton ConvertMouseButton(Qt::MouseButton button)
    {
        he::io::MouseButton result(he::io::MouseButton_MAX);
        switch (button)
        {
        case Qt::LeftButton: result = he::io::MouseButton_Left; break;
        case Qt::RightButton: result = he::io::MouseButton_Right; break;
        case Qt::MiddleButton: result = he::io::MouseButton_Middle; break;
        case Qt::XButton1: result = he::io::MouseButton_X1; break;
        case Qt::XButton2: result = he::io::MouseButton_X2; break;
        }
        return result;
    }
}

namespace hs {

RenderWidget::RenderWidget(QWidget *parent): QGLWidget(QGLFormat::defaultFormat(), parent, Sandbox::getInstance()->getSharedWidget())
{
    m_Context = NEW GLContextQT();
    setAutoBufferSwap(false);
}

RenderWidget::~RenderWidget()
{
    delete m_Context;
    m_Context = nullptr;
}

void RenderWidget::initializeGL()
{
    QGLWidget::initializeGL();
}
void RenderWidget::paintGL()
{
    QGLWidget::paintGL();
}
void RenderWidget::resizeGL(int width, int height)
{
    QGLWidget::resizeGL(width, height);
    Resized(width, height);
}
void RenderWidget::present()
{
    swapBuffers();
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    he::io::IMouse* mouse(CONTROLS->getMouse(getHandle()));
    mouse->MouseMoved(he::vec2(event->x(), event->y()));
    QWidget::mouseMoveEvent(event);
}
void RenderWidget::mousePressEvent(QMouseEvent* event)
{
    he::io::IMouse* mouse(CONTROLS->getMouse(getHandle()));
    he::io::MouseButton button(ConvertMouseButton(event->button()));
    if (button != he::io::MouseButton_MAX)
        mouse->MouseButtonPressed(button);
    QWidget::mousePressEvent(event);
    setFocus();
}
void RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    he::io::IMouse* mouse(CONTROLS->getMouse(getHandle()));
    he::io::MouseButton button(ConvertMouseButton(event->button()));
    if (button != he::io::MouseButton_MAX)
        mouse->MouseButtonReleased(button);
    QWidget::mouseReleaseEvent(event);
    setFocus();
}

void RenderWidget::wheelEvent(QWheelEvent* event)
{
    he::io::IMouse* mouse(CONTROLS->getMouse(getHandle()));
    mouse->MouseWheelMoved(event->delta());
    QWidget::wheelEvent(event);
}
void RenderWidget::closeEvent (QCloseEvent* /*event*/)
{
    Closed();
}

void RenderWidget::moveEvent(QMoveEvent* event)
{
    Moved(event->pos().x(), event->pos().y());
}

void RenderWidget::keyPressEvent(QKeyEvent* event)
{
    he::io::IKeyboard* keyboard(CONTROLS->getKeyboard(getHandle()));
    if (event->isAutoRepeat() == false)
    {
        he::io::Key key(ConvertKey(event->key()));
        if (key != he::io::Key_Unknown)
            keyboard->KeyPressed(key);
    }
    if (event->text().size() > 0)
    {
        keyboard->TextEntered(event->text().toUtf8().data());
    }

    QWidget::keyPressEvent(event);
}
void RenderWidget::keyReleaseEvent(QKeyEvent* event)
{
    he::io::IKeyboard* keyboard(CONTROLS->getKeyboard(getHandle()));
    if (event->isAutoRepeat() == false)
    {
        he::io::Key key(ConvertKey(event->key()));
        if (key != he::io::Key_Unknown)
            keyboard->KeyReleased(key);
    }
    QWidget::keyReleaseEvent(event);
}

void RenderWidget::focusInEvent(QFocusEvent* /*event*/)
{
    GRAPHICS->setActiveWindow(this);
    GainedFocus();
}

void RenderWidget::focusOutEvent(QFocusEvent* /*event*/)
{
    LostFocus();
}

bool RenderWidget::create(const bool show)
{
    bool result(false);

    GRAPHICS->registerWindow(this);
    if (Window::create(show))
    {
        GRAPHICS->setActiveWindow(this);
        m_WindowRect.x = x();
        m_WindowRect.y = y();
        m_WindowRect.width = width();
        m_WindowRect.height = height();
        result = true;

        CONTROLS->registerInputForWindow(getHandle(), NEW he::io::Keyboard(), NEW he::io::Mouse());
    }
    return result;
}
void RenderWidget::destroy()
{
    CONTROLS->unregisterInputForWindow(getHandle());
    Window::destroy();
    GRAPHICS->unregisterWindow(this);
}
bool RenderWidget::isOpen() const
{
    return QGLWidget::isVisible() && checkFlag(Window::eFlags_IsVisible);
}

void RenderWidget::show()
{
    QGLWidget::show();
    Window::show();
}

void RenderWidget::hide()
{
    QGLWidget::hide();
    Window::hide();
}

void RenderWidget::setWindowTitle( const he::String& caption )
{
    QGLWidget::setWindowTitle(caption.c_str());
    Window::setWindowTitle(caption);
}

void RenderWidget::setWindowPosition( int x, int y )
{
    move(x, y);
}

void RenderWidget::setWindowDimension( he::uint32 width, he::uint32 height )
{
    resize(width, height);
}

void RenderWidget::setVSync( bool enable )
{
    he::gfx::Window::setVSync(enable);
}

void RenderWidget::setFullscreen(bool fullscreen)
{
    if (fullscreen)
        showFullScreen();
    else
        showNormal();
    he::gfx::Window::setFullscreen(fullscreen);
}

void RenderWidget::setMousePosition( const he::vec2& /*pos*/ )
{

}

he::gfx::GLContext* RenderWidget::getContext() const
{
    return m_Context;
}


} //end namespace

