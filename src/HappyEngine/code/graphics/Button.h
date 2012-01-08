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
//Author:  Sebastiaan Sprengers
//Created: 01/10/2011

#ifndef _HE_BUTTON_H_
#define _HE_BUTTON_H_
#pragma once

#include "vec2.h"
#include "HappyEngine.h"
#include "Texture2D.h"
#include <vector>
#include "Hitregion.h"
#include <string>
#include "Font.h"
#include "event.h"

namespace he {
namespace gui {

class Button
{
public:

    enum TYPE
    {
        TYPE_NORMAL = 0,
        TYPE_SPRITE = 1,
        TYPE_SPRITESHEET = 2
    };

    enum STATE
    {
        STATE_NORMAL = 0,
        STATE_HOVER = 1,
        STATE_DOWN = 2,
        STATE_DISABLED = 3
    };

    enum ACTIVATION
    {
        ACTIVATION_MOUSE = 0,
        ACTIVATION_MANUAL = 1
    };

    /* CONSTRUCTOR - DESTRUCTOR */
    Button(TYPE type, const vec2& centerPos, const vec2& size);
    virtual ~Button();

    /* GENERAL */
    virtual void tick();
    virtual void draw();

    /* SETTERS */
    virtual void setSpriteSheet(const gfx::Texture2D::pointer& pSpriteSheet);
    virtual void setSprites(	const gfx::Texture2D::pointer& pNormalSprite,
                                const gfx::Texture2D::pointer& pHoverSprite,
                                const gfx::Texture2D::pointer& pDownSprite,
                                const gfx::Texture2D::pointer& pDisabledSprite);
    virtual void setState(STATE state);
    virtual void setActivationMode(ACTIVATION activationMode);
    virtual void setPosition(const vec2& centerPos);
    virtual void setText(const std::string& text, ushort fontSize = 12);

    virtual void addOnClickListener(boost::function<void()> callback);

    /* GETTERS */
    virtual bool isNormal() const;
    virtual bool isHover() const;
    virtual bool isDown() const;
    virtual bool isDisabled() const;
    virtual bool isClicked() const;

protected:

    /* EXTRA */
    virtual void drawColor();
    virtual void drawSprites();
    virtual void drawSpriteSheet();

    /* CALLBACK HANDLERS */
    virtual void clicked();

    /* DATAMEMBERS */
    vec2 m_Pos;
    vec2 m_Size;

    bool m_bClicked;

    TYPE m_Type;
    STATE m_State;
    ACTIVATION m_ActivationType;

    std::vector<gfx::Texture2D::pointer> m_Sprites;
    gfx::Texture2D::pointer m_pSpriteSheet;

    Hitregion* m_pHitregion;

    std::string m_Text;
    gfx::Font::pointer m_pFont;

    event<void> m_OnClickEvent;

private:

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Button(const Button&);
    Button& operator=(const Button&);
};

} } //end namespace

#endif
