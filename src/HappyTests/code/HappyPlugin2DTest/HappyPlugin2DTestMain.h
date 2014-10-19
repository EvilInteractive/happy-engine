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
//Created: 2014/10/19

#ifndef _HT_HappyPlugin2DTestMain_H_
#define _HT_HappyPlugin2DTestMain_H_
#pragma once

#include <IPlugin.h>
#include <IDrawable2D.h>

#include <GridLayout.h>

#include "ColouredRect.h"

namespace he {
    namespace gfx {
        class View;
        class Renderer2D;
    }
    namespace gui {
        class Text;
    }
}

namespace ht {
class FlyCamera;
class HappyPlugin2DTestMain : public he::pl::IPlugin, public he::gfx::IDrawable2D
{
public:
    HappyPlugin2DTestMain();
    virtual ~HappyPlugin2DTestMain();

    virtual void init(he::gfx::Window* const window, const he::RectF& viewport) override;
    virtual void terminate() override;

    virtual he::pl::EPluginType getPluginType() const override { return he::pl::ePluginType_ToolGamePlugin; }
    virtual he::gfx::Scene* getScene() const override { return nullptr; }
    virtual he::gfx::ICamera* getActiveCamera() const override { return nullptr; }
    virtual void setActiveCamera(he::gfx::ICamera* const /*camera*/) {}

    // Events
    virtual void onLoadLevel(const he::Path& path) override;
    virtual void onUnloadLevel() override;
    virtual void onLevelLoaded() override;
    virtual void onStartGame() override;
    virtual void onStopGame() override;
    virtual void onPauseGame() override;
    virtual void onResumeGame() override;

    virtual void draw2D(he::gui::Canvas2D* canvas) override;

private:
    he::gfx::View* m_View;
    he::gfx::Renderer2D* m_Renderer;

    he::gui::Text* m_DebugText;

    he::gui::GridLayout m_Grid;
    he::gui::GridLayout m_GridInGrid;
    he::ObjectList<ColouredRect> m_RectList;

    /* DEFAULT COPY & ASSIGNMENT */
    HappyPlugin2DTestMain(const HappyPlugin2DTestMain&);
    HappyPlugin2DTestMain& operator=(const HappyPlugin2DTestMain&);
};

} //end namespace

#endif
