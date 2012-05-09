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
//Created: 24/12/2011

#ifndef _HE_LOADING_SCREEN_H_
#define _HE_LOADING_SCREEN_H_
#pragma once

#include "vec2.h"
#include <vector>
#include "Texture2D.h"
#include "Font.h"

namespace he {
namespace tools {

class LoadingScreen
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    LoadingScreen();
    virtual ~LoadingScreen();

    /* GENERAL */
    void tick();
    void draw();

private:

    /* X-MAS! */
    struct SnowFlake
    {
        float size;
        vec2 speed;
        vec2 pos;
    };

    /* DATAMEMBERS */
    std::vector<SnowFlake> m_SnowFlakes;

    gfx::Font* m_pFontLoading;
    gfx::Font* m_pFontMessage;

    const gfx::Texture2D* m_pLogo;

    /* DEFAULT COPY & ASSIGNMENT */
    LoadingScreen(const LoadingScreen&);
    LoadingScreen& operator=(const LoadingScreen&);
};

} } //end namespace

#endif
