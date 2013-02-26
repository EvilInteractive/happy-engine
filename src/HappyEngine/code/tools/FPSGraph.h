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
//Author: Sebastiaan Sprengers 
//Created: 30/08/2011

#ifndef _FPS_GRAPH_H_
#define _FPS_GRAPH_H_
#pragma once

#define BOOST_DISABLE_ASSERTS

#include "IDrawable2D.h"

namespace he {
namespace gfx {
    class Font;
    class View;
}
namespace gui {
    class Sprite;
    class Canvas2D;
}
namespace tools {

class FPSGraph : public gfx::IDrawable2D
{
public:

    enum Type
    {
        Type_Hide,
        Type_ToConsole,
        Type_TextOnly,
        Type_Full
    };

    enum PositionType
    {
        PositionType_Absolute,
        PositionType_Dock
    };

    enum Position
    {
        Position_TopLeft,
        Position_TopRight,
        Position_BottomLeft,
        Position_BottomRight
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    FPSGraph(float interval = 0.5f, uint16 recordTime = 60);
    virtual ~FPSGraph();

    /* GENERAL */
    void tick(float dTime);
    virtual void draw2D(gui::Canvas2D* canvas);

    /* GETTERS */
    uint16 getMaxFPS() const;
    uint16 getMinFPS() const;
    uint16 getAverageFPS() const;

    /* SETTERS */
    void setType(Type type);
    void setPositionType(PositionType posType);
    void setPos(const vec2& pos);
    void setPos(Position pos);

private:

    /* INTERNAL */
    inline uint16 cap(const float& fps) const;
    inline uint16 cap(const uint32& fps) const;
    void drawToConsole(gui::Canvas2D* canvas);
    void drawTextOnly(gui::Canvas2D* canvas);
    void drawFull(gui::Canvas2D* canvas);
    void updateScale(uint16 currentMaxFpsInFrame);

    void renderGraph();

    /* DATAMEMBERS */
    he::PrimitiveList<uint16> m_FpsHistory;

    float m_GameTime;
    float m_TBase;
    float m_CurrentDTime;
    float m_Interval;
    float m_AcumulatedDTime;

    uint16 m_CurrentFPS;
    uint16 m_Ticks;
    uint16 m_RecordTime;
    
    gui::Font* m_Font;
    gui::Text m_Text;

    int m_FPSGraphState;

    vec2 m_Pos;

    gui::Sprite* m_Sprites[2];
    uint8 m_ActiveSprite;

    float m_CurrentScale;

    float m_CurrentCPU;

    Color m_ColorWhite;
    Color m_ColorWhiteAlpha;
    Color m_ColorYellow;
    Color m_ColorYellowAlpha;
    Color m_ColorBlue;
    Color m_ColorBlueAlpha;
    Color m_ColorDarkGrey;
    Color m_ColorGrey;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    FPSGraph(const FPSGraph&);
    FPSGraph& operator=(const FPSGraph&);
};

} } //end namespace

#endif
