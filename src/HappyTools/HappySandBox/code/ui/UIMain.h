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

#ifndef _HT_UIMain_H_
#define _HT_UIMain_H_
#pragma once

#include "system/UI.h"

namespace Awesomium {
    class JSArray;
}

namespace he {
namespace gui {
    class WebView;
} }

namespace hs {

class UIMain : public UI
{
public:
    UIMain();
    virtual ~UIMain();

    void load();
    void unload();

    void show();
    void hide();

private:
    void createEntityHandler(const Awesomium::JSArray& args);

    he::gui::WebView* m_WebView;

    /* DEFAULT COPY & ASSIGNMENT */
    UIMain(const UIMain&);
    UIMain& operator=(const UIMain&);
};

} //end namespace

#endif
