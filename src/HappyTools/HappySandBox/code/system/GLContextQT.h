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

#ifndef _HE_GLContextQT_H_
#define _HE_GLContextQT_H_
#pragma once

#include "GLContext.h"

namespace he {
namespace gfx {
class Window;
} }

namespace hs {
class GameWidget;

class GLContextQT : public he::gfx::GLContext
{
public:
    GLContextQT();
    ~GLContextQT() {}
    
    bool create(he::gfx::Window* const window);
    void destroy();
    void makeCurrent();
    
    GameWidget* getWidget() const { return m_Widget; }

private:
    GameWidget* m_Widget;

    GLContextQT(const GLContextQT&);
    GLContextQT& operator=(const GLContextQT&);
};

} //end namespace

#endif
