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
//Created: 2013/03/03

#ifndef _HT_HappyPluginTestMain_H_
#define _HT_HappyPluginTestMain_H_
#pragma once

#include "IPlugin.h"

namespace he {
    namespace gfx {
        class View;
        class Scene;
    }
    namespace ge {
        class DefaultRenderPipeline;
    }
}

namespace ht {

class HappyPluginTestMain : public he::pl::IPlugin
{
public:
    HappyPluginTestMain();
    virtual ~HappyPluginTestMain();

    virtual void sdmInit(he::io::BinaryVisitor& visitor);
    virtual void init(he::gfx::Window* const window, const he::RectI& viewport);
    virtual void terminate();

    virtual he::pl::EPluginType getPluginType() const { return he::pl::ePluginType_ToolGamePlugin; }

    // Events
    virtual void onLoadLevel(const he::Path& path);
    virtual void onUnloadLevel();
    virtual void onLevelLoaded();
    virtual void onStartGame();
    virtual void onStopGame();
    virtual void onPauseGame();
    virtual void onResumeGame();
    virtual void onResize(const he::RectI& newViewport);

private:
    he::gfx::View* m_View;
    he::gfx::Scene* m_Scene;
    he::ge::DefaultRenderPipeline* m_RenderPipeline;

    /* DEFAULT COPY & ASSIGNMENT */
    HappyPluginTestMain(const HappyPluginTestMain&);
    HappyPluginTestMain& operator=(const HappyPluginTestMain&);
};

} //end namespace

#endif
