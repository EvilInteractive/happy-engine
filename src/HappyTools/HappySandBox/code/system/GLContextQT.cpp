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
//Author:  Bastian Damman
//Created: 11/05/2014
#include "HappySandBoxPCH.h"
#include "GLContextQT.h"

#include "forms/RenderWidget.h"

#include <GraphicsEngine.h>
#include <Window.h>

#include <QOpenGLContext>
#include <QGLWidget>

namespace hs {

GLContextQT::GLContextQT()
    : m_Widget(nullptr)
{
}

bool GLContextQT::create(he::gfx::Window* const window)
{
    HE_ASSERT(window && window->getType() == HSFS::strQTWindow, "No window or wrong window type when creating context!");
    m_Widget = he::checked_cast<RenderWidget*>(window);
    return GLContext::create(window);
}
    
void GLContextQT::destroy()
{
    if (m_Widget != nullptr)
    {
        GLContext::destroy();
        m_Widget = nullptr;
    }
}
    
void GLContextQT::makeCurrent()
{
    m_Widget->makeCurrent();
}

} //end namespace
