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
//Author:  Sebastiaan Sprengers
//Created: 25/11/2012

#ifndef _HE_SPRITECREATOR_H_
#define _HE_SPRITECREATOR_H_
#pragma once

namespace he {
namespace gfx {
    class Canvas2DRendererCairo;
}
namespace gui {
    class Sprite;
    

class SpriteCreator
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    SpriteCreator();
    virtual ~SpriteCreator();

    /* GENERAL */
    void tick(float dTime);
    void glThreadInvoke();

    Sprite* createSprite(const vec2& size);
    // renders drawcalls to active sprite
    void renderSpriteAsync();

    /* GETTERS */
    
    /* SETTERS */
    // set colors
    void setColor(const Color& color);

    // set linewidth
    void setLineWidth(float width);

    /* DRAW */
    // start the next path from this point
    void moveTo(const vec2& pos);
    // create a line in the path to this point
    void lineTo(const vec2& pos);
    // create a rectangle in the path
    void rectangle(const vec2& pos, const vec2& size);
    // create a rounded rectangle
    void roundedRectangle(const vec2& pos, const vec2& size, float radius);
    // create a circle
    void circle(const vec2& pos, float radius);
    // create an arc shape
    void arc(const vec2& pos, float radius, float angleRadStart, float angleRadEnd);
    // create a curve
    void curveTo(const vec2& start, const vec2& middle, const vec2& end);
    // creates a new path
    void newPath();
    // closes current path
    void closePath();
    
    // stroke the current path
    void stroke();
    // fill the current path
    void fill();

private:

    /* MEMBERS */
    //PrimitiveList<Sprite*> m_Sprites;

    //uint16 m_ActiveSpriteID;

    gfx::Canvas2DRendererCairo* m_Renderer;
    Color m_Color;
    float m_LineWidth;

    /* DEFAULT COPY & ASSIGNMENT */ 
    SpriteCreator(const SpriteCreator&);
    SpriteCreator& operator=(const SpriteCreator&);
};

}}//end namespace

#endif
