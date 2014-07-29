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
//Created: 2013/04/01

#ifndef _HT_UIDebug_H_
#define _HT_UIDebug_H_
#pragma once

#include "system/UI.h"

namespace he {
namespace tools {
    class FPSGraph;
} }

namespace hs {

class UIDebug : public UI
{
public:
    UIDebug();
    virtual ~UIDebug();

    void load();
    void unload();

    void show();
    void hide();

    void repositionElements(const he::int32 width, const he::int32 height);

private:
    he::tools::FPSGraph* m_FPSGraph;
    he::eventCallback2<void, he::int32, he::int32> m_ResizeCallback;

    /* DEFAULT COPY & ASSIGNMENT */
    UIDebug(const UIDebug&);
    UIDebug& operator=(const UIDebug&);
};

} //end namespace

#endif
