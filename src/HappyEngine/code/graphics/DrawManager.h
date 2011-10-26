//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 04/10/2011

#ifndef _HE_DRAW_MANAGER_H_
#define _HE_DRAW_MANAGER_H_
#pragma once

#include "IDrawable.h"
#include "Camera.h"
#include "DrawSettings.h"

namespace he {
namespace gfx {

class ShadowCaster;

class DrawManager
{
public:
    struct DrawElement
    {
        const IDrawable* pDrawable;
        float sorter;
        bool operator<(const DrawElement& other)
        {
            return sorter < other.sorter;
        }
    };
    enum Type
    {
        Type_Immediate,
        Type_FrontToBack,
        Type_BackToFront
    };

	DrawManager();
    virtual ~DrawManager();

    void init(const DrawSettings& settings);

    void begin(Type type, const Camera* pCamera);
    void end();
    
    void renderShadow();

    void draw(const IDrawable* pDrawabe);

    bool viewClip(const shapes::Sphere& boundingSphere);

private:

    Type m_Type;
    const Camera* m_pCamera;

    std::vector<DrawElement> m_DrawList;

    ShadowCaster* m_pShadowCaster;

    //Disable default copy constructor and default assignment operator
    DrawManager(const DrawManager&);
    DrawManager& operator=(const DrawManager&);
};

} } //end namespace

#endif
