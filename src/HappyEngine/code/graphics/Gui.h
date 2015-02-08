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
//Author:  Sebastiaan Sprengers
//Created: 27/11/2012

#ifndef _HE_GUI_H_
#define _HE_GUI_H_
#pragma once

#include "Renderer2D.h"
#include "Canvas2D.h"
#include "SpriteCreator.h"
#include "Sprite.h"
#include "Text.h"
#include "Font.h"
#include "Hitregion.h"

namespace he {
namespace gui {

struct Gui
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Gui() : m_SpriteCreator(HENew(SpriteCreator)()) {}

    ~Gui()
    {
        HEDelete(m_SpriteCreator);
    }

    /* GETTERS */
    SpriteCreator* getSpriteCreator() const
    {
        return m_SpriteCreator;
    }

private:

    /* MEMBERS */
    SpriteCreator* m_SpriteCreator;

    /* DEFAULT COPY & ASSIGNMENT */
    Gui(const Gui&);
    Gui& operator=(const Gui&);
};

}} //end namespace

#endif
