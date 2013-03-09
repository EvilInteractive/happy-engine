//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 09/03/2013

#ifndef _HT_MainGame_H_
#define _HT_MainGame_H_
#pragma once

#include <Game.h>

namespace he {
    namespace gfx {
        class Window;
    }
    namespace pl {
        class PluginLoader;
        class IPlugin;
    }
}

namespace ht {

class MainGame : public he::ge::Game
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void destroy();

private:

    /* DATAMEMBERS */
    he::gfx::Window* m_Window;
    he::pl::PluginLoader* m_PluginLoader;
    he::pl::IPlugin* m_Plugin;

    /* DEFAULT COPY & ASSIGNMENT */
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
